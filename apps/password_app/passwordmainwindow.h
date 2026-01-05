#pragma once

#include <QMainWindow>

class PasswordManagerPage;

class PasswordMainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit PasswordMainWindow(QWidget *parent = nullptr);
    ~PasswordMainWindow() override;

    bool isReady() const;
    void activateFromMessage();

private:
    bool ensureInfrastructure();
    void setupUi();

    PasswordManagerPage *page_ = nullptr;
    bool ready_ = false;
};

