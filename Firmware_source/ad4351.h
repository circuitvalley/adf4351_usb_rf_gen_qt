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


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef AD4351_H
#define	AD4351_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "system.h"

#define LD_PORT C
#define LD_BIT  0

#define PDBrf_PORT C
#define PDBrf_BIT 1

#define LE_PORT B
#define LE_BIT  5

#define MUX_PORT B
#define MUX_BIT 7

#define CE_PORT C
#define CE_BIT  2

#define CLK_PORT B
#define CLK_BIT  6

#define SDO_PORT C
#define SDO_BIT 7



#define set_PASTER(REG, BIT, REG_TYPE, MEMBER, val) ## REG_TYPE ## REG ## bits. ## MEMBER ## REG ## BIT = val
#define get_PASTER(REG, BIT, REG_TYPE, MEMBER)      ## REG_TYPE ## REG ## bits. ## MEMBER ## REG ## BIT 

#define set_LAT(REG, BIT, val)  set_PASTER(REG, BIT, LAT, LAT, val)
#define set_TRIS(REG, BIT, val)  set_PASTER(REG, BIT, TRIS, TRIS, val)
#define set_ANSEL(REG, BIT, val)  set_PASTER(REG, BIT, ANSEL, ANS, val)

#define get_PORT(REG, BIT)  get_PASTER(REG, BIT, PORT, R)


#define setCE(val)      set_LAT(CE_PORT, CE_BIT, val)
#define setLE(val)      set_LAT(LE_PORT, LE_BIT, val)
#define setPDBrf(val)   set_LAT(PDBrf_PORT, PDBrf_BIT, val)
#define setPDBrf_ANSEL(val)   set_ANSEL(PDBrf_PORT, PDBrf_BIT, val)
#define getPDBrf        get_PORT(PDBrf_PORT, PDBrf_BIT)
#define getMUX          get_PORT(MUX_PORT, MUX_BIT)
#define getLD           get_PORT(LD_PORT, LD_BIT)

#define setCE_TRIS(val)     set_TRIS(CE_PORT, CE_BIT, val)
#define setLE_TRIS(val)     set_TRIS(LE_PORT, LE_BIT, val)
#define setPDBrf_TRIS(val)  set_TRIS(PDBrf_PORT, PDBrf_BIT, val)
#define setMUX_TRIS(val)    set_TRIS(MUX_PORT, MUX_BIT, val)
#define setLD_TRIS(val)     set_TRIS(LD_PORT, LD_BIT, val)
#define setSDO_TRIS(val)    set_TRIS(SDO_PORT, SDO_BIT, val)
#define setCLK_TRIS(val)    set_TRIS(CLK_PORT, CLK_BIT, val)

#define ANSEL_ANALOG 1
#define ANSEL_DIGITAL 0

#define TRIS_OUTPUT  0
#define TRIS_INPUT 1
void ad4351_write_reg(AD4351_reg *AD4351);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

