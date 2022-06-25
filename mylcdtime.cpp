#include "mylcdtime.h"
#include <QTime>
#include <QTimer>
#include <QMouseEvent>

MyLCDTime::MyLCDTime(QWidget *parent) : QLCDNumber(parent)
{
    setSegmentStyle(Filled);

    m_stringvalue = "";

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MyLCDTime::showTime);
    m_timer->start(1000);
}

MyLCDTime::~MyLCDTime()
{
    delete m_timer;
}

void MyLCDTime::showTime()
{
    QTime time = QTime::currentTime();
    if(time.second() % 2){
        m_stringvalue = time.toString("hh mm ss");
    }
    else{
        m_stringvalue = time.toString("hh:mm:ss");
    }
    display(m_stringvalue);
}

void MyLCDTime::mousePressEvent(QMouseEvent *)
{
    emit sendString(m_stringvalue);
    update();
}
