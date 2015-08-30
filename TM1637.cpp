/**
 * @file TM1637.cpp
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, jeditekunum
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

#include "TM1637.hh"

#define AUTO       0x40
#define BASE       0xc0
#define BRIGHTNESS 0x88
#define COLON      0x80

#define CLEAR      0xff

#define STALL (DELAY(50))


const uint8_t TM1637::table[] __PROGMEM = {
//  XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

void
TM1637::clear()
{
  uint8_t clearbuf[] = {0, 0, 0, 0};
  bool save_colon = m_colon;
  m_colon = false;
  set(clearbuf, sizeof(clearbuf));
  m_colon = save_colon;
}

void
TM1637::set(const uint8_t* digits, uint8_t length, uint8_t position)
{
  start();
  write(AUTO);
  stop();

  start();
  write(BASE + (position & 0x3));
  for (uint8_t i=0; i < length; i++)
    write(digits[i] | (m_colon ? COLON : 0x0));
  stop();

  start();
  write(BRIGHTNESS + (m_brightness & 0x7));
  stop();
}

void
TM1637::show(uint16_t number, bool leading_zeros,
             uint8_t base, uint8_t length, uint8_t position)
{
  if (base != 2 && base != 10 && base != 16)
    base = 16;

  if (position > 3)
    position = 0;

  if (length > 4)
    length = 4;

  if (length == 0)
    length = 1;

  uint8_t value[] = {CLEAR, CLEAR, CLEAR, CLEAR};
  uint8_t digits = 0;

  if (number)
    {
      while (number)
        {
          if (digits < length)
            {
              value[length - 1 - digits] = pgm_read_byte(&table[number % base]);
              digits++;
              number /= base;
            }
          else
            break;
        }

      for (uint8_t i = 0; i < 4; i++)
        if (value[i] == CLEAR)
          value[i] = leading_zeros ? pgm_read_byte(&table[0]) : 0;
    }
  else
    {
      value[length-1] = pgm_read_byte(&table[0]);
    }

  set(value, length, position);
}

void
TM1637::start()
{
  m_dio.low();
  STALL;
}

void
TM1637::stop()
{
  m_dio.low();
  STALL;
  m_clk.high();
  STALL;
  m_dio.high();
  STALL;
}

void
TM1637::write(uint8_t byte)
{
  for (uint8_t bit=0; bit<8; bit++)
    {
      m_clk.low();
      STALL;

      if (byte & 0x1)
        m_dio.high();
      else
        m_dio.low();
      STALL;

      byte >>= 1;

      m_clk.high();
      STALL;
    }

  m_clk.low();
  STALL;

  m_dio.set_mode(IOPin::INPUT_MODE);
  STALL;

  m_clk.high();
  STALL;

  //  while(m_dio.read());

  m_dio.set_mode(IOPin::OUTPUT_MODE);
  m_dio.low();
  STALL;

  m_clk.low();
  STALL;
}
