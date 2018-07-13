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


// DOM-IGNORE-END


#include <xc.h>
#include "spi.h"
//#include "system_config.h"
#include "system.h"
#include <stdint.h>

#define DRV_SPI_CONFIG_CHANNEL_1_ENABLE        

// error checks
#if !defined(DRV_SPI_CONFIG_CHANNEL_1_ENABLE) && !defined(DRV_SPI_CONFIG_CHANNEL_2_ENABLE) && !defined(DRV_SPI_CONFIG_CHANNEL_3_ENABLE))
    #warning "No SPI Channel defined! Please define in system_config.h or system.h"
#endif

static int spiMutex[3] = { 0, 0, 0};

/*****************************************************************************
 * void DRV_SPI_Initialize(DRV_SPI_INIT_DATA *pData)
 *****************************************************************************/
void DRV_SPI_Initialize(DRV_SPI_INIT_DATA *pData)
{
#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE        
    if (pData->channel == 1)
    {
        SSP1STAT = 0x00;
        SSP1STATbits.CKE = pData->cke;

        SSP1CON1 = 0x00;
        SSP1CON1bits.SSPM = pData->divider;

        switch (pData->spibus_mode)
        {
            case SPI_BUS_MODE_0:
                //smp = 0, ckp = 0
                SSP1CON1bits.CKP = 0;
                SSP1STATbits.SMP = 0;
                break;
            case SPI_BUS_MODE_1:
                //smp = 1, ckp = 0
                SSP1CON1bits.CKP = 0;
                SSP1STATbits.SMP = 1;
                break;
            case SPI_BUS_MODE_2:
                //smp = 0, ckp = 1
                SSP1CON1bits.CKP = 1;
                SSP1STATbits.SMP = 0;
                break;
            case SPI_BUS_MODE_3:
                //smp = 1, ckp = 1
                SSP1CON1bits.CKP = 1;
                SSP1STATbits.SMP = 1;
                break;
            default:
                // should not happen
                break;
        }                   
        SSP1ADD = pData->baudRate;
        SSP1CON1bits.SSPEN = 1;
        
        return;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (pData->channel == 2)
    {
        SSP2STAT = 0x00;
        SSP2STATbits.CKE = pData->cke;

        SSP2CON1 = 0x00;
        SSP2CON1bits.SSPM = pData->divider;

        switch (pData->spibus_mode)
        {
            case SPI_BUS_MODE_0:
                //smp = 0, ckp = 0
                SSP2CON1bits.CKP = 0;
                SSP2STATbits.SMP = 0;
                break;
            case SPI_BUS_MODE_1:
                //smp = 1, ckp = 0
                SSP2CON1bits.CKP = 0;
                SSP2STATbits.SMP = 1;
                break;
            case SPI_BUS_MODE_2:
                //smp = 0, ckp = 1
                SSP2CON1bits.CKP = 1;
                SSP2STATbits.SMP = 0;
                break;
            case SPI_BUS_MODE_3:
                //smp = 1, ckp = 1
                SSP2CON1bits.CKP = 1;
                SSP2STATbits.SMP = 1;
                break;
            default:
                // should not happen
                break;
        }                           
        
        SSP2CON1bits.SSPEN = 1;

        return;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (pData->channel == 3)
    {
        SSP3STAT = 0x00;
        SSP3STATbits.CKE = pData->cke;

        SSP3CON1 = 0x00;
        SSP3CON1bits.SSPM = pData->divider;

        switch (pData->spibus_mode)
        {
            case SPI_BUS_MODE_0:
                //smp = 0, ckp = 0
                SSP3CON1bits.CKP = 0;
                SSP3STATbits.SMP = 0;
                break;
            case SPI_BUS_MODE_1:
                //smp = 1, ckp = 0
                SSP3CON1bits.CKP = 0;
                SSP3STATbits.SMP = 1;
                break;
            case SPI_BUS_MODE_2:
                //smp = 0, ckp = 1
                SSP3CON1bits.CKP = 1;
                SSP3STATbits.SMP = 0;
                break;
            case SPI_BUS_MODE_3:
                //smp = 1, ckp = 1
                SSP3CON1bits.CKP = 1;
                SSP3STATbits.SMP = 1;
                break;
            default:
                // should not happen
                break;
        }                       
        
        SSP3CON1bits.SSPEN = 1;
        return;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
}

void DRV_SPI_Deinitialize (uint8_t channel)
{
#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
        SSP1CON1bits.SSPEN = 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
        SSP2CON1bits.SSPEN = 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
        SSP3CON1bits.SSPEN = 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
}

/*****************************************************************************
 * void SPIPut(unsigned int channel, unsigned char data)
 *****************************************************************************/
void DRV_SPI_Put(const uint8_t channel, const uint8_t data)
{
    uint8_t clear;
#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
        clear = SSP1BUF;
        SSP1IF = 0;
        SSP1BUF = data;
        if (SSP1CON1 & 0x80)
        {
            return;
        }
        else
        {
            while (!SSP1IF);
        }

        return;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
        clear = SSP2BUF;
        SSP2IF = 0;
        SSP2BUF = data;
        if (SSP2CON1 & 0x80)
        {
            return;
        }
        else
        {
            while (!SSP2IF);
        }

        return;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
        clear = SSP3BUF;
        SSP3IF = 0;
        SSP3BUF = data;
        if (SSP3CON1 & 0x80)
        {
            return;
        }
        else
        {
            while (!SSP3IF);
        }

        return;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
}

void DRV_SPI_PutBuffer (uint8_t channel, uint8_t * data, uint16_t count)
{
    static uint8_t bData;
    static uint16_t FSR0Save;
    static uint16_t PRODSave;

    if (count == 0)
    {
        return;
    }
    
    //Context save C compiler managed registers.
   // FSR0Save = FSR0; 
   // PRODSave = PROD;

    //Using PRODH and PRODL as 16 bit loop counter.  These are convenient since
    //they are always in the access bank.
  //  PROD = count;
    //Using FSR0 directly, for optimal SPI loop speed.
    FSR0 = (uint16_t)data; 

#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
#asm
            movf    POSTINC0, w  //Fetch next uint8_t to send and store in WREG
            bra     ASMSPI1XmitLoopEntryPoint
ASMSPI1XmitLoop:    
            movf    POSTINC0, w  //Pre-Fetch next uint8_t to send and temporarily store in WREG
            //Wait until last hardware SPI transaction is complete
ASMSPI1Offset2:
            btfss   SSP1IF_bit, 0
            bra     ASMSPI1Offset2
            
ASMSPI1XmitLoopEntryPoint:
            //Start the next SPI transaction
            bcf     SSP1IF_bit, 0   //Clear interrupt flag
            movwf   SSP1BUF       //Write next uint8_t to transmit to SSPBUF
            
            //Now decrement uint8_t counter for loop exit condition
            movlw   0x00
            decf    PRODL, f     //Decrement LSB
            subwfb  PRODH, f     //Decrement MSB, only if borrow from LSB decrement
            //Check if anymore uint8_ts remain to be sent
            movf    PRODL, w     //copy PRODL to WREG
            iorwf   PRODH, w     //Z bit will be set if both PRODL and PRODH == 0x00
            bnz     ASMSPI1XmitLoop  //Go back and loop if our counter isn't = 0x0000.
#endasm

        //Wait until the last SPI transaction is really complete.  
        //Above loop jumps out after the last uint8_t is started, but not finished yet.
        while(!SSP1IF);

        //Leave SPI module in a "clean" state, ready for next transaction.
        bData = SSP1BUF;         //Dummy read to clear BF flag.
        SSP1IF = 0; //Clear interrupt flag.
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
#asm
            movf    POSTINC0, w  //Fetch next uint8_t to send and store in WREG
            bra     ASMSPI2XmitLoopEntryPoint
ASMSPI2XmitLoop:    
            movf    POSTINC0, w  //Pre-Fetch next uint8_t to send and temporarily store in WREG
            //Wait until last hardware SPI transaction is complete
ASMSPI2Offset2:
            btfss   SSP2IF_bit, 0
            bra     ASMSPI2Offset2
            
ASMSPI2XmitLoopEntryPoint:
            //Start the next SPI transaction
            bcf     SSP2IF_bit, 0   //Clear interrupt flag
            movwf   SSP2BUF       //Write next uint8_t to transmit to SSPBUF
            
            //Now decrement uint8_t counter for loop exit condition
            movlw   0x00
            decf    PRODL, f     //Decrement LSB
            subwfb  PRODH, f     //Decrement MSB, only if borrow from LSB decrement
            //Check if anymore uint8_ts remain to be sent
            movf    PRODL, w     //copy PRODL to WREG
            iorwf   PRODH, w     //Z bit will be set if both PRODL and PRODH == 0x00
            bnz     ASMSPI2XmitLoop  //Go back and loop if our counter isn't = 0x0000.
#endasm

        //Wait until the last SPI transaction is really complete.  
        //Above loop jumps out after the last uint8_t is started, but not finished yet.
        while(!SSP2IF);

        //Leave SPI module in a "clean" state, ready for next transaction.
        bData = SSP2BUF;         //Dummy read to clear BF flag.
        SSP2IF = 0; //Clear interrupt flag.
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
#asm
            movf    POSTINC0, w  //Fetch next uint8_t to send and store in WREG
            bra     ASMSPI3XmitLoopEntryPoint
ASMSPI3XmitLoop:    
            movf    POSTINC0, w  //Pre-Fetch next uint8_t to send and temporarily store in WREG
            //Wait until last hardware SPI transaction is complete
ASMSPI3Offset1:
            btfss   SSP3IF_bit, 0
            bra     ASMSPI3Offset1
            
ASMSPI3XmitLoopEntryPoint:
            //Start the next SPI transaction
            bcf     SSP3IF_bit, 0   //Clear interrupt flag
            movwf   SSP3BUF       //Write next uint8_t to transmit to SSPBUF
            
            //Now decrement uint8_t counter for loop exit condition
            movlw   0x00
            decf    PRODL, f     //Decrement LSB
            subwfb  PRODH, f     //Decrement MSB, only if borrow from LSB decrement
            //Check if anymore uint8_ts remain to be sent
            movf    PRODL, w     //copy PRODL to WREG
            iorwf   PRODH, w     //Z bit will be set if both PRODL and PRODH == 0x00
            bnz     ASMSPI3XmitLoop  //Go back and loop if our counter isn't = 0x0000.
#endasm

        //Wait until the last SPI transaction is really complete.  
        //Above loop jumps out after the last uint8_t is started, but not finished yet.
        while(!SSP3IF);

        //Leave SPI module in a "clean" state, ready for next transaction.
        bData = SSP3BUF;         //Dummy read to clear BF flag.
        SSP3IF = 0; //Clear interrupt flag.
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE

    //Restore C compiler managed registers that we modified
   // PROD = PRODSave;
  //  FSR0 = FSR0Save;

    return;
}

/*****************************************************************************
 * uint8_t SPIGet (unsigned int channel)
 *****************************************************************************/
uint8_t DRV_SPI_Get (uint8_t channel)
{
    uint8_t clear;
#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
        clear = SSP1BUF;
        SSP1IF = 0;
        SSP1BUF = 0xFF;
        while (!SSP1IF);
        return SSP1BUF;
    }
#endif //#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
        clear = SSP2BUF;
        SSP2IF = 0;
        SSP2BUF = 0xFF;
        while (!SSP2IF);
        return SSP2BUF;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
        clear = SSP3BUF;
        SSP3IF = 0;
        SSP3BUF = 0xFF;
        while (!SSP3IF);
        return SSP3BUF;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE

    return 0x00;
}

void DRV_SPI_GetBuffer(uint8_t channel, uint8_t * data, uint16_t count)
{
    static uint16_t FSR0Save;
    static uint16_t PRODSave;

    if (count == 0)
    {
        return;
    }
    
    //Context save C compiler managed registers that we will modify in this function.
   // FSR0Save = FSR0;    
  //  PRODSave = PROD;    
    
    //Using PRODH and PRODL as convenient 16-bit access bank counter
  //  PROD = count;    //ioInfo.wNumuint8_ts holds the total number of uint8_ts
                                //this function will read from SPI.
    //Going to use the FSR0 directly.  This is non-conventional, but delivers
    //better performance than using a normal C managed software RAM pointer.
    FSR0 = (uint16_t)data;

#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
        //Initiate the first SPI operation
        WREG = SSP1BUF;
        SSP1IF = 0;
        SSP1BUF = 0xFF;

        //Highly speed efficient SPI read loop, written in inline assembly
        //language for best performance.  
#asm
            bra     ASMSPI1ReadLoopEntryPoint

ASMSPI1ReadLoop:
            // Wait until last hardware SPI transaction is complete
            btfss   SSP1IF_bit, 0
            bra     ASMSPI1ReadLoop
            bcf     SSP1IF_bit,  0

            // Save received uint8_t and start the next transfer
            movf    SSP1BUF, w    // Copy SPIBUF uint8_t into WREG
            setf    SSP1BUF       // Write 0xFF to SPIBUF, to start a SPI transaction
            movwf   POSTINC0      // Write the last received uint8_t to the user's RAM buffer
ASMSPI1ReadLoopEntryPoint:
            // Now decrement 16-bit counter for loop exit test condition
            movlw   0x00
            decf    PRODL, f     // Decrement LSB
            subwfb  PRODH, f     // Decrement MSB, only if borrow from LSB decrement
            // Check if anymore uint8_ts remain to be sent
            movf    PRODL, w     // copy PRODL to WREG
            iorwf   PRODH, w     // Z bit will be set if both PRODL and PRODH == 0x00
            bnz     ASMSPI1ReadLoop  // Go back and loop if our counter isn't = 0x0000.

            // Wait until the very last SPI transaction is complete and save the uint8_t
ASMSPI1Offset1:
            btfss   SSP1IF_bit, 0
            bra     ASMSPI1Offset1
            movff   SSP1BUF, POSTINC0
#endasm

        SSP1IF = 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
        //Initiate the first SPI operation
        WREG = SSP2BUF;
        SSP2IF = 0;
        SSP2BUF = 0xFF;

        //Highly speed efficient SPI read loop, written in inline assembly
        //language for best performance.  Total number of uint8_ts that will be fetched
        //is exactly == the value of ioInfo.wNumuint8_ts prior to calling this function.
#asm
            bra     ASMSPI2ReadLoopEntryPoint
        
ASMSPI2ReadLoop:
            //Wait until last hardware SPI transaction is complete
            btfss   SSP2IF_bit, 0
            bra     ASMSPI2ReadLoop
            bcf     SSP2IF_bit, 0

            //Save received uint8_t and start the next transfer
            movf    SSP2BUF, w    //Copy SPIBUF uint8_t into WREG
            setf    SSP2BUF          //Write 0xFF to SPIBUF, to start a SPI transaction
            movwf   POSTINC0      //Write the last received uint8_t to the user's RAM buffer
        
ASMSPI2ReadLoopEntryPoint:
            //Now decrement 16-bit counter for loop exit test condition
            movlw   0x00
            decf    PRODL, f     //Decrement LSB
            subwfb  PRODH, f     //Decrement MSB, only if borrow from LSB decrement
            //Check if anymore uint8_ts remain to be sent
            movf    PRODL, w     //copy PRODL to WREG
            iorwf   PRODH, w     //Z bit will be set if both PRODL and PRODH == 0x00
            bnz     ASMSPI2ReadLoop  //Go back and loop if our counter isn't = 0x0000.

            //Wait until the very last SPI transaction is complete and save the uint8_t
ASMSPI2Offset1:
            btfss   SSP2IF_bit, 0
            bra     ASMSPI2Offset1
            movff   SSP2BUF, POSTINC0
#endasm

        SSP2IF = 0;	 
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
        //Initiate the first SPI operation
        WREG = SSP3BUF;
        SSP3IF = 0;
        SSP3BUF = 0xFF;

        //Highly speed efficient SPI read loop, written in inline assembly
        //language for best performance.  Total number of uint8_ts that will be fetched
        //is exactly == the value of ioInfo.wNumuint8_ts prior to calling this function.
#asm
            bra     ASMSPI3ReadLoopEntryPoint
        
ASMSPI3ReadLoop:
            //Wait until last hardware SPI transaction is complete
            btfss   SSP3IF_bit, 0
            bra     ASMSPI3ReadLoop
            bcf     SSP3IF_bit, 0

            //Save received uint8_t and start the next transfer
            movf    SSP3BUF, w    //Copy SPIBUF uint8_t into WREG
            setf    SSP3BUF          //Write 0xFF to SPIBUF, to start a SPI transaction
            movwf   POSTINC0      //Write the last received uint8_t to the user's RAM buffer
        
ASMSPI3ReadLoopEntryPoint:
            //Now decrement 16-bit counter for loop exit test condition
            movlw   0x00
            decf    PRODL, f     //Decrement LSB
            subwfb  PRODH, f     //Decrement MSB, only if borrow from LSB decrement
            //Check if anymore uint8_ts remain to be sent
            movf    PRODL, w     //copy PRODL to WREG
            iorwf   PRODH, w     //Z bit will be set if both PRODL and PRODH == 0x00
            bnz     ASMSPI3ReadLoop  //Go back and loop if our counter isn't = 0x0000.

            //Wait until the very last SPI transaction is complete and save the uint8_t
ASMSPI3Offset3:
            btfss   SSP3IF_bit, 0
            bra     ASMSPI3Offset3
            movff   SSP3BUF, POSTINC0
#endasm

        SSP3IF = 0;	 
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE

    //Context restore C compiler managed registers
   // PROD = PRODSave;
   // FSR0 = FSR0Save;    

    return;
}

/*****************************************************************************
 * int SPILock(unsigned int channel)
 *****************************************************************************/
int DRV_SPI_Lock(uint8_t channel)
{
#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
        if(!spiMutex[0])
        {
            spiMutex[0] = 1;
            return 1;
        }

        return 0;
    }
#endif //#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
        if(!spiMutex[1])
        {
            spiMutex[1] = 1;
            return 1;
        }

        return 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
        if(!spiMutex[2])
        {
            spiMutex[2] = 1;
            return 1;
        }

        return 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    return -1;
}

/*****************************************************************************
 * int SPIUnLock(unsigned int channel)
 *****************************************************************************/
void DRV_SPI_Unlock(uint8_t channel)
{
#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
    if (channel == 1)
    {
        spiMutex[0] = 0;
    }
#endif //#ifdef DRV_SPI_CONFIG_CHANNEL_1_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
    if (channel == 2)
    {
        spiMutex[1] = 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_2_ENABLE
#ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
    if (channel == 3)
    {
        spiMutex[2] = 0;
    }
#endif // #ifdef DRV_SPI_CONFIG_CHANNEL_3_ENABLE
}
