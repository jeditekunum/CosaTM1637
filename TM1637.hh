/**
 * @file TM1637.hh
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

#ifndef COSA_TM1637_HH
#define COSA_TM1637_HH

#include "Cosa/OutputPin.hh"
#include "Cosa/IOPin.hh"

//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D

/**
 * Driver for the TM1637 2-wire 4 digit 7 segment LED.
 * Note this is NOT an I2C compatible device.
 */
class TM1637 {
public:
  /**
   * Segments
   */
  static const uint8_t  SEG_A = 0b00000001;
  static const uint8_t  SEG_B = 0b00000010;
  static const uint8_t  SEG_C = 0b00000100;
  static const uint8_t  SEG_D = 0b00001000;
  static const uint8_t  SEG_E = 0b00010000;
  static const uint8_t  SEG_F = 0b00100000;
  static const uint8_t  SEG_G = 0b01000000;
  
  /**
   * Brightness range.
   */
  static const uint8_t DIM = 0;
  static const uint8_t NORMAL = 2;
  static const uint8_t BRIGHT = 3;

  /**
   * Construct TM1637 device.
   */
  TM1637(Board::DigitalPin clk, Board::DigitalPin dio) :
    m_clk(clk),
    m_dio(dio, IOPin::OUTPUT_MODE),
    m_brightness(NORMAL)
  {}

  /**
   * Clear.
   */
  void clear();

  /**
   * Set digit(s)
   * @param[in] digit(s)
   * @param[in] number of digits
   * @param[in] position starting at 0 (0 is left, 3 is right)
   */
  void set(const uint8_t* digits, uint8_t length = 4, uint8_t position = 0);

  /**
   * Display number.
   * @param[in] number 0..0xFFFF.
   * @param[in] leading_zeros if true.
   * @param[in] base base 2, 10, or 16
   * @param[in] length number of digits to show
   * @param[in] position offset to begin
   */
  void show(uint16_t number, bool leading_zeros = false,
            uint8_t base = 10, uint8_t length = 4, uint8_t position = 0);

  /**
   * Get digit bitmap for a value 0-15 (10-15 are hex)
   * @param[in] value
   * @return digit
   */
  uint8_t encode(uint8_t digit)
    __attribute__((always_inline))
  {
    return (table[digit & 0xF]);
  }

  /**
   * Get brightness.
   * @return brighness
   */
  uint8_t brightness()
    __attribute__((always_inline))
  {
    return (m_brightness);
  }

  /**
   * Set brightness and return current setting.
   * @param[in] set brigthness to set.
   * @returns old_brightness.
   */
  uint8_t brightness(uint8_t set)
    __attribute__((always_inline))
  {
    uint8_t old = m_brightness;
    if (set <= BRIGHT)
      m_brightness = set;
    return (old);
  }

  /**
   * Get colon.
   * @return colon
   */
  bool colon()
    __attribute__((always_inline))
  {
    return (m_colon);
  }

  /**
   * Set colon and return current setting.
   * @param[in] flag colon on/off.
   * @returns old_colon.
   */
  bool colon(bool flag)
    __attribute__((always_inline))
  {
    bool old = m_colon;
    m_colon = flag;
    return (old);
  }

protected:
  /* Pins */
  OutputPin m_clk;
  IOPin m_dio;

  /** Brightness. */
  uint8_t m_brightness;

  /** colon illuminated? */
  bool m_colon;

  static const uint8_t table[] __PROGMEM;

  void stall();
  void start();
  void stop();
  void write(uint8_t byte);
};

#endif
