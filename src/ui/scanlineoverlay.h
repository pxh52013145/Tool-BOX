#pragma once

#include <QWidget>

class ScanlineOverlay final : public QWidget
{
    Q_OBJECT

public:
    explicit ScanlineOverlay(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

