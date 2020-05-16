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




#include "NVSAVR_AD567x.h"

/* 
    
    CONNECTIONS:

    - AD567x SCLK -> Arduino SCLK Pin
    - AD567xSDI -> Arduino MOSI Pin
    - AD567x SYNC -> Arduino Pin 10

    - AD567x RESET -> Tie to AD567x VLogic
    - AD567x GAIN -> Tie to AD567x VLogic
    - AD567x LDAC -> Tie to GND
    - AD567x RTSEL -> tie to GND
    
*/

AD567x ad567x;
   
void setup() 
{
  ad567x.init(10);
}


void loop() {

  uint16_t v = 32760 + sin(micros() * M_PI  / 60000) * 32760;

  for (int i=7;i>=0;i--)
    ad567x.analogWrite(i, v);   
}
