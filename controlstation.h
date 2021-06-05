#ifndef CONTROLSTATION_H
#define CONTROLSTATION_H

#include <iostream>
#include <QObject>
#include <QQueue>
#include <QList>
#include <QString>

using namespace std;

class ControlStation:public QObject
{
    Q_OBJECT
private:
    int time_interval;          // 时间单位
    int station_num;            // 站点的数量
    int count;                  // 当前已经发送的时钟信号次数
    QList<QString> *channel;   // 指向广播信道的地址
    int send_speed;                     // 传送的速度（每个时间单位传送多少位）
    bool flag=1;//0--结束start work

public:
    float use_rate;

    ControlStation(int interval, int station_num, QList<QString> *channel, int send_speed);
    ~ControlStation(){};
    //
    QString Recv_Signal();
    //
    void start_work();
    void Sleeptt(float a);
    //循环等待信号
    void waitingForLove();

    //停止等待槽函数
    void giveLove();

    //设置自动
    bool alwaysWait=0;
    void setAlways();
    void endWork();

signals:
    void Send_Clock(QString signal);        // 信号

};

#endif // CONTROLSTATION_H
