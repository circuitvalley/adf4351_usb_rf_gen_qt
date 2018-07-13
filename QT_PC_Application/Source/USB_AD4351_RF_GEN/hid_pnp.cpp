#include "hid_pnp.h"

HID_PnP::HID_PnP(QObject *parent) : QObject(parent) {
    ui_data.isConnected = false;

    ui_data.ioUpdatePending = 0;

    device = NULL;
    buf[0] = 0x00;
    buf[1] = 0x37;
    memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
    memset(ui_data.reg, 0, sizeof(ui_data.reg));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));

    timer->start(250);

    slow_read = new QTimer();
    connect(slow_read, SIGNAL(timeout()), this, SLOT(slow_read_timeout()));
    slow_read->start(1000);
}

HID_PnP::~HID_PnP() {
    disconnect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));
}

void HID_PnP::PollUSB()
{
    buf[0] = 0x00;
    memset((void*)&buf[2], 0x00, sizeof(buf) - 2);

    if (ui_data.isConnected == false) {
        device = hid_open(0x1209, 0x7877, NULL);  //vid pid

        if (device) {
            ui_data.isConnected = true;
            hid_set_nonblocking(device, true);
            timer->start(15);
        }
    }
    else {
        if(ui_data.regUpdatePending == true) {
            ui_data.regUpdatePending = false;

            buf[1] = COMMAND_SET_REG ;
            memcpy(&buf[2], ui_data.reg, sizeof(ui_data.reg));

            if (hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }
        }
        if(ui_data.RF_CTRL_Pending == true)
        {
            ui_data.RF_CTRL_Pending = false;
            buf[1] = COMMAND_RF_CTRL ;
            buf[2] = ui_data.RF_OUT ;

            if (hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }
        }


        if ( ui_data.readFirmwareInfoPending == true )
        {

            buf[0] = 0x00;
            buf[1] = COMMAND_GET_BUILD_INFO;
            memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
            if (hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }
            if(hid_read(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }


        }

        if (ui_data.readRFCTRL_pending)
        {
            buf[0] = 0x00;
            buf[1] = COMMAND_READ_RF_CTRL;
            memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
            if (hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }

            if(hid_read(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }


        }


        if (buf[0] == COMMAND_READ_RF_CTRL)
        {
            ui_data.readRFCTRL_pending = false;
            ui_data.RF_OUT = (bool) buf[1];
            hid_comm_update(ui_data.isConnected, &ui_data);

        }
        else if (buf[0] == COMMAND_GET_BUILD_INFO)
        {
            ui_data.readFirmwareInfoPending = false;
            ui_data.firmware_version_major = buf[ 1 ];
            ui_data.firmware_version_minor = buf[ 2 ];
            ui_data.firmware_build_number = (buf[ 3 ] << 8) | buf[ 4 ];
            hid_comm_update(ui_data.isConnected, &ui_data);

        }


    }

}



void HID_PnP::change_reg(const uint32_t *reg) {
    memcpy(ui_data.reg , reg, sizeof(ui_data.reg));
    ui_data.regUpdatePending= true;
}


void HID_PnP::change_RF_CTRL()
{
    ui_data.RF_OUT = !ui_data.RF_OUT;
    ui_data.RF_CTRL_Pending= true;
    hid_comm_update(ui_data.isConnected, &ui_data);

}

void HID_PnP::CloseDevice()
{
    hid_close(device);
    device = NULL;
    ui_data.isConnected = false;



    ui_data.ioUpdatePending = 0;
    hid_comm_update(ui_data.isConnected, &ui_data);
    timer->start(250);

}


void HID_PnP::slow_read_timeout()
{
    ui_data.readFirmwareInfoPending = true;
    ui_data.readRFCTRL_pending  = true;
}
