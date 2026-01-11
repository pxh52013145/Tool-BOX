#include "passwordcommonpasswordsdialog.h"

#include "password/passwordrepository.h"
#include "password/passwordvault.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>
#include <QVBoxLayout>

namespace {

class CommonPasswordEditDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit CommonPasswordEditDialog(QWidget *parent = nullptr) : QDialog(parent) { setupUi(); }

    void setData(const PasswordCommonPasswordSecrets &data)
    {
        data_ = data;
        if (nameEdit_)
            nameEdit_->setText(data.item.name);
        if (pwdEdit_)
            pwdEdit_->setText(data.password);
        if (notesEdit_)
            notesEdit_->setText(data.notes);
    }

    PasswordCommonPasswordSecrets data() const { return data_; }

private:
    void setupUi()
    {
        setWindowTitle("常用密码");
        resize(520, 240);

        auto *root = new QVBoxLayout(this);
        auto *form = new QFormLayout();

        nameEdit_ = new QLineEdit(this);
        nameEdit_->setPlaceholderText("例如：常用强密码 / 临时密码");
        form->addRow("名称：", nameEdit_);

        pwdEdit_ = new QLineEdit(this);
        pwdEdit_->setEchoMode(QLineEdit::Password);
        pwdEdit_->setPlaceholderText("输入密码");
        form->addRow("密码：", pwdEdit_);

        notesEdit_ = new QLineEdit(this);
        notesEdit_->setPlaceholderText("可选：备注（MVP 先用单行）");
        form->addRow("备注：", notesEdit_);

        root->addLayout(form);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        buttons->button(QDialogButtonBox::Ok)->setText("保存");
        buttons->button(QDialogButtonBox::Cancel)->setText("取消");
        root->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
            if (!nameEdit_ || !pwdEdit_ || !notesEdit_)
                return;

            data_.item.name = nameEdit_->text().trimmed();
            data_.password = pwdEdit_->text();
            data_.notes = notesEdit_->text();

            if (data_.item.name.isEmpty()) {
                QMessageBox::warning(this, "提示", "名称不能为空");
                return;
            }
            if (data_.password.isEmpty()) {
                QMessageBox::warning(this, "提示", "密码不能为空");
                return;
            }
            accept();
        });
    }

    PasswordCommonPasswordSecrets data_;
    QLineEdit *nameEdit_ = nullptr;
    QLineEdit *pwdEdit_ = nullptr;
    QLineEdit *notesEdit_ = nullptr;
};

QString formatUpdated(const QDateTime &dt)
{
    if (!dt.isValid())
        return {};
    return dt.toString("yyyy-MM-dd HH:mm");
}

} // namespace

PasswordCommonPasswordsDialog::PasswordCommonPasswordsDialog(PasswordRepository *repo, PasswordVault *vault, QWidget *parent)
    : QDialog(parent), repo_(repo), vault_(vault)
{
    setupUi();
    wireSignals();
    reload();
    updateUiState();
}

PasswordCommonPasswordsDialog::~PasswordCommonPasswordsDialog()
{
    lastClipboardSecret_.clear();
}

void PasswordCommonPasswordsDialog::setupUi()
{
    setWindowTitle("常用密码区（演示）");
    resize(860, 520);

    auto *root = new QVBoxLayout(this);

    auto *tip = new QLabel("用于保存“无账号”的常用密码，并支持快速注入到登录/注册/改密页面（后续在 Web 助手中使用）。", this);
    tip->setWordWrap(true);
    root->addWidget(tip);

    hintLabel_ = new QLabel(this);
    hintLabel_->setText("提示：复制后 15 秒自动清空剪贴板");
    root->addWidget(hintLabel_);

    model_ = new QStandardItemModel(this);
    model_->setColumnCount(2);
    model_->setHorizontalHeaderLabels({"名称", "最近更新"});

    tableView_ = new QTableView(this);
    tableView_->setModel(model_);
    tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView_->setSortingEnabled(true);
    tableView_->setAlternatingRowColors(true);
    tableView_->horizontalHeader()->setStretchLastSection(true);
    tableView_->verticalHeader()->setVisible(false);
    root->addWidget(tableView_, 1);

    auto *row = new QHBoxLayout();
    addBtn_ = new QPushButton("新增", this);
    editBtn_ = new QPushButton("编辑", this);
    deleteBtn_ = new QPushButton("删除", this);
    copyBtn_ = new QPushButton("复制密码", this);
    row->addWidget(addBtn_);
    row->addWidget(editBtn_);
    row->addWidget(deleteBtn_);
    row->addWidget(copyBtn_);
    row->addStretch(1);
    root->addLayout(row);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttons->button(QDialogButtonBox::Close)->setText("关闭");
    root->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    clipboardClearTimer_ = new QTimer(this);
    clipboardClearTimer_->setInterval(15000);
    clipboardClearTimer_->setSingleShot(true);
    connect(clipboardClearTimer_, &QTimer::timeout, this, [this]() {
        auto *cb = QApplication::clipboard();
        if (cb->text() == lastClipboardSecret_)
            cb->clear();
        lastClipboardSecret_.clear();
    });
}

void PasswordCommonPasswordsDialog::wireSignals()
{
    connect(addBtn_, &QPushButton::clicked, this, &PasswordCommonPasswordsDialog::addItem);
    connect(editBtn_, &QPushButton::clicked, this, &PasswordCommonPasswordsDialog::editSelected);
    connect(deleteBtn_, &QPushButton::clicked, this, &PasswordCommonPasswordsDialog::deleteSelected);
    connect(copyBtn_, &QPushButton::clicked, this, &PasswordCommonPasswordsDialog::copySelectedPassword);

    connect(tableView_, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        if (!index.isValid())
            return;
        editSelected();
    });

    connect(tableView_->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() { updateUiState(); });
}

void PasswordCommonPasswordsDialog::reload()
{
    if (!model_)
        return;

    if (hintLabel_)
        hintLabel_->setText("提示：复制后 15 秒自动清空剪贴板");

    model_->removeRows(0, model_->rowCount());

    if (!repo_ || !vault_) {
        QMessageBox::warning(this, "失败", "对话框未初始化");
        return;
    }

    const auto items = repo_->listCommonPasswords();
    model_->setRowCount(items.size());
    for (int i = 0; i < items.size(); ++i) {
        const auto &it = items.at(i);

        auto *nameItem = new QStandardItem(it.name);
        nameItem->setData(it.id, Qt::UserRole + 1);
        model_->setItem(i, 0, nameItem);

        model_->setItem(i, 1, new QStandardItem(formatUpdated(it.updatedAt)));
    }

    if (items.isEmpty())
        hintLabel_->setText("暂无常用密码：点击“新增”创建一个用于演示注入");
}

qint64 PasswordCommonPasswordsDialog::selectedId() const
{
    if (!tableView_ || !model_)
        return 0;

    const auto idx = tableView_->currentIndex();
    if (!idx.isValid())
        return 0;

    const auto id = model_->index(idx.row(), 0).data(Qt::UserRole + 1).toLongLong();
    return id;
}

void PasswordCommonPasswordsDialog::addItem()
{
    if (!repo_ || !vault_)
        return;
    if (!vault_->isUnlocked()) {
        QMessageBox::information(this, "提示", "请先解锁 Vault");
        return;
    }

    CommonPasswordEditDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    if (!repo_->addCommonPassword(dlg.data())) {
        QMessageBox::warning(this, "失败", repo_->lastError());
        return;
    }

    reload();
    updateUiState();
}

void PasswordCommonPasswordsDialog::editSelected()
{
    if (!repo_ || !vault_)
        return;
    if (!vault_->isUnlocked()) {
        QMessageBox::information(this, "提示", "请先解锁 Vault");
        return;
    }

    const auto id = selectedId();
    if (id <= 0)
        return;

    const auto loaded = repo_->loadCommonPassword(id);
    if (!loaded.has_value()) {
        QMessageBox::warning(this, "失败", repo_->lastError());
        return;
    }

    CommonPasswordEditDialog dlg(this);
    dlg.setData(loaded.value());
    if (dlg.exec() != QDialog::Accepted)
        return;

    auto data = dlg.data();
    data.item.id = id;
    if (!repo_->updateCommonPassword(data)) {
        QMessageBox::warning(this, "失败", repo_->lastError());
        return;
    }

    reload();
    updateUiState();
}

void PasswordCommonPasswordsDialog::deleteSelected()
{
    if (!repo_ || !vault_)
        return;
    if (!vault_->isUnlocked()) {
        QMessageBox::information(this, "提示", "请先解锁 Vault");
        return;
    }

    const auto id = selectedId();
    if (id <= 0)
        return;

    if (QMessageBox::question(this, "删除", "删除选中的常用密码？") != QMessageBox::Yes)
        return;

    if (!repo_->deleteCommonPassword(id)) {
        QMessageBox::warning(this, "失败", repo_->lastError());
        return;
    }

    reload();
    updateUiState();
}

void PasswordCommonPasswordsDialog::copySelectedPassword()
{
    if (!repo_ || !vault_)
        return;
    if (!vault_->isUnlocked()) {
        QMessageBox::information(this, "提示", "请先解锁 Vault");
        return;
    }

    const auto id = selectedId();
    if (id <= 0)
        return;

    const auto loaded = repo_->loadCommonPassword(id);
    if (!loaded.has_value()) {
        QMessageBox::warning(this, "失败", repo_->lastError());
        return;
    }

    if (QMessageBox::question(this, "复制密码", "复制密码到剪贴板？") != QMessageBox::Yes)
        return;

    QApplication::clipboard()->setText(loaded->password);
    lastClipboardSecret_ = loaded->password;
    clipboardClearTimer_->start();
    hintLabel_->setText("密码已复制（15 秒后自动清空）");
}

void PasswordCommonPasswordsDialog::updateUiState()
{
    const auto hasSelection = selectedId() > 0;
    if (editBtn_)
        editBtn_->setEnabled(hasSelection);
    if (deleteBtn_)
        deleteBtn_->setEnabled(hasSelection);
    if (copyBtn_)
        copyBtn_->setEnabled(hasSelection);
}

#include "passwordcommonpasswordsdialog.moc"
