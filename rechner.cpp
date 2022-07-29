#include "rechner.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <calculate.h>
bool is_2nd = false;
bool is_mem = false;
bool is_sto = false;
QVector<QString> deleteTokens;
QString displayString = "";
QString trigMode = "deg";
QString mem1 = "";
QString mem2 = "";
QString mem3 = "";
Rechner::Rechner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Rechner)
{
    ui->setupUi(this);
    QList<QPushButton *> buttons = findChildren<QPushButton *>();
    for(auto *button : buttons){
        auto name = button->objectName();
        connect(button, &QPushButton::pressed, this, &Rechner::buttonPressed);
    }
}

Rechner::~Rechner()
{
    delete ui;
}
void drg(){
    if(trigMode == "deg"){
        trigMode = "rad";
    }
    else if(trigMode == "rad"){
        trigMode = "gra";
    }
    else if (trigMode == "gra"){
        trigMode = "deg";
    }
    // Inform calculation program
    calculation::trigMode = trigMode.toStdString();
}


double calculate(){
    std::wstring wstringCalc = displayString.toStdWString();
    double result = calculation::calculate(wstringCalc);
    return result;
}

void Rechner::renderCalculator(){
    if(displayString == "nan"){
        displayString = "ERROR";
        deleteTokens = QVector<QString>{"ERROR"};
    }
    ui->display->setText(displayString.size() != 0?displayString:"0");
    QString visibility = is_2nd?"color:black":"color:transparent";
    ui->show_2nd->setStyleSheet(visibility+";background-color:transparent;");
    if(trigMode != "deg"){
        ui->show_trigmode->setText(trigMode);
        ui->show_trigmode->setStyleSheet("color:black;background-color:transparent;");
    }
    else{
        ui->show_trigmode->setStyleSheet("color:transparent;background-color:transparent;");
    }
    if(is_mem||is_sto){
        ui->show_mem->setStyleSheet("color:black;background-color:transparent;");
    }
    else{
        ui->show_mem->setStyleSheet("color:transparent;background-color:transparent;");
    }
    if(mem1.size() != 0){
        ui->mem1_indicator->setStyleSheet("color:black;background-color:transparent;");
    }
    else{
        ui->mem1_indicator->setStyleSheet("color:transparent;background-color:transparent;");
    }
    if(mem2.size() != 0){
        ui->mem2_indicator->setStyleSheet("color:black;background-color:transparent;");
    }
    else{
        ui->mem2_indicator->setStyleSheet("color:transparent;background-color:transparent;");
    }
    if(mem3.size() != 0){
        ui->mem3_indicator->setStyleSheet("color:black;background-color:transparent;");
    }
    else{
        ui->mem3_indicator->setStyleSheet("color:transparent;background-color:transparent;");
    }
}
QString replaceScientific(QString input){
    return input.replace("e","E");
}
void displayToTokens(QString input){
    QVector<QString> splitResult;
    for(auto character : input){
        splitResult.push_back(character);
    }
    deleteTokens = splitResult;
}
void Rechner::handleClick(QString action){
    if(is_mem){
        is_mem = false;
        if(!QString("123").contains(action) ){
            renderCalculator();
            return;
        }
        QString mem = action=="1"?mem1:(action=="2"?mem2:mem3);
        displayString += mem;

        for(auto character : mem){
            deleteTokens.push_back(character);
        }
        renderCalculator();
        return;
    }
    if(is_sto){
        is_sto = false;
        if(!QString("123").contains(action) ){
            renderCalculator();
            return;
        }
        double result = calculate();
        if(action=="1"){
            mem1 = QString::number(result);
        }
        if(action=="2"){
            mem2 = QString::number(result);
        }
        if(action=="3"){
            mem3 = QString::number(result);
        }
        renderCalculator();
        return;

    }
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
        else if(action == "pi"){
            displayString += "e";
            deleteTokens.push_back("e");
        }
        else if(action=="clear"){
            qApp->exit();
        }
        else if(action=="log"){
            double result = pow(10, calculate());
            displayString = replaceScientific(QString::number(result));

            displayToTokens(displayString);
        }
        else if(action=="ln"){
            double result = pow(2.718,calculate());

            displayString = replaceScientific(QString::number(result));

            displayToTokens(displayString);
        }
        else if(action=="0"){

            displayString += "[y√x]3";
            deleteTokens.push_back("[y√x]");
            deleteTokens.push_back("3");
        }
        else if(action=="ypowerx"){

            displayString += "[y√x]";
            deleteTokens.push_back("[y√x]");


        }

        else if(action=="cos"){
            displayString += "[ACOS]";
            deleteTokens.push_back("[ACOS]");
        }
        else if(action=="sin"){
            displayString += "[ASIN]";
            deleteTokens.push_back("[ASIN]");
        }
        else if(action=="tan"){
            displayString += "[ATAN]";
            deleteTokens.push_back("[ATAN]");
        }
        else if(action=="ee"){
            displayString += ",";
            deleteTokens.push_back(",");
        }
        else if(action=="sigmaplus"){
            double result = calculate();
            displayString = replaceScientific(QString::number(result-1));
            displayToTokens(displayString);
        }
        else if(action=="2"){
            double result = calculate();
            displayString = replaceScientific(QString::number(result/100));
            displayToTokens(displayString);
        }
        else if(action=="1"){
            displayString += "^3";
            deleteTokens.push_back("^3");
        }
        else if(action=="8"){
            displayString += "[nCr]";
            deleteTokens.push_back("[nCr]");
        }
        else if(action=="9"){
            displayString += "[nPr]";
            deleteTokens.push_back("[nPr]");
        }
        else if(action=="drg"){
            displayString += "[LOG_2]";
            deleteTokens.push_back("[LOG_2]");
        }
        else{
            handleClick(action);
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
        if(action=="divide"||action=="aboverc"){
            displayString += "÷";
            deleteTokens.push_back("÷");
        }
        if(action=="pi"){
            displayString += "π";
            deleteTokens.push_back("π");
        }
        if(action=="clear"||action=="on"){
            trigMode = "deg";
            displayString = "";
            deleteTokens = QVector<QString>{};
            if(action=="on"){
                mem1 = "";
                mem2 = "";
                mem3 = "";
            }
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
            displayString = replaceScientific(QString::number(result));

            displayToTokens(displayString);

        }
        if(action=="sigmaplus"){
            double result = calculate();
            displayString = replaceScientific(QString::number(result+1));
            displayToTokens(displayString);
        }
        if(action == "toggle"){
            double result = calculate() * -1;
            displayString = replaceScientific(QString::number(result));

            displayToTokens(displayString);

        }
        if(action == "inverse"){
            double result = 1/calculate();
            displayString = replaceScientific(QString::number(result));

            displayToTokens(displayString);

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
        if(action=="log"){
            displayString += "[LOG]";
            deleteTokens.push_back("[LOG]");
        }
        if(action=="ln"){
            displayString += "[LN]";
            deleteTokens.push_back("[LN]");
        }
        if(action =="drg"){
            drg();
        }
        if(action == "ee"){
            displayString += "E";
            deleteTokens.push_back("E");
        }
        if(action=="rcl"){
            is_mem = !is_mem;
        }
        if(action=="sto"){
            is_sto = !is_mem;
        }
        if(action=="mod"){
            displayString += "%";
            deleteTokens.push_back("%");
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

