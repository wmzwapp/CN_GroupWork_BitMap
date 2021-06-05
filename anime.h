#ifndef ANIME_H
#define ANIME_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>

namespace Ui {
class Anime;
}

class Anime : public QWidget
{
    Q_OBJECT

public:
    explicit Anime(int stationNum,int timeInterval,int frequency,int zhenLength, QWidget *parent = nullptr);
    ~Anime();

    //提示信息和竞争槽颜色
    void drawGroove(QVector<QPushButton*> QVpushbutton,QLabel* labelHint, QString signal);

    //竞争槽数字
    void setGroove(QVector<QPushButton*> QVpushbutton, int i,QString signal);

    //labelHint2
    void setHint2(QVector<QPushButton*> QVpushbutton,QLabel* labelHint2);

    //textBrowser
    void setTextBrowser(QTextBrowser *textBrowser,int i,QList<QString> *channel,int lasti);

private:
    Ui::Anime *ui;
};

#endif // ANIME_H
