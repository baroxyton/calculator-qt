#include "rechner.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>
bool is_2nd = false;
QVector<QString> deleteTokens;
QString displayString = "";
Rechner::Rechner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Rechner)
{
    ui->setupUi(this);
    QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for(auto *button : buttons){
        auto name = button->objectName();
        qDebug() << name;
        connect(button, &QPushButton::pressed, this, &Rechner::buttonPressed);
    }
}

Rechner::~Rechner()
{
    delete ui;
}
void Rechner::handleClick(QString action){
    qDebug() << is_2nd;
    ui->display->setText(action);
    if(action=="2nd"){
        is_2nd = !is_2nd;
        return;
    }
    if(is_2nd){
    is_2nd = false;
    }
    else{
       if(action == "0"||action=="1"||action=="2"){


       }
    }
}

void Rechner::buttonPressed(){
QObject* senderObj = sender();
QString name = senderObj->objectName();
handleClick(name.remove(0,7));
}

