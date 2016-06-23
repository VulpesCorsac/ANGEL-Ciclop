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

    this->monochromator = nullptr;
    this->monochromator = new Monochromator();

    ui->pushButtonMonochromatorComAction->setText("Connect");

    ui->comboBoxMonochromatorType->addItem("MDG_23_by_Geo");

    ui->groupBoxMonochromatorSettingsAndOutput->hide();

    this->constructorActive = false;
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

                // ACTUAL CONNECTION

                connect(this->monochromator, SIGNAL(Monochromator_currentDisplayNumberChanged(double)), this, SLOT(getCurrentDisplayNumberChanged(double)), Qt::DirectConnection);

                ui->groupBoxMonochromatorSettingsAndOutput->show();

                ui->pushButtonMonochromatorComAction->setText("Disconnect");

                ui->comboBoxMonochromatorCom->setEnabled(false);
                ui->comboBoxMonochromatorType->setEnabled(false);

                ui->comboBoxMonochromatorGrating->addItem("600");
                ui->comboBoxMonochromatorGrating->addItem("1200");
                ui->comboBoxMonochromatorGrating->setCurrentText("1200");

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

                ui->comboBoxMonochromatorWavelengthMode->addItem("Single");
                ui->comboBoxMonochromatorWavelengthMode->addItem("Step");
                ui->comboBoxMonochromatorWavelengthMode->setCurrentText("Single");

                ui->tabWidgetMonochromatorSettingsAndOutput->setCurrentIndex(0);

                return;
            } else
                return;
        } else {
            // disconnect

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
