#include "mystation.h"


MyStation::MyStation(int id, int num, QList<QString> *broadcast_channel, int speed, float send_frequency)
{
    station_id = id;
    all_stations = new int[num];
    for(int i=0; i<num; i++)
        all_stations[i]=0;
    channel = broadcast_channel;
    station_num = num;
    send_speed = speed;
    iscatch = false;
    frequency = send_frequency;
}

QList<QString> MyStation::getFrameBuffer()
{
    return frame_buffer;
}

// 产生一个想要发送的数据帧
void MyStation::Create_Frame(){
    // 生成一个0-100之间的随机数
    float num = QRandomGenerator::global()->bounded(100.0);
    // 按照概率决定是否生成数据帧
    if(num/100.0 < frequency){
        // TODO: 将发送数据帧的长度也随机
        float num2 = QRandomGenerator::global()->bounded(100.0);
        QString frame = "start" ;
        for(int i=0; i<int(num2) ; i++){
            frame += "0";
        }
        frame_buffer.push_back(frame);
    }
}

int* MyStation::getallstation()
{
    return all_stations;
}

// 向广播通道发送数据帧
int MyStation::Send_Frame(QString str){
    // 默认是发送帧数据
    if(str == "notend"){
        QString frame = frame_buffer.front();           // 要传送的数据帧
        frame_buffer.pop_front();
        QString send_buf = frame.mid(0, send_speed);    // 本次传入的数据帧
        // 如果这次没传完，则将剩余部分压入 frame_buffer 中
        if(send_buf.length() < frame.length()){
//            qDebug()<< "站点" + QString::number(station_id) + "压入的数据:" + frame.mid(send_speed, -1);
            frame_buffer.push_front(frame.mid(send_speed, -1));
            channel->push_back(send_buf);
            qDebug()<< "站点" + QString::number(station_id) + "发送的数据:" + send_buf;
            return 0;
        }
        else{
            channel->push_back(send_buf);
            qDebug()<< "站点" + QString::number(station_id) + "发送的数据:" + send_buf;
            return 1;
        }
    }
    // 否则发送结束信号
    else{
        channel->push_back(str);
        qDebug()<< "站点" + QString::number(station_id) + "发送的数据:" + str;
        return 1;
    }
}

// 接收信号
QString MyStation::Recv_Signal(){
    return channel->last();
}

// 发送信号
void MyStation::Send_Signal(QString signal){
    qDebug()<< "站点" + QString::number(station_id) + "发送的数据:" + signal;
    channel->push_back(signal);
}

// 收到 Send_MyID 信号之后
void MyStation::Listening2station(int station_id){
    all_stations[station_id] = 1;
}

// 槽函数
void MyStation::Listening2clock(QString signal){

    Create_Frame();
    // clock1-n 竞争期
    if(signal != "clock"){

        int num = signal.mid(5, -1).toInt();
        // 轮到自己发言了
        if(station_id == num){
            if(!frame_buffer.isEmpty()){
                SendSignal("1");
                emit Send_MyID(station_id);
            }
            else
                SendSignal("0");
        }
    }
    // clock 传输期
    else if(signal == "clock"){
//        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<";
//        qDebug() << "站点" << station_id << "收到的信号" << signal;
        // 检测是否轮到了自己
        for(int i=0; i<station_num; i++){
            if(all_stations[i] == 1){
                if(i == station_id)
                    iscatch = true;
                else
                    break;
            }
        }
        // 如果是则发送数据
        if(iscatch){
            // 如果发送完毕，则将 all_station 中的数据清空，并发送结束信号
            if(Send_Frame() == 1){
                iscatch = false;
                Send_Frame("end"+QString::number(station_id));
                all_stations[station_id] = 0;
            }
        }
        // 否则监听其他站的结束信号并修改 all_stations 中的内容
        else{
            QString recv_signal = Recv_Signal().right(4);
            // end1-n
            if(recv_signal.left(3) == "end"){
                int num = recv_signal.right(1).toInt();
                all_stations[num] = 0;
            }
        }
    }
}
