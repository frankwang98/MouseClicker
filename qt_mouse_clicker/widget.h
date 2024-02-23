#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


private:
    Ui::Widget *ui;

    bool m_shiftPressed = false;
    int m_clickCount = 0;

private slots:
    void checkRunning();

    bool eventFilter(QObject *watched, QEvent *event);

    void simulateMouseClick();
};

#endif // WIDGET_H
