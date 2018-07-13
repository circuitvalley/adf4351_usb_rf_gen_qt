/*******************************************************************************
    Author:  Gaurav Singh
    website: www.circuitvalley.com 
    Created on July 12, 2018
    
    This file is part of Circuitvalley USB HID Bootloader.
    Circuitvalley USB HID Bootloader is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Circuitvalley USB HID Bootloader is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Circuitvalley USB HID Bootloader.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include <xc.h>
#include	<stdio.h>
#include "system.h"
#include "usb.h"

#include "spi.h"
#include "ad4351.h"

/** CONFIGURATION Bits **********************************************/
// PIC16F1459 configuration bit settings:
#define USE_INTERNAL_OSC
#if defined (USE_INTERNAL_OSC)	    // Define this in system.h if using the HFINTOSC for USB operation
    // CONFIG1
    #pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
    #pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
    #pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
    #pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
    #pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
    #pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
    #pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
    #pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
    #pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

    // CONFIG2
    #pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
    #pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (NO CPU system divide)
    #pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
    #pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
    #pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
    #pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
    #pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
    #pragma config LVP = OFF       // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#else
    // CONFIG1
    #pragma config FOSC = HS        // Oscillator Selection Bits (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
    #pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
    #pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
    #pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
    #pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
    #pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
    #pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
    #pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
    #pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

    // CONFIG2
    #pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
    #pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (NO CPU system divide)
    #pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
    #pragma config PLLMULT = 4x     // PLL Multipler Selection Bit (4x Output Frequency Selected)
    #pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
    #pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
    #pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
    #pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#endif

 AD4351_reg ad4351_system;

/*********************************************************************
* Function: void SYSTEM_Initialize( SYSTEM_STATE state )
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  SYSTEM_STATE - the state to initialize the system into
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize( SYSTEM_STATE state )
{ 
    switch(state)
    {
        case SYSTEM_STATE_USB_START:
            #if defined(USE_INTERNAL_OSC)
                //Make sure to turn on active clock tuning for USB full speed 
                //operation from the INTOSC
                OSCCON = 0xFC;  //HFINTOSC @ 16MHz, 3X PLL, PLL enabled
                ACTCON = 0x90;  //Active clock tuning enabled for USB
            #endif
            makeSerialNumber();
            DRV_SPI_INIT_DATA spiInitData;
            spiInitData.channel = DRV_SPI_CHANNEL_1;
            spiInitData.cke = 1;  
            spiInitData.divider = 0; //SPI master ,Fosc/4
            spiInitData.baudRate = 0; // 
            spiInitData.mode = SPI_TRANSFER_MODE_8BIT;
            spiInitData.spibus_mode = SPI_BUS_MODE_0; 
            DRV_SPI_Initialize(&spiInitData);
            setCE_TRIS(TRIS_OUTPUT);
            setLE_TRIS(TRIS_OUTPUT);
            setPDBrf_TRIS(TRIS_OUTPUT);
            setPDBrf_ANSEL(ANSEL_DIGITAL);
            setMUX_TRIS(TRIS_INPUT);
            setLD_TRIS(TRIS_INPUT);
            setSDO_TRIS(TRIS_OUTPUT);
            setCLK_TRIS(TRIS_OUTPUT);
            setCE(true);
            
            
            break;
            
        case SYSTEM_STATE_USB_SUSPEND: 
            break;
            
        case SYSTEM_STATE_USB_RESUME:
            break;
    }
}

			
			
void interrupt SYS_InterruptHigh(void)
{
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}
