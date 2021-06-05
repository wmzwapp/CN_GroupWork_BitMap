#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "controlstation.h"
#include "mystation.h"
#include "anime.h"
#include "introduction.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("位图协议之输入参数");
    resize(600,400);


    connect(ui->pbAnime,&QPushButton::clicked,[=](){
//        close();
        Anime *anime = new Anime(ui->sbNum->value(),ui->sbTime->value(),ui->sbFre->value(),ui->sbLength->value());
        anime->resize(800,600);
    });
    connect(ui->pbText,&QPushButton::clicked,[=](){
        Introduction* introduction = new Introduction();
        introduction->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

