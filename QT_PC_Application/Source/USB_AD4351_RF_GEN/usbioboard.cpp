#include "usbioboard.h"
#include "ui_usbio.h"
#include "ad4351.h"

USBIOBoard::USBIOBoard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::USB_AD4351_form)
{
    ui->setupUi(this);

    plugNPlay = new HID_PnP();
    ad4351 = new AD4351();
    this->enable_auto_tx = false;
   // connect(ui->spinBox_pwm_freq,SIGNAL(valueChanged(int)),this,SLOT(update_pwm()));

    connect(this, SIGNAL(singal_recalculate()), ad4351, SLOT(BuildRegisters()));

    connect(this, SIGNAL(signal_update_reg(const uint32_t *)), plugNPlay, SLOT(change_reg(const uint32_t *)));
    connect(this, SIGNAL(signal_update_RF_CTRL()), plugNPlay, SLOT(change_RF_CTRL()));
    connect(this, SIGNAL(signal_auto_tx()), this, SLOT(update_reg()));
    connect(ui->USBTX, SIGNAL(clicked(bool)), this, SLOT(update_reg()));
    //connect(ui->groupBox_main, SIGNAL(clicked(bool)), this, SLOT(recalculate()));

    connect(ui->RF_CTRL, SIGNAL(clicked(bool)), this, SLOT(update_RF_CTRL()));

    connect(plugNPlay, SIGNAL(hid_comm_update(bool, UI_Data*)), this, SLOT(update_gui(bool, UI_Data*)));
    connect(ad4351, SIGNAL(reg_update_result()), this, SLOT(display_reg()));
    connect(ui->pushButton_sweep_start, SIGNAL(clicked(bool)), this, SLOT(sweep_start_click()));
    connect(ui->pushButton_sweep_stop, SIGNAL(clicked(bool)), this, SLOT(sweep_stop_click()));
    connect(ui->pushButton_hop_start, SIGNAL(clicked(bool)), this, SLOT(hop_start_click()));
    connect(ui->pushButton_hop_stop, SIGNAL(clicked(bool)), this, SLOT(hop_stop_click()));
    connect(ui->checkBox_autotx, SIGNAL(clicked(bool)), this, SLOT(autotx_clicked()));

    connect(ui->doubleSpinBox_freq, SIGNAL(valueChanged(double)), this, SLOT(recalculate()));

    connect(ui->groupBox_main, SIGNAL(clicked(bool)), this, SLOT(recalculate()));
    connect(ui->comboBox_ABP, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_mode, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_mux_out, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_MTLD, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_band_select_clk_mode, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_AUX_EN, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_AUX_OUT, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_AUX_out_power, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_phase_adjust, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_double_buff, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_charge_cancellation, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_charge_pump_current, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_counter_rst, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_cp_3_state, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_CLK_div_mode, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_CSR, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_feedback, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_LDF, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_LDP, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_LDPIN, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_powerdown, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_presacler, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_PD_polarity, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_vco_powerdown, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_RF_OUT, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->comboBox_RF_POWER, SIGNAL(currentIndexChanged(int)), this , SLOT(recalculate()));
    connect(ui->spinBox_clock_divider, SIGNAL(valueChanged(int)), this, SLOT(recalculate()));
    connect(ui->spinBox_rcount, SIGNAL(valueChanged(int)), this, SLOT(recalculate()));
    connect(ui->spinBox_phase_val, SIGNAL(valueChanged(int)), this, SLOT(recalculate()));
    connect(ui->checkBox_refdiv2, SIGNAL(clicked(bool)), this, SLOT(recalculate()));
    connect(ui->checkBox_refx2, SIGNAL(clicked(bool)), this, SLOT(recalculate()));
    connect(ui->lineEdit_ref, SIGNAL(textChanged(QString)), this, SLOT(recalculate()));

    sweep_timer = new QTimer();
    hop_timer = new QTimer();


    connect(hop_timer, SIGNAL(timeout()), this, SLOT(hop_timer_timeout()));
    connect(sweep_timer, SIGNAL(timeout()), this, SLOT(sweep_timer_timeout()));
    this->recalculate();

}

void USBIOBoard::showEvent( QShowEvent *event )
{
// call whatever your base class is!

    this->recalculate();
}

USBIOBoard::~USBIOBoard()
{
 //   disconnect(this, SIGNAL(tris_box_state_changed()),plugNPlay,SLOT(toggle_io()));

    disconnect(plugNPlay, SIGNAL(hid_comm_update(bool,UI_Data)), this, SLOT(update_gui(bool, UI_Data)));
    delete ui;
    delete plugNPlay;
}

void USBIOBoard::update_dac()
{
   // emit signal_update_dac(ui->spinBox_dac->value());
}
void USBIOBoard::getDataFromUI()
{
    this->ad4351->frequency = ui->doubleSpinBox_freq->text().toDouble();
    this->ad4351->REF_FREQ = ui->lineEdit_ref->text().toInt();
    this->ad4351->r_counter = ui->spinBox_rcount->text().toInt();
    this->ad4351->PHASE = ui->spinBox_phase_val->text().toInt();
    this->ad4351->PHASE_ADJUST = ui->comboBox_phase_adjust->currentIndex();
    this->ad4351->ref_div2 = ui->checkBox_refdiv2->isChecked();
    this->ad4351->ref_doubler = ui->checkBox_refx2->isChecked();
    this->ad4351->low_noise_spur_mode = ui->comboBox_mode->currentIndex();
    this->ad4351->muxout = ui->comboBox_mux_out->currentIndex();
    this->ad4351->double_buff = ui->comboBox_double_buff->currentIndex();
    this->ad4351->charge_pump_current = ui->comboBox_charge_pump_current->currentIndex();
    this->ad4351->LDF = ui->comboBox_LDF->currentIndex();
    this->ad4351->LDP = ui->comboBox_LDP->currentIndex();
    this->ad4351->PD_Polarity = ui->comboBox_PD_polarity->currentIndex();
    this->ad4351->cp_3stage = ui->comboBox_cp_3_state->currentIndex();
    this->ad4351->counter_reset = ui->comboBox_counter_rst->currentIndex();
    this->ad4351->band_select_clock_mode = ui->comboBox_band_select_clk_mode->currentIndex();
    this->ad4351->charge_cancelletion = ui->comboBox_charge_cancellation->currentIndex();
    this->ad4351->ABP = ui->comboBox_ABP->currentIndex();
    this->ad4351->CSR = ui->comboBox_CSR->currentIndex();
    this->ad4351->clock_divider = ui->spinBox_clock_divider->value();
    this->ad4351->CLK_DIV_MODE = ui->comboBox_CLK_div_mode->currentIndex();
    this->ad4351->LD = ui->comboBox_LDPIN->currentIndex();
    this->ad4351->power_down = ui->comboBox_powerdown->currentIndex();
    this->ad4351->VCO_power_down = ui->comboBox_vco_powerdown->currentIndex();
    this->ad4351->MTLD = ui->comboBox_MTLD->currentIndex();
    this->ad4351->AUX_output_mode = ui->comboBox_AUX_OUT->currentIndex();
    this->ad4351->AUX_output_enable = ui->comboBox_AUX_EN->currentIndex();
    this->ad4351->AUX_output_power = ui->comboBox_AUX_out_power->currentIndex();
    this->ad4351->RF_output_power = ui->comboBox_RF_POWER->currentIndex();
    this->ad4351->RF_ENABLE = ui->comboBox_RF_OUT->currentIndex();
    this->ad4351->PR1 = ui->comboBox_presacler->currentIndex();
    this->ad4351->feedback_select = ui->comboBox_feedback->currentIndex();
}
void USBIOBoard::recalculate()
{
   this->getDataFromUI();
   emit singal_recalculate();
}

void USBIOBoard::update_reg()
{

    bool bStatus = false;
    const uint32_t hex_values[] = {
        ui->line_reg0->text().toUInt(&bStatus, 16),
        ui->line_reg1->text().toUInt(&bStatus, 16),
        ui->line_reg2->text().toUInt(&bStatus, 16),
        ui->line_reg3->text().toUInt(&bStatus, 16),
        ui->line_reg4->text().toUInt(&bStatus, 16),
        ui->line_reg5->text().toUInt(&bStatus, 16),
    };


    emit signal_update_reg(hex_values);
}

void USBIOBoard::update_RF_CTRL()
{
    emit signal_update_RF_CTRL();
}

void USBIOBoard::sweep_stop_click()
{
    this->enable_auto_tx = ui->checkBox_autotx->isChecked();
    sweep_timer->stop();
}

void USBIOBoard::sweep_start_click()
{
    this->enable_auto_tx = true;
    ui->doubleSpinBox_sweep_freq->setValue(ui->doubleSpinBox_sweep_start->value());
    sweep_timer->start(ui->spinBox_sweep_delay->value());
}


void USBIOBoard::autotx_clicked()
{
    this->enable_auto_tx = ui->checkBox_autotx->isChecked();
    ui->USBTX->setEnabled(!this->enable_auto_tx);
}

void USBIOBoard::hop_stop_click()
{
    this->enable_auto_tx = ui->checkBox_autotx->isChecked();
    hop_timer->stop();
}

void USBIOBoard::hop_start_click()
{
    this->enable_auto_tx = true;
    ui->doubleSpinBox_sweep_freq->setValue(ui->doubleSpinBox_hop_freq_a->value());
    hop_timer->start(ui->spinBox_hop_delay->value());
}



void USBIOBoard::hop_timer_timeout()
{
    if (ui->doubleSpinBox_hop_freq->value() == ui->doubleSpinBox_hop_freq_a->value())
    {
        ui->doubleSpinBox_hop_freq->setValue(ui->doubleSpinBox_hop_freq_b->value());
    }
    else
    {
        ui->doubleSpinBox_hop_freq->setValue(ui->doubleSpinBox_hop_freq_a->value());
    }
    this->ad4351->frequency = ui->doubleSpinBox_hop_freq->value();
    emit singal_recalculate();
}


void USBIOBoard::sweep_timer_timeout()
{
    const double frequency = ui->doubleSpinBox_sweep_freq->text().toDouble() + ui->doubleSpinBox_sweep_step->text().toDouble();
    ui->doubleSpinBox_sweep_freq->setValue(frequency);
    this->ad4351->frequency = ui->doubleSpinBox_sweep_freq->text().toDouble();

    if ( frequency >= ui->doubleSpinBox_sweep_end->value())
    {
        if (ui->checkBox_sweep_loop->isChecked())
        {
            ui->doubleSpinBox_sweep_freq->setValue(ui->doubleSpinBox_sweep_start->value());
        }
        else
        {
            this->sweep_stop_click();
        }
    }

    emit singal_recalculate();
}

void USBIOBoard::display_reg()
{
    ui->line_reg0->setText(QString::number(this->ad4351->reg[0],16).toUpper());
    ui->line_reg1->setText(QString::number(this->ad4351->reg[1],16).toUpper());
    ui->line_reg2->setText(QString::number(this->ad4351->reg[2],16).toUpper());
    ui->line_reg3->setText(QString::number(this->ad4351->reg[3],16).toUpper());
    ui->line_reg4->setText(QString::number(this->ad4351->reg[4],16).toUpper());
    ui->line_reg5->setText(QString::number(this->ad4351->reg[5],16).toUpper());

    if (this->enable_auto_tx)
    {
        emit signal_auto_tx();
    }
}

void USBIOBoard::update_gui(bool isConnected, UI_Data *ui_data)
{
    if(isConnected)
    {
        if (!ui_data->readFirmwareInfoPending   )
        {
            this->setWindowTitle("AD4351 I/O Board : FW : " + QString::number( ui_data->firmware_version_major ) +"." + QString::number( ui_data->firmware_version_minor ) + ":" +  QString::number( ui_data->firmware_build_number ) + " Device Found");
        }
        else
        {
            this->setWindowTitle("AD4351 I/O Board : Device Found");
        }

        if (!ui_data->readFirmwareInfoPending)
        {

            if (ui_data->RF_OUT)
            {
                ui->RF_CTRL->setText("RF : ON");
            }else
            {
                ui->RF_CTRL->setText("RF : OFF");
            }
        }
    }
    else
    {
        ui_data->readRFCTRL_pending = true;
        ui_data->readFirmwareInfoPending = true;
        // stop all timers here
        this->setWindowTitle("AD4351 I/O Board : Device Not Found");
        ui->RF_CTRL->setText("RF : XX");
        sweep_timer->stop();
        hop_timer->stop();
    }
}
