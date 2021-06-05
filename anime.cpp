#include "anime.h"
#include "ui_anime.h"

#include "mystation.h"
#include "controlstation.h"

#include <QVector>

QList<QString> channel;
int station_num = 6;
float time_interval = 1;

void Anime::drawGroove(QVector<QPushButton*> QVpushbutton,QLabel* labelHint, QString signal)
{
    QString string;
    //设置提示文字
    QFont ft;
    ft.setPointSize(12);
    ft.setBold(1);
    labelHint->setFont(ft);
    // clock1-n 竞争期
    if(signal != "clock"){
        string="现在是竞争期,由信号站"+signal.mid(5, -1)+"发送信号!";
        labelHint->setText(string);
        int num = signal.mid(5, -1).toInt();
        //将对应的槽设为红色
        for(int i=0;i<station_num;i++)
        {
            QVpushbutton[i]->setStyleSheet("background-color:rgb(255,250,250)");
        }
        QVpushbutton[num]->setStyleSheet("background-color:rgb(255,69,0)");

    }

    // clock 传输期
    else if(signal == "clock"){
        labelHint->setText("现在是传输期，各站点将按顺序发送帧");
        for(int i=0;i<station_num;i++)
        {
            QVpushbutton[i]->setStyleSheet("background-color:rgb(119,136,153)");
        }
    }
}

void Anime::setGroove(QVector<QPushButton*> QVpushbutton,int i, QString signal)
{
    if(i==0)
        for(int i=0;i<station_num;i++)
            QVpushbutton[i]->setText("0");
    QVpushbutton[i]->setText(signal);
}

void Anime::setHint2(QVector<QPushButton *> QVpushbutton,QLabel* labelHint2)
{
    QString string;
    QFont ft;
    ft.setPointSize(10);
    ft.setBold(1);
    labelHint2->setFont(ft);
    string="本次传输期需要发送帧的信号站(按顺序):";
    for(int i=0;i<station_num;i++)
    {
        if(QVpushbutton[i]->text()=="1")
        {
            string+=QString::number(i);
            string+=" ";
        }
    }
    labelHint2->setText(string);
}

void Anime::setTextBrowser(QTextBrowser *textBrowser, int i,QList<QString> *channel,int lasti)
{

    QString string;
//    string="传送站"+QString::number(i)+"正在发送帧...\n";
    if(channel->last().left(3)=="end"&&lasti>=0)
        string+="传送站"+QString::number(lasti)+"发送的数据:"+channel->at(channel->size()-2);
    else
        string+="传送站"+QString::number(i)+"发送的数据:"+channel->last();

    textBrowser->append(string);


}

Anime::Anime(int stationNum,int timeInterval,int frequency,int zhenLength, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Anime)
{
    ui->setupUi(this);
    //展示演示窗口
    station_num=stationNum;
    time_interval=timeInterval;
    float FRequency=frequency*0.05;
    int ZHENLength=zhenLength;

    show();
    setWindowTitle("位图协议演示");
    resize(800,700);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->label_8->setVisible(false);

    //建立controlstation对象
    QVector<MyStation*> pstations;
    for(int i=0; i<station_num; i++){
        MyStation *station = new MyStation(i, station_num, &channel, ZHENLength, FRequency);
        pstations.append(station);
    }

    // 建立控制站对象
    ControlStation *controlstation = nullptr;
    controlstation = new ControlStation(time_interval, station_num, &channel, ZHENLength);




    // 绑定信号和槽
    for(int i=0; i<station_num; i++){
        connect(controlstation, &ControlStation::Send_Clock, pstations[i], &MyStation::Listening2clock);
        for(int j=0; j<station_num; j++){
            connect(pstations[i], &MyStation::Send_MyID, pstations[j], &MyStation::Listening2station);
        }
    }

    int lasti=-1;
    //传输期 显示textBrowser
    connect(controlstation,&ControlStation::Send_Clock,[&](QString Signal){
        if(Signal=="clock"){
            ui->textBrowser->setVisible(true);
            for(int i=0;i<station_num;i++){
                if(pstations[i]->getallstation()[i]==1){
                    setTextBrowser(ui->textBrowser,i,&channel,lasti);
                    lasti=i;
                    break;
                }
            }

        }
        else{
            lasti=-1;
            ui->textBrowser->setVisible(false);
            ui->textBrowser->setText("");
        }

    });

    //将按钮存入QVector
    QVector<QPushButton*> QVpushbutton;
    QVpushbutton.append(ui->pushButton);
    QVpushbutton.append(ui->pushButton_2);
    QVpushbutton.append(ui->pushButton_3);
    QVpushbutton.append(ui->pushButton_4);
    QVpushbutton.append(ui->pushButton_5);
    QVpushbutton.append(ui->pushButton_6);
    QVpushbutton.append(ui->pushButton_7);
    QVpushbutton.append(ui->pushButton_9);
    QVpushbutton.append(ui->pushButton_10);
    //将标签存入QVector
    QVector<QLabel*> QVLabel;
    QVLabel.append(ui->label);
    QVLabel.append(ui->label_2);
    QVLabel.append(ui->label_3);
    QVLabel.append(ui->label_4);
    QVLabel.append(ui->label_5);
    QVLabel.append(ui->label_6);
    QVLabel.append(ui->label_7);
    QVLabel.append(ui->label_9);
    QVLabel.append(ui->label_10);
    //将'要'存入QVector
    QVector<QPushButton*> QVpushbuttonYao;
    QVpushbuttonYao.append(ui->pushButton_11);
    QVpushbuttonYao.append(ui->pushButton_12);
    QVpushbuttonYao.append(ui->pushButton_13);
    QVpushbuttonYao.append(ui->pushButton_14);
    QVpushbuttonYao.append(ui->pushButton_15);
    QVpushbuttonYao.append(ui->pushButton_16);
    QVpushbuttonYao.append(ui->pushButton_17);
    QVpushbuttonYao.append(ui->pushButton_18);
    QVpushbuttonYao.append(ui->pushButton_19);

    for(int i=station_num;i<9;i++)
    {
        QVpushbutton[i]->setVisible(false);
        QVLabel[i]->setVisible(false);
        QVpushbuttonYao[i]->setVisible(false);
    }

    for(int i=0;i<9;i++)
    {
        QVpushbuttonYao[i]->setVisible(false);
    }

    for(int i=0;i<station_num;i++)
    {
        QVpushbutton[i]->setStyleSheet("background-color:rgb(255,250,250)");
    }

    ui->labelHint2->setVisible(false);
    ui->textBrowser->setVisible(false);

    //将Send_Signal信号化
    for(int i=0; i<station_num; i++)
        connect(pstations[i], &MyStation::SendSignal, pstations[i], &MyStation::Send_Signal);

    //当控制站发clock信号时，改变竞争槽颜色
    connect(controlstation,&ControlStation::Send_Clock,[=](QString signal){
        drawGroove(QVpushbutton,ui->labelHint,signal);
        if(controlstation->use_rate>0){
            ui->label_8->setText(QString("信道利用率=%1\%").arg(controlstation->use_rate*100));
            ui->label_8->setVisible(true);
        }
        else
            ui->label_8->setVisible(false);
    });

    //当站点发送信号1时，将竞争槽置1
    for(int i=0;i<station_num;i++)
    {
        connect(pstations[i],&MyStation::SendSignal,this,[=](QString signal){
                setGroove(QVpushbutton,i,signal);
        });
    }

    //传输期 显示labelHint2
    connect(controlstation,&ControlStation::Send_Clock,[=](QString Signal){
        if(Signal=="clock"){
            ui->labelHint2->setVisible(true);
            setHint2(QVpushbutton,ui->labelHint2);
        }
        else
            ui->labelHint2->setVisible(false);
    });






    connect(ui->pbExit,&QPushButton::clicked,[=](){
        close();
        controlstation->endWork();
    });

    connect(ui->pbLove,&QPushButton::clicked,controlstation,&ControlStation::giveLove);

    connect(ui->pbAlways,&QPushButton::clicked,controlstation,[=](){
        controlstation->setAlways();
        if(controlstation->alwaysWait==1)
            ui->pbAlways->setText("stop");
        else
            ui->pbAlways->setText("auto");
    });

    for(int i=0;i<station_num;i++)
        connect(controlstation,&ControlStation::Send_Clock,[=](){
            QVpushbuttonYao[i]->setVisible(false);
            if(!pstations[i]->getFrameBuffer().isEmpty())
                QVpushbuttonYao[i]->setVisible(true);
        });

    // 开始工作
    controlstation->start_work();

}

Anime::~Anime()
{
    delete ui;
}
