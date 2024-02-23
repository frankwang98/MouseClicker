#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QCursor>
#include <QKeyEvent>
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("鼠标连点器 MouseClicker");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::checkRunning);
    timer->start(500); // 每隔500毫秒执行一次

    qApp->installEventFilter(this); // 安装事件过滤器
}

Widget::~Widget()
{
    delete ui;
}

void Widget::checkRunning()
{
    QProcess process;
    process.start("tasklist", QStringList() << "/FI" << "IMAGENAME eq WeChat.exe");
    process.waitForFinished(-1);

    QString taskListOutput = process.readAllStandardOutput();
    if (taskListOutput.contains("WeChat.exe")) {
        qDebug() << "WeChat程序正在运行";
    } else {
        qDebug() << "WeChat程序未在运行";
    }
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Shift) {
            m_shiftPressed = true;
        }
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Shift) {
            m_shiftPressed = false;
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton && m_shiftPressed) {
            simulateMouseClick(); // 模拟鼠标连点
        }
    }

    return QObject::eventFilter(watched, event);
}

void Widget::simulateMouseClick()
{
    const int clickInterval = 100; // 点击间隔为100毫秒
    const int totalClicks = 5; // 总共点击5次

    for (int i = 0; i < totalClicks; ++i) {
        QCursor::setPos(QCursor::pos()); // 移动到当前位置
        QThread::msleep(clickInterval); // 等待点击间隔
    }

    m_clickCount += totalClicks;
    qDebug() << "已执行鼠标连点" << m_clickCount << "次";

    m_clickCount = 0;
}
