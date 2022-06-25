#ifndef MYLCDTIME_H
#define MYLCDTIME_H

#include <QLCDNumber>

class MyLCDTime : public QLCDNumber
{
    Q_OBJECT
public:
    MyLCDTime(QWidget *parent = nullptr);
    ~MyLCDTime();

public slots:
    void showTime();
signals:
    void sendString(QString str);
protected:
    void mousePressEvent(QMouseEvent *);
private:
    QTimer *m_timer;
    QString m_stringvalue;
};

#endif // MYLCDTIME_H
