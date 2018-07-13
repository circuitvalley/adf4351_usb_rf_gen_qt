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

#include "system.h"
#include "spi.h"
#include "ad4351.h"


/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */


void ad4351_write_reg(AD4351_reg *AD4351)
{
    int8_t i;
    for(i=(sizeof(AD4351->reg)/4)-1; i >= 0 ; --i)
    {
    DRV_SPI_Put(DRV_SPI_CHANNEL_1, (AD4351->reg[i]>>24) & 0xFF);
    DRV_SPI_Put(DRV_SPI_CHANNEL_1, (AD4351->reg[i]>>16) & 0xFF);
    DRV_SPI_Put(DRV_SPI_CHANNEL_1, (AD4351->reg[i]>>8) & 0xFF);
    DRV_SPI_Put(DRV_SPI_CHANNEL_1, (AD4351->reg[i]) & 0xFF);
    setLE(1);
    setLE(0);
    }
}


/* *****************************************************************************
 End of File
 */
