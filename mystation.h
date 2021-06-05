#ifndef MYSTATION_H
#define MYSTATION_H

#include <iostream>
#include <QObject>
#include <QQueue>
#include <QList>
#include <QString>
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>


using namespace std;

class MyStation:public QObject
{
    Q_OBJECT

private:
    int station_id;                     // 站点的唯一标识符
    QList<QString> frame_buffer;       // 帧缓冲队列
    int *all_stations;                  // 记录竞争期时每个站广播的信号
    QList<QString> *channel;           // 指向广播信道的地址
    int station_num;                    // 所有站点的数量
    int send_speed;                     // 传送的速度（每个时间单位传送多少位）
    bool iscatch;                       // 是否占有信道
    float frequency;                    // 发送信号的频率

public:
    MyStation(int id, int station_num, QList<QString> *channel, int speed, float frequency);
    ~MyStation(){}

    // 接收信号
    QString Recv_Signal();

    // 发送信号
    void Send_Signal(QString signal);

    // 发送数据帧
    int Send_Frame(QString="notend");

    // 产生数据帧
    void Create_Frame();

    int* getallstation();

    QList<QString> getFrameBuffer();

signals:
    void Send_MyID(int MyID);
    void SendSignal(QString signal);

public slots:
    // 槽函数，当收到 controlstation 的信号之后执行
    void Listening2clock(QString signal);
    // 当收到有别的站点广播自己想要发送数据时执行
    void Listening2station(int station_id);
};

#endif // MYSTATION_H
