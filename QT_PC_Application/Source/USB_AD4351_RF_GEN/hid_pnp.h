#ifndef HID_PNP_H
#define HID_PNP_H

#include <QObject>
#include <QTimer>
#include "../HIDAPI/hidapi.h"

#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ADC_CHANNEL 8  //including tempsensor
#define MAX_STR 65

#define REF_VREF_PIN     4 //has nothing to do with FVR but still hier
#define REF_4v096   3
#define REF_2v048   2
#define REF_1v025   1

typedef enum
{
      COMMAND_SET_REG = 0x80,
      COMMAND_GET_REG = 0x83,
      COMMAND_RF_CTRL = 0x81,
      COMMAND_READ_RF_CTRL = 0x82,
      COMMAND_GET_BUILD_INFO = 0xB0,

} CUSTOM_HID_DEMO_COMMANDS;

class UI_Data
{
    public:
    bool isConnected;
    bool ioUpdatePending = true;
    bool pwdUpdatePending = true;
    bool regUpdatePending = true;
    bool RF_CTRL_Pending = true;
    bool readFirmwareInfoPending = true;
    uint8_t readRFCTRL_pending = true;
    uint16_t firmware_build_number;
    uint8_t firmware_version_major;
    uint8_t firmware_version_minor;
    uint32_t reg[6];
    bool RF_OUT = false;
};

class HID_PnP : public QObject
{
    Q_OBJECT
public:
    explicit HID_PnP(QObject *parent = 0);
    ~HID_PnP();

signals:
    void hid_comm_update(bool isConnected, UI_Data *ui_data);

public slots:
    void PollUSB();
    void change_reg(const uint32_t *reg);
    void change_RF_CTRL();
    void slow_read_timeout();

private:

    UI_Data ui_data;

    hid_device *device;
    QTimer *timer;
    QTimer *slow_read;
    unsigned char buf[MAX_STR];

    void CloseDevice();
};

#endif // HID_PNP_H
