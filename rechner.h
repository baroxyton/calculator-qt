#ifndef RECHNER_H
#define RECHNER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Rechner; }
QT_END_NAMESPACE

class Rechner : public QMainWindow
{
    Q_OBJECT

public:
    Rechner(QWidget *parent = nullptr);
    void handleClick(QString);
    void renderCalculator();
    ~Rechner();

private:
    Ui::Rechner *ui;

private slots:
    void buttonPressed();
};
#endif // RECHNER_H
