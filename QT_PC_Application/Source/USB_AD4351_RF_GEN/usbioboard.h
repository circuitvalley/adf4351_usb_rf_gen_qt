#ifndef USBIOBOARD_H
#define USBIOBOARD_H

#include <QMainWindow>
#include "hid_pnp.h"
#include "ad4351.h"
#include <QTimer>

namespace Ui {
    class USB_AD4351_form;
}

class USBIOBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit USBIOBoard(QWidget *parent = 0);
    ~USBIOBoard();

private:
    Ui::USB_AD4351_form *ui;
    HID_PnP *plugNPlay;
    AD4351 *ad4351;
    QTimer *sweep_timer;
    QTimer *hop_timer;

    bool enable_auto_tx;
    void getDataFromUI();
    void showEvent(QShowEvent *event);

signals:
   void signal_update_io(uint16_t tris, uint16_t ansel,uint16_t alternate , uint16_t drive );
   void signal_update_pwm(uint16_t *duty,long frequency);
   void signal_update_reg(const uint32_t *reg);
   void signal_auto_tx();

   void singal_recalculate();
   void signal_update_RF_CTRL();
   void signal_update_dac(uint16_t value);
   void signal_update_ref(uint16_t adc, uint16_t dac );

public slots:
    void update_gui(bool isConnected, UI_Data *ui_data);
     void display_reg();
     void sweep_timer_timeout();
     void hop_timer_timeout();
     void hop_start_click();
     void hop_stop_click();
     void autotx_clicked();
     void sweep_start_click();
     void sweep_stop_click();
     void update_dac();
     void update_reg();
     void recalculate();
     void update_RF_CTRL();
};

#endif // USBIOBOARD_H
