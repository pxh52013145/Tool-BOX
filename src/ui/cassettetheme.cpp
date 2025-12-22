#include "cassettetheme.h"

#include <QApplication>
#include <QFont>

void CassetteTheme::apply()
{
    if (!qApp)
        return;

    QFont font("Consolas");
    font.setPointSize(10);
    qApp->setFont(font);
    qApp->setStyleSheet(styleSheet());
}

QString CassetteTheme::styleSheet()
{
    return QString::fromUtf8(R"qss(
        QWidget {
            background: #0b0b0b;
            color: #f59e0b;
            font-family: Consolas, "Cascadia Mono", "Microsoft YaHei UI";
        }

        QMainWindow {
            background: #0b0b0b;
        }

        QLabel#CassetteTitle {
            font-size: 32px;
            font-weight: 700;
            letter-spacing: 1px;
            color: #f59e0b;
        }

        QLabel#CassetteSubtitle {
            color: rgba(245, 158, 11, 0.55);
            letter-spacing: 3px;
        }

        QWidget#CassetteWindow {
            background: #0a0a0a;
            border: 2px solid rgba(146, 64, 14, 0.6);
            border-radius: 10px;
        }

        QWidget#CassetteTopBar {
            background: rgba(146, 64, 14, 0.16);
            border-top-left-radius: 10px;
            border-top-right-radius: 10px;
            border-bottom: 1px solid rgba(146, 64, 14, 0.35);
        }

        QToolButton {
            background: transparent;
            border: 1px solid rgba(245, 158, 11, 0.35);
            padding: 6px 10px;
            color: #f59e0b;
        }
        QToolButton:hover {
            background: rgba(245, 158, 11, 0.15);
        }

        QPushButton {
            background: transparent;
            border: 1px solid rgba(245, 158, 11, 0.5);
            padding: 6px 10px;
            color: #f59e0b;
        }
        QPushButton:hover {
            background: rgba(245, 158, 11, 0.18);
        }
        QPushButton:disabled {
            color: rgba(245, 158, 11, 0.35);
            border-color: rgba(245, 158, 11, 0.2);
        }

        QPlainTextEdit {
            background: rgba(0,0,0,0.35);
            border: 1px solid rgba(146, 64, 14, 0.5);
            border-radius: 8px;
            padding: 10px;
            selection-background-color: #f59e0b;
            selection-color: #0a0a0a;
        }

        QComboBox {
            background: rgba(0,0,0,0.35);
            border: 1px solid rgba(146, 64, 14, 0.55);
            padding: 4px 8px;
        }

        QListView {
            background: transparent;
            border: none;
            outline: 0;
        }
        QListView::item {
            background: rgba(0,0,0,0.30);
            border: 1px solid rgba(146, 64, 14, 0.35);
            padding: 8px;
            margin: 6px;
        }
        QListView::item:hover {
            border: 1px solid rgba(245, 158, 11, 0.65);
            background: rgba(245, 158, 11, 0.08);
        }
        QListView::item:selected {
            border: 1px solid rgba(245, 158, 11, 0.9);
            background: rgba(245, 158, 11, 0.14);
        }

        QHeaderView::section {
            background: rgba(146, 64, 14, 0.16);
            color: #f59e0b;
            border: 1px solid rgba(146, 64, 14, 0.35);
            padding: 6px;
        }

        QTableView {
            background: rgba(0,0,0,0.22);
            border: 1px solid rgba(146, 64, 14, 0.35);
        }
        QTableView::item {
            background: transparent;
            padding: 6px;
        }
    )qss");
}

