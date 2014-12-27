/**
 * @file CosaTM1637_test.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014, jediunix
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 */

#include "Cosa/IOStream/Driver/UART.hh"
#include "Cosa/Trace.hh"
#include "Cosa/OutputPin.hh"

#include "TM1637.hh"

#define PAUSE 500 // ms
#define STEP(x) {x; delay(PAUSE);}

TM1637 tm1637(Board::D20, Board::D21);


void setup()
{
  uart.begin(9600);
  trace.begin(&uart, PSTR("CosaTM1637_test: started"));
}

void loop()
{
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff};
  uint8_t i;

  // All on/off
  STEP(tm1637.set(data));
  STEP(tm1637.clear());
  STEP(tm1637.set(data));
  STEP(tm1637.clear());

  // Colon
  data[0] = 0;
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  for (i=0; i<4; i++)
    {
      tm1637.colon(!tm1637.colon());
      STEP(tm1637.set(data));
    }
  STEP(tm1637.clear());
  
  // Walking
  data[0] = tm1637.encode(0);
  for (i=0; i<4; i++)
    {
      STEP(tm1637.set(data, 1, i));
      tm1637.clear();
    }
  for (i=0; i<3; i++)
    {
      STEP(tm1637.set(data, 1, 2-i));
      tm1637.clear();
    }

  // Pretty patterns
  data[0] = TM1637::SEG_F | TM1637::SEG_E | TM1637::SEG_B | TM1637::SEG_C;
  for (i=0; i<4; i++)
    STEP(tm1637.set(data, 1, i));
  STEP(tm1637.clear());
  data[0] = TM1637::SEG_A | TM1637::SEG_G | TM1637::SEG_D;
  for (i=0; i<4; i++)
    STEP(tm1637.set(data, 1, i));
  STEP(tm1637.clear());
  data[0] = TM1637::SEG_F | TM1637::SEG_G | TM1637::SEG_C;
  for (i=0; i<4; i++)
    STEP(tm1637.set(data, 1, i));
  STEP(tm1637.clear());
  data[0] = TM1637::SEG_B | TM1637::SEG_G | TM1637::SEG_E;
  for (i=0; i<4; i++)
    STEP(tm1637.set(data, 1, i));
  STEP(tm1637.clear());

  // Box
  data[0] = TM1637::SEG_A | TM1637::SEG_D | TM1637::SEG_F | TM1637::SEG_E;
  data[1] = TM1637::SEG_A | TM1637::SEG_D;
  data[2] = TM1637::SEG_A | TM1637::SEG_D;
  data[3] = TM1637::SEG_A | TM1637::SEG_D | TM1637::SEG_B | TM1637::SEG_C;
  STEP(tm1637.set(data));
  tm1637.colon(true);
  STEP(tm1637.set(data));
  tm1637.colon(false);
  STEP(tm1637.clear());

  // Line
  data[0] = TM1637::SEG_G;
  for (i=0; i<4; i++)
    STEP(tm1637.set(data, 1, i));
  STEP(tm1637.clear());
  
  // Step
  data[0] = TM1637::SEG_D;
  data[1] = TM1637::SEG_G;
  data[2] = TM1637::SEG_A;
  data[3] = TM1637::SEG_C;
  for (i=0; i<4; i++)
    STEP(tm1637.set(&data[i], 1, i));
  STEP(tm1637.clear());
  
  // Basic numbers with leading zeros
  for (i=0; i<10; i++)
    STEP(tm1637.show(i, true));
  STEP(tm1637.clear());

  // Hex
  for (i=0; i<16; i++)
    STEP(tm1637.show(i, false, 16));
  STEP(tm1637.clear());

  // Binary
  for (i=0; i<8; i++)
    STEP(tm1637.show(i, false, 2));
  STEP(tm1637.clear());
  
  // Brightness Test
  uint8_t before = tm1637.brightness();
  for(i=TM1637::DIM; i<=TM1637::BRIGHT; i++)
    {
      tm1637.brightness(i);
      STEP(tm1637.show(i, true));
    }
  tm1637.brightness(before);
  STEP(tm1637.clear());

  // Colon
  for (i=0; i<10; i++)
    {
      tm1637.colon(!tm1637.colon());
      STEP(tm1637.show(2400 + i));
    }
  STEP(tm1637.clear());
}
