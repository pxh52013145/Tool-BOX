#include "scanlineoverlay.h"

#include <QPainter>
#include <QPainterPath>

ScanlineOverlay::ScanlineOverlay(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
}

void ScanlineOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    p.fillRect(rect(), QColor(0, 0, 0, 0));

    const int lineHeight = 3;
    QColor lineColor(245, 158, 11, 18);
    for (int y = 0; y < height(); y += lineHeight) {
        p.fillRect(0, y, width(), 1, lineColor);
    }

    QRadialGradient gradient(rect().center(), qMax(width(), height()) * 0.7);
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 180));
    p.fillRect(rect(), gradient);
}

