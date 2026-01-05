#pragma once

#include <QMainWindow>

class TranslatorPage;

class TranslateMainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit TranslateMainWindow(QWidget *parent = nullptr);
    ~TranslateMainWindow() override;

    bool isReady() const;
    void activateFromMessage();

private:
    bool ensureInfrastructure();
    void setupUi();

    TranslatorPage *page_ = nullptr;
    bool ready_ = false;
};

