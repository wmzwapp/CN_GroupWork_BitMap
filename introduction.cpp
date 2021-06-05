#include "introduction.h"
#include "ui_introduction.h"

Introduction::Introduction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Introduction)
{
    ui->setupUi(this);
    resize(500,700);
    setWindowTitle("位图协议之文字介绍");
}

Introduction::~Introduction()
{
    delete ui;
}
