#include "rechner.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>
#include <calculate.h>
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
double calculate(){
    std::wstring wstringCalc = displayString.toStdWString();
    double result = calculation::calculate(wstringCalc);
    return result;
}

void Rechner::renderCalculator(){
    ui->display->setText(displayString.size() != 0?displayString:"0");
    QString visibility = is_2nd?"color:black":"color:transparent";
    ui->show_2nd->setStyleSheet(visibility+";background-color:transparent;");
}
void Rechner::handleClick(QString action){
    if(action=="2nd"){
        is_2nd = !is_2nd;
        renderCalculator();
        return;
    }
    if(is_2nd){
        is_2nd = false;
        if(action == "3"){
        displayString += "!";
        deleteTokens.push_back("!");
        }
    }
    else{
        if(action == "0"||action=="1"||action=="2"||action=="3"||action=="4"||action=="5"||action=="6"||action=="7"||action=="8"||action=="9"){
            displayString += action;
            deleteTokens.push_back(action);
        }
        if(action=="plus"){
            displayString += "+";
            deleteTokens.push_back("+");
        }
        if(action=="minus"){
            displayString += "-";
            deleteTokens.push_back("-");
        }
        if(action=="multiply"){
            displayString += "×";
            deleteTokens.push_back("×");
        }
        if(action=="divide"){
            displayString += "÷";
            deleteTokens.push_back("÷");
        }
        if(action=="pi"){
            displayString += "π";
            deleteTokens.push_back("π");
        }
        if(action=="clear"||action=="on"){
            displayString = "";
            deleteTokens = QVector<QString>{};

        }
        if(action=="period"){
            displayString += ".";
            deleteTokens.push_back(".");

        }
        if(action=="squarex"){
            displayString += "^2";
            deleteTokens.push_back("^2");
        }
        if(action=="rootx"){
            displayString += "√";
            deleteTokens.push_back("√");
        }

        if(action=="paropen"){
            displayString += "(";
            deleteTokens.push_back("(");
        }
        if(action=="parclose"){
            displayString += ")";
            deleteTokens.push_back(")");
        }

        if(action=="delete"){

            if(deleteTokens.size() == 0){
                return;
            }
            deleteTokens.pop_back();
            QString joined;
            for(QString str : deleteTokens){
                joined += str;
            }
            displayString = joined;
        }
        if(action == "equals"){
            double result = calculate();
            displayString = QString::number(result);

            QVector<QString> splitResult;
            for(auto character : displayString){
                splitResult.push_back(character);
            }
            deleteTokens = splitResult;
            if(displayString == "nan"){
                displayString = "ERROR";
                deleteTokens = QVector<QString>{"ERROR"};
            }
        }
        if(action=="ypowerx"){
            displayString += "^";
            deleteTokens.push_back("^");

        }
        if(action=="cos"){
            displayString += "[COS]";
            deleteTokens.push_back("[COS]");
        }
        if(action=="sin"){
            displayString += "[SIN]";
            deleteTokens.push_back("[SIN]");
        }
        if(action=="tan"){
            displayString += "[TAN]";
            deleteTokens.push_back("[TAN]");
        }
    }

    renderCalculator();
    return;
}

void Rechner::buttonPressed(){
    QObject* senderObj = sender();
    QString name = senderObj->objectName();
    handleClick(name.remove(0,7));
}

