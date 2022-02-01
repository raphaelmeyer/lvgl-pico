#pragma once

#include "hw_config.h"

#include <initializer_list>

extern "C" struct spi_inst;

class St7735 {
public:
  St7735(Config::Display const &config);

  void set_window(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  void colorize_next_pixel(uint16_t color);

  void clear_screen(uint16_t color);

private:
  void write_command(uint8_t command);
  void write_command(uint8_t command,
                     std::initializer_list<uint8_t> const &data);

  Config::Display const config_;
};
