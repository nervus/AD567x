/****************************************************************************
 *                   ,__
 *                  ( ..) 
 *  ______o00-------------00o_________________________________________________
 * | ___  __, ___.         __,         .    ,__                               |
 * | |  | |__ |__)\  /|  |(___        /||  /|__)                              |
 * | |  | |__ |  \ \/ |__|.___)      /-|| / | \                   nervusAVR   |
 * |________________________________/___|/_____\______________________________/
 *
 * Copyright (c) 2020 Mircea Marghidanu
 * BSD-3-Clause license ( https://opensource.org/licenses/BSD-3-Clause )
 *
 *  
 * Copyright (c) 2020, Mircea Marghidanu
 * All rights reserved.
 * 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * 
 * 
 ******************************************************************************/

#pragma once

#include "Arduino.h"
#include <SPI.h>

struct AD567x
{
   /*

      Data sheet:

         https://www.analog.com/media/en/technical-documentation/data-sheets/ad5672r_5676r.pdf


         Simplest connection ( 3 wire ):

          - SCLK - SCLK
          - SDI - MOSI
          - SYNC - CS Pin (pin 10)

          - RESET - Tie to VLogic
          - GAIN - Tie to VLogic
          - LDAC - Tie to GND
          - RTSEL - tie to GND
   
   */

    struct Command
    {
      enum
      {
         Nop = 0,                // No operation
         WriteToReg,             // Write to Input Register n where n = 1 to 8, depending on the DAC selected from the address bits in Table 11 (dependent on LDAC)
         UpdateDACReg,           // Update DAC Register n with contents of Input Register n
         WriteAndUpdateDac,      // Write to and update DAC Channel n
         PowerOnOff,             // Power down/power up the DAC
         LDACMaskReg,            // Hardware LDAC mask register
         SoftReset,              // Software reset (power-on reset)
         IntRefAndGainSetupReg,  // Internal reference and gain setup register
         DCENRegSetup,           // Set up the DCEN register (daisy-chain enable)
         ReadbackRegSetup,       // Set up the readback register (readback enable)
         UpdateAllInputRegs,     // Update all channels of the input register simultaneously with the input data
         UpdateAllDACRegs,       // Update all channels of the DAC register and input register simultaneously with the input data
 
         _Reserved0,             // Reserved
         _Reserved1,             // Reserved
         _Reserved2,             // Reserved
          
         Nop_DC,                 // No operation, daisy-chain mode
      };
   };

   struct Address { enum { DAC0 = 0, DAC1, DAC2, DAC3, DAC4, DAC5, DAC6, DAC7 }; };

   void init(int syncPin)
   {
      _SyncPin = syncPin;

      digitalWrite(_SyncPin, HIGH);
      pinMode(_SyncPin, OUTPUT);
      
      delay(10);

      SPI.begin();
      softReset();
      

   }
   void softReset()
   {
        writeCommand(Command::SoftReset, 0, 0x1234);
   }

   void analogWrite(uint8_t channel, uint16_t value)
   {
      
      writeCommand(Command::WriteToReg, channel, value);
   }

   void writeCommand(uint8_t command, uint8_t address, uint16_t data)
   {
        /*
           Packet structure:
     
           +-----------------------------------------------------------------------------------+
           | DB23 (MSB)                                                              DB0 (LSB) |
           +-------------+-------------+-------------------------------------------------------+
           | C3 C2 C1 C0 | A3 A2 A1 A0 | D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 |
           +-------------+-------------+-------------------------------------------------------+
           |    COMMAND  |   ADDRESS   |                       DATA BITS                       |
           +-------------+-------------+-------------------------------------------------------+
  
        */

  
        SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE1));
        {
          digitalWrite(_SyncPin, LOW);
       
          SPI.transfer((command << 4) + (address & 0xf));
          SPI.transfer16(data);

          digitalWrite(_SyncPin, HIGH);
        }
        SPI.endTransaction();

     }

   private:
   
      int _SyncPin = 10;

};
