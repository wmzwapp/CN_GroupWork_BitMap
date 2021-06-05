#include "controlstation.h"
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QDebug>

// 当前发送期是否有数据发送
bool EMPTY = true;
// 最后一个有数据发送的站点的序号
int last_station = -1;
// 可以继续开始竞争期
bool CONTINUE = true;

bool plzStop=0;


ControlStation::ControlStation(int interval, int num, QList<QString> *broadcast_channel, int speed)
{
    time_interval = interval;
    station_num = num;
    count = 0;
    channel = broadcast_channel;
    send_speed = speed;
    use_rate = 0;
}

void ControlStation::endWork()
{
    flag=0;
}

void ControlStation::Sleeptt(float a)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed() <a*1000)
        QCoreApplication::processEvents();
}

QString ControlStation::Recv_Signal(){
    return channel->last();
}

void ControlStation::giveLove()
{
    plzStop=1;
}

void ControlStation::waitingForLove()
{
    Sleeptt(time_interval);
    while(!plzStop)
    {
        Sleeptt(0.01);
    }
    if(!alwaysWait)
        plzStop=0;
}

void ControlStation::setAlways()
{
    alwaysWait=!alwaysWait;
}


void ControlStation::start_work(){
    // 开始信号
    qDebug() << "控制站发送:\"clock0\"";
    waitingForLove();
    emit Send_Clock("clock0");

    waitingForLove();

    int clock_count = 0;        // 计算clock的次数

    while(flag){
        QString recv_signal = Recv_Signal();                    // 监听是否有是否有站点要发送数据，并记下该站点的序号
        QString signal = "";                                    // 发送的信号
        // 在每次发送时钟信号之前先监听channel
        if(recv_signal == "1"){                                 // 有站点想发送数据
            EMPTY = false;
            last_station = count;
        }
        else if(recv_signal != "1" && recv_signal != "0"){      // 站点已经开始发送数据
            CONTINUE = false;
            if(recv_signal == "end" + QString::number(last_station)){    // 直到监听到某个站点发送完数据
                use_rate = float(clock_count*send_speed)/float(clock_count*send_speed+station_num);
                qDebug() << "clock_count:"<<clock_count<<"send_speed:"<<send_speed<<"use_rate:"<< use_rate;
                CONTINUE = true;
                EMPTY = true;
            }
        }
        // 如果竞争期结束且有站点发数据||站点正在发送数据
        if(!CONTINUE || (!EMPTY && count == station_num-1)){
            clock_count ++;
            signal = "clock";
//            qDebug() << "CONTINUE:" << CONTINUE << " EMPTY:" << EMPTY;
            qDebug() << "控制站发送:" << signal;
            emit Send_Clock(signal);
        }
        // 如果还处于竞争期
        else{
            clock_count = 0;
            count ++;
            if(count == station_num)
                count = 0;
            signal = "clock" + QString::number(count);
            qDebug() << "控制站发送:" << signal;
            emit Send_Clock(signal);
        }
        waitingForLove();
    }
}
