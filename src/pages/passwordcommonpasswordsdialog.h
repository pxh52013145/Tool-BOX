#pragma once

#include <QDialog>

class PasswordRepository;
class PasswordVault;

class QLabel;
class QStandardItemModel;
class QTableView;
class QPushButton;
class QTimer;

class PasswordCommonPasswordsDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordCommonPasswordsDialog(PasswordRepository *repo, PasswordVault *vault, QWidget *parent = nullptr);
    ~PasswordCommonPasswordsDialog() override;

private:
    void setupUi();
    void wireSignals();
    void reload();

    qint64 selectedId() const;

    void addItem();
    void editSelected();
    void deleteSelected();
    void copySelectedPassword();

    void updateUiState();

    PasswordRepository *repo_ = nullptr;
    PasswordVault *vault_ = nullptr;

    QLabel *hintLabel_ = nullptr;
    QTableView *tableView_ = nullptr;
    QStandardItemModel *model_ = nullptr;

    QPushButton *addBtn_ = nullptr;
    QPushButton *editBtn_ = nullptr;
    QPushButton *deleteBtn_ = nullptr;
    QPushButton *copyBtn_ = nullptr;

    QTimer *clipboardClearTimer_ = nullptr;
    QString lastClipboardSecret_;
};

