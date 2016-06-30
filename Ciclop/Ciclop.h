#ifndef CICLOP_H
#define CICLOP_H

#include <QMainWindow>

#include <QTabBar>
#include <QTableWidget>

#include <QStyle>
#include <QStyleFactory>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QThread>

#include <QDateTime>
#include <QTime>
#include <QTimer>

#include <QTest>

#include <QDir>

#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "../ANGEL/Angel.h"

namespace Ui {
    class Ciclop;
}

class Ciclop : public QMainWindow
{
    Q_OBJECT

private:
    bool constructorActive = false;

    const int maxConnectAttempts = 10;

    Monochromator* monochromator;

public:
    explicit Ciclop(QWidget *parent = 0);
    explicit Ciclop(Monochromator* new_monochromator);
    ~Ciclop();

private slots:
    void on_pushButtonMonochromatorComAction_clicked();
    void on_pushButtonMonochromatorCurrentDisplayNumberAction_clicked();
    void on_comboBoxMonochromatorWavelengthMode_currentTextChanged(const QString &arg1);
    void getCurrentDisplayNumberChanged(const double &newDisplayNumber);
    void on_comboBoxMonochtomatorStepDenominator_currentTextChanged(const QString &arg1);
    void on_comboBoxMonochromatorGrating_currentTextChanged(const QString &arg1);
    void on_pushButtonMonochromatorMoveCoarseLess_clicked();
    void on_pushButtonMonochromatorMoveFineLess_clicked();
    void on_pushButtonMonochromatorMoveFineMore_clicked();
    void on_pushButtonMonochromatorMoveCoarseMore_clicked();
    void on_pushButtonMonochromatorGoto_clicked();

private:
    Ui::Ciclop *ui;
};

#endif // CICLOP_H
