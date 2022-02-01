#pragma once

#include <hardware/spi.h>
#include <hardware/structs/pio.h>

struct Config {

  struct Display {
    uint const clock;
    uint const mosi;
    uint const chip_select;
    uint const data_command;
    uint const reset;

    spi_inst_t *const spi;
  };

  Display display;
};
