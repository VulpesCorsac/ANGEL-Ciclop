#include "Ciclop.h"
#include "ui_Ciclop.h"

Ciclop::Ciclop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ciclop)
{
    this->constructorActive = true;

    ui->setupUi(this);

//    this->setAttribute(Qt::WA_DeleteOnClose, true);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comboBoxMonochromatorCom->addItem(info.portName());
    ui->comboBoxMonochromatorCom->setCurrentText("COM11");


    this->monochromator = nullptr;
    this->monochromator = new Monochromator();

    ui->pushButtonMonochromatorComAction->setText("Connect");

    ui->comboBoxMonochromatorType->addItem("MDG_23_by_Geo");

    ui->groupBoxMonochromatorSettingsAndOutput->hide();

    this->constructorActive = false;
}

Ciclop::Ciclop(Monochromator *new_monochromator)
{
    this->monochromator = new_monochromator;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comboBoxMonochromatorCom->addItem(info.portName());

    ui->comboBoxMonochromatorType->addItem("MDG_23_by_Geo");

    if (this->monochromator->isActive()) {
        connect(this->monochromator, SIGNAL(Monochromator_currentDisplayNumberChanged(double)), this, SLOT(getCurrentDisplayNumberChanged(double)), Qt::DirectConnection);

        ui->groupBoxMonochromatorSettingsAndOutput->show();

        ui->pushButtonMonochromatorComAction->setText("Disconnect");

        ui->comboBoxMonochromatorCom->setEnabled(false);
        ui->comboBoxMonochromatorType->setEnabled(false);

        ui->comboBoxMonochromatorGrating->clear();
        ui->comboBoxMonochromatorGrating->addItem("600");
        ui->comboBoxMonochromatorGrating->addItem("1200");
        ui->comboBoxMonochromatorGrating->setCurrentText(QString::number(this->monochromator->getGrating()));

        ui->comboBoxMonochtomatorStepDenominator->clear();
        ui->comboBoxMonochtomatorStepDenominator->addItem("1/1");
        ui->comboBoxMonochtomatorStepDenominator->addItem("1/2");
        ui->comboBoxMonochtomatorStepDenominator->addItem("1/4");
        ui->comboBoxMonochtomatorStepDenominator->addItem("1/16");
        ui->comboBoxMonochtomatorStepDenominator->setCurrentText(QString::number(this->monochromator->getStepDenominator()));

        ui->pushButtonMonochromatorCurrentDisplayNumberAction->setText("Lock");
        ui->pushButtonMonochromatorCurrentDisplayNumberAction->setEnabled(true);

        ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(this->monochromator->getCurrentDisplayNumber());

        ui->doubleSpinBoxMonochromatorGoto->setEnabled(false);
        ui->pushButtonMonochromatorGoto->setEnabled(false);

        ui->checkBoxMonochromatorMoveExtraFine->show();
        ui->pushButtonMonochromatorMoveCoarseLess->show();
        ui->pushButtonMonochromatorMoveFineLess->show();
        ui->pushButtonMonochromatorMoveCoarseMore->show();
        ui->pushButtonMonochromatorMoveFineMore->show();

        ui->groupBoxMonochromatorWavelength->hide();

        ui->comboBoxMonochromatorWavelengthMode->clear();
        ui->comboBoxMonochromatorWavelengthMode->addItem("Single");
        ui->comboBoxMonochromatorWavelengthMode->addItem("Step");
        ui->comboBoxMonochromatorWavelengthMode->setCurrentText("Single");

        ui->tabWidgetMonochromatorSettingsAndOutput->setCurrentIndex(0);

        ui->pushButtonMonochromatorComAction->setText("Disonnect");

        ui->groupBoxMonochromatorSettingsAndOutput->show();

        connect(this->monochromator, SIGNAL(Monochromator_currentDisplayNumberChanged(double)), this, SLOT(getCurrentDisplayNumberChanged(double)), Qt::DirectConnection);
    } else {
        ui->pushButtonMonochromatorComAction->setText("Connect");

        ui->groupBoxMonochromatorSettingsAndOutput->hide();
    }
}

Ciclop::~Ciclop()
{
    this->monochromator->~Monochromator();

    delete ui;
}

void Ciclop::on_pushButtonMonochromatorComAction_clicked()
{
    for (int attempt = 0; attempt < this->maxConnectAttempts; attempt++) {
        if (ui->pushButtonMonochromatorComAction->text() == "Connect") {
            if (ui->comboBoxMonochromatorCom->currentText().isEmpty())
                return;
            if (ui->comboBoxMonochromatorType->currentText() == "MDG_23_by_Geo") {
                MDG_23_by_Geo* MDG23ByGeo = this->monochromator->getMDG23ByGeoMonochromator();
                if (MDG23ByGeo == nullptr) {
                    MDG23ByGeo = new MDG_23_by_Geo();
                    this->monochromator->setMDG23ByGeoMonochromator(MDG23ByGeo);
                }

                SMSD_4_2* stepMotor = MDG23ByGeo->getStepMotorController();
                if (stepMotor == nullptr) {
                    stepMotor = new SMSD_4_2();
                    MDG23ByGeo->setStepMotorController(stepMotor);
                }

                if (!this->monochromator->setMDG23ByGeoConnection(ui->comboBoxMonochromatorCom->currentText()))
                    return;

                if (!this->monochromator->isActive())
                    return;

                connect(this->monochromator, SIGNAL(Monochromator_currentDisplayNumberChanged(double)), this, SLOT(getCurrentDisplayNumberChanged(double)), Qt::DirectConnection);

                ui->groupBoxMonochromatorSettingsAndOutput->show();

                ui->pushButtonMonochromatorComAction->setText("Disconnect");

                ui->comboBoxMonochromatorCom->setEnabled(false);
                ui->comboBoxMonochromatorType->setEnabled(false);

                ui->comboBoxMonochromatorGrating->clear();
                ui->comboBoxMonochromatorGrating->addItem("600");
                ui->comboBoxMonochromatorGrating->addItem("1200");
                ui->comboBoxMonochromatorGrating->setCurrentText("1200");

                ui->comboBoxMonochtomatorStepDenominator->clear();
                ui->comboBoxMonochtomatorStepDenominator->addItem("1/1");
                ui->comboBoxMonochtomatorStepDenominator->addItem("1/2");
                ui->comboBoxMonochtomatorStepDenominator->addItem("1/4");
                ui->comboBoxMonochtomatorStepDenominator->addItem("1/16");
                ui->comboBoxMonochtomatorStepDenominator->setCurrentText("1/16");

                ui->pushButtonMonochromatorCurrentDisplayNumberAction->setText("Lock");
                ui->pushButtonMonochromatorCurrentDisplayNumberAction->setEnabled(true);

                ui->doubleSpinBoxMonochromatorGoto->setEnabled(false);
                ui->pushButtonMonochromatorGoto->setEnabled(false);

                ui->checkBoxMonochromatorMoveExtraFine->show();
                ui->pushButtonMonochromatorMoveCoarseLess->show();
                ui->pushButtonMonochromatorMoveFineLess->show();
                ui->pushButtonMonochromatorMoveCoarseMore->show();
                ui->pushButtonMonochromatorMoveFineMore->show();

                ui->groupBoxMonochromatorWavelength->hide();

                ui->comboBoxMonochromatorWavelengthMode->clear();
                ui->comboBoxMonochromatorWavelengthMode->addItem("Single");
                ui->comboBoxMonochromatorWavelengthMode->addItem("Step");
                ui->comboBoxMonochromatorWavelengthMode->setCurrentText("Single");

                ui->tabWidgetMonochromatorSettingsAndOutput->setCurrentIndex(0);

                connect(this->monochromator, SIGNAL(Monochromator_currentDisplayNumberChanged(double)), this, SLOT(getCurrentDisplayNumberChanged(double)), Qt::DirectConnection);

                return;
            } else
                return;
        } else {
            this->monochromator->disconnectMDG23ByGeo();

            ui->comboBoxMonochromatorCom->setEnabled(true);
            ui->comboBoxMonochromatorType->setEnabled(true);

            ui->pushButtonMonochromatorComAction->setText("Connect");
            ui->groupBoxMonochromatorSettingsAndOutput->hide();

            return;
        }
    }

    return;
}

void Ciclop::on_pushButtonMonochromatorCurrentDisplayNumberAction_clicked()
{
    if (ui->pushButtonMonochromatorCurrentDisplayNumberAction->text() == "Lock") {
        ui->pushButtonMonochromatorCurrentDisplayNumberAction->setText("Unlock");

        ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setEnabled(false);

        ui->checkBoxMonochromatorMoveExtraFine->hide();
        ui->pushButtonMonochromatorMoveCoarseLess->hide();
        ui->pushButtonMonochromatorMoveFineLess->hide();
        ui->pushButtonMonochromatorMoveCoarseMore->hide();
        ui->pushButtonMonochromatorMoveFineMore->hide();

        ui->doubleSpinBoxMonochromatorGoto->setEnabled(true);
        ui->pushButtonMonochromatorGoto->setEnabled(true);

        ui->groupBoxMonochromatorWavelength->show();

        this->monochromator->setCurrentDisplayNumber(ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->value());
    } else {
        ui->pushButtonMonochromatorCurrentDisplayNumberAction->setText("Lock");

        ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setEnabled(true);

        ui->checkBoxMonochromatorMoveExtraFine->show();
        ui->pushButtonMonochromatorMoveCoarseLess->show();
        ui->pushButtonMonochromatorMoveFineLess->show();
        ui->pushButtonMonochromatorMoveCoarseMore->show();
        ui->pushButtonMonochromatorMoveFineMore->show();

        ui->doubleSpinBoxMonochromatorGoto->setEnabled(false);
        ui->pushButtonMonochromatorGoto->setEnabled(false);

        ui->groupBoxMonochromatorWavelength->hide();
    }

    return;
}

void Ciclop::on_comboBoxMonochromatorWavelengthMode_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Single") {
        ui->doubleSpinBoxMonochromatorWavelengthTo->setValue(ui->doubleSpinBoxMonochromatorWavelengthFrom->value());
        ui->doubleSpinBoxMonochromatorWavelengthTo->setEnabled(false);

        ui->doubleSpinBoxMonochromatorWavelengthStep->setValue(0.00000);
        ui->doubleSpinBoxMonochromatorWavelengthStep->setEnabled(false);

        return;
    } else {
        ui->doubleSpinBoxMonochromatorWavelengthTo->setValue(ui->doubleSpinBoxMonochromatorWavelengthFrom->value());
        ui->doubleSpinBoxMonochromatorWavelengthTo->setEnabled(true);

        ui->doubleSpinBoxMonochromatorWavelengthStep->setValue(this->monochromator->getDisplaySteps());
        ui->doubleSpinBoxMonochromatorWavelengthStep->setEnabled(true);

        return;
    }

    return;
}

void Ciclop::getCurrentDisplayNumberChanged(const double &new_displaynumber)
{
    ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(new_displaynumber);
    ui->doubleSpinBoxMonochromatorWavelengthCurrentWavelength->setValue(this->monochromator->displayNumberToWavelength(new_displaynumber));

    return;
}

void Ciclop::on_comboBoxMonochtomatorStepDenominator_currentTextChanged(const QString &arg1)
{
    if (arg1 == "1/1")
        this->monochromator->setStepDenominator(1);
    if (arg1 == "1/2")
        this->monochromator->setStepDenominator(2);
    if (arg1 == "1/4")
        this->monochromator->setStepDenominator(4);
    if (arg1 == "1/16")
        this->monochromator->setStepDenominator(16);

    return;
}

void Ciclop::on_comboBoxMonochromatorGrating_currentTextChanged(const QString &arg1)
{
    if (arg1 == "600")
        this->monochromator->setGrating(600);
    if (arg1 == "1200")
        this->monochromator->setGrating(1200);

    return;
}

void Ciclop::on_pushButtonMonochromatorMoveCoarseLess_clicked()
{
    int steps = !ui->checkBoxMonochromatorMoveExtraFine->isChecked() ? this->monochromator->getCoarseStep() :
                                                                       this->monochromator->getExtraFineStep();
    this->monochromator->move(this->monochromator->isRightRotationBySign(-1), steps);
    ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(this->monochromator->getCurrentDisplayNumber());

    return;
}

void Ciclop::on_pushButtonMonochromatorMoveFineLess_clicked()
{
    int steps = !ui->checkBoxMonochromatorMoveExtraFine->isChecked() ? this->monochromator->getFineStep() :
                                                                       this->monochromator->getUltraFineStep();
    this->monochromator->move(this->monochromator->isRightRotationBySign(-1), steps);
    ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(this->monochromator->getCurrentDisplayNumber());

    return;

}

void Ciclop::on_pushButtonMonochromatorMoveFineMore_clicked()
{
    int steps = !ui->checkBoxMonochromatorMoveExtraFine->isChecked() ? this->monochromator->getFineStep() :
                                                                       this->monochromator->getUltraFineStep();
    this->monochromator->move(this->monochromator->isRightRotationBySign(1), steps);
    ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(this->monochromator->getCurrentDisplayNumber());

    return;
}

void Ciclop::on_pushButtonMonochromatorMoveCoarseMore_clicked()
{
    int steps = !ui->checkBoxMonochromatorMoveExtraFine->isChecked() ? this->monochromator->getCoarseStep() :
                                                                       this->monochromator->getExtraFineStep();
    this->monochromator->move(this->monochromator->isRightRotationBySign(1), steps);
    ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(this->monochromator->getCurrentDisplayNumber());

    return;
}

void Ciclop::on_pushButtonMonochromatorGoto_clicked()
{
    this->monochromator->move(ui->doubleSpinBoxMonochromatorGoto->value());
    ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->setValue(this->monochromator->getCurrentDisplayNumber());
    ui->doubleSpinBoxMonochromatorWavelengthCurrentWavelength->setValue(ui->doubleSpinBoxMonochromatorCurrentDisplayNumber->value());

    return;
}

