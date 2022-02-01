#include "st7735.h"

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

namespace {
constexpr const uint8_t NOP = 0x00;
constexpr const uint8_t SWRESET = 0x01;
constexpr const uint8_t RDDID = 0x04;
constexpr const uint8_t RDDST = 0x09;

constexpr const uint8_t SLPIN = 0x10;
constexpr const uint8_t SLPOUT = 0x11;
constexpr const uint8_t PTLON = 0x12;
constexpr const uint8_t NORON = 0x13;

constexpr const uint8_t INVOFF = 0x20;
constexpr const uint8_t INVON = 0x21;
constexpr const uint8_t DISPOFF = 0x28;
constexpr const uint8_t DISPON = 0x29;
constexpr const uint8_t CASET = 0x2A;
constexpr const uint8_t RASET = 0x2B;
constexpr const uint8_t RAMWR = 0x2C;
constexpr const uint8_t RAMRD = 0x2E;

constexpr const uint8_t PTLAR = 0x30;
constexpr const uint8_t COLMOD = 0x3A;
constexpr const uint8_t MADCTL = 0x36;

constexpr const uint8_t FRMCTR1 = 0xB1;
constexpr const uint8_t FRMCTR2 = 0xB2;
constexpr const uint8_t FRMCTR3 = 0xB3;
constexpr const uint8_t INVCTR = 0xB4;
constexpr const uint8_t DISSET5 = 0xB6;

constexpr const uint8_t PWCTR1 = 0xC0;
constexpr const uint8_t PWCTR2 = 0xC1;
constexpr const uint8_t PWCTR3 = 0xC2;
constexpr const uint8_t PWCTR4 = 0xC3;
constexpr const uint8_t PWCTR5 = 0xC4;
constexpr const uint8_t VMCTR1 = 0xC5;

constexpr const uint8_t RDID1 = 0xDA;
constexpr const uint8_t RDID2 = 0xDB;
constexpr const uint8_t RDID3 = 0xDC;
constexpr const uint8_t RDID4 = 0xDD;

constexpr const uint8_t PWCTR6 = 0xFC;

constexpr const uint8_t GMCTRP1 = 0xE0;
constexpr const uint8_t GMCTRN1 = 0xE1;
} // namespace

St7735::St7735(Config::Display const &config) : config_{config} {

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  spi_init(config_.spi, 5'000'000);
  gpio_set_function(config_.mosi, GPIO_FUNC_SPI);
  gpio_set_function(config_.clock, GPIO_FUNC_SPI);

  gpio_init(config_.chip_select);
  gpio_set_dir(config_.chip_select, GPIO_OUT);
  gpio_put(config_.chip_select, true);

  gpio_init(config_.data_command);
  gpio_set_dir(config_.data_command, GPIO_OUT);

  gpio_init(config_.reset);
  gpio_set_dir(config_.reset, GPIO_OUT);

  // initialize
  gpio_put(config_.chip_select, false);
  gpio_put(config_.reset, true);
  gpio_put(config_.data_command, true);
  sleep_ms(200);

  gpio_put(config_.reset, false);
  sleep_ms(200);

  gpio_put(config_.reset, true);
  sleep_ms(200);

  write_command(SWRESET);
  sleep_ms(150);

  write_command(SLPOUT);
  sleep_ms(500);

  gpio_put(PICO_DEFAULT_LED_PIN, true);

  write_command(FRMCTR1, {0x01, 0x2C, 0x2D});
  write_command(FRMCTR2, {0x01, 0x2C, 0x2D});
  write_command(FRMCTR3, {0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D});

  write_command(INVCTR, {0x07});
  write_command(PWCTR1, {0xA2, 0x02, 0x84});
  write_command(PWCTR2, {0xC5});
  write_command(PWCTR3, {0x0A, 0x00});
  write_command(PWCTR4, {0x8A, 0x2A});
  write_command(PWCTR5, {0x8A, 0xEE});
  write_command(VMCTR1, {0x0E});
  write_command(INVOFF);
  write_command(MADCTL, {0xC8});
  write_command(COLMOD, {0x05});

  write_command(CASET, {0x00, 0x00, 0x00, 0x7F});
  write_command(RASET, {0x00, 0x00, 0x00, 0x7F});
  write_command(GMCTRP1, {0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29,
                          0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10});
  write_command(GMCTRN1, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E,
                          0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10});

  gpio_put(PICO_DEFAULT_LED_PIN, false);

  write_command(NORON);
  sleep_ms(10);

  write_command(DISPON);
  sleep_ms(100);
}

void St7735::set_window(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  write_command(CASET, {0x00, static_cast<uint8_t>(x + 2), 0x00,
                        static_cast<uint8_t>(x + width + 1)});
  write_command(RASET, {0x00, static_cast<uint8_t>(y + 3), 0x00,
                        static_cast<uint8_t>(y + height + 2)});
  write_command(RAMWR);
}

void St7735::colorize_next_pixel(uint16_t color) {
  gpio_put(config_.chip_select, false);
  gpio_put(config_.data_command, true);

  uint8_t const value[2]{static_cast<uint8_t>(color >> 8),
                         static_cast<uint8_t>(color & 0xFF)};
  spi_write_blocking(config_.spi, value, 2);

  gpio_put(config_.chip_select, true);
}

void St7735::clear_screen(uint16_t color) {
  set_window(0, 0, 128, 128);
  for (uint i = 0; i < 128 * 128; ++i) {
    colorize_next_pixel(color);
  }
}

void St7735::write_command(uint8_t command) {
  gpio_put(config_.chip_select, false);
  gpio_put(config_.data_command, false);

  spi_write_blocking(config_.spi, &command, 1);

  gpio_put(config_.chip_select, true);
}

void St7735::write_command(uint8_t command,
                           std::initializer_list<uint8_t> const &data) {
  gpio_put(config_.chip_select, false);
  gpio_put(config_.data_command, false);

  spi_write_blocking(config_.spi, &command, 1);

  gpio_put(config_.data_command, true);

  for (auto const value : data) {
    spi_write_blocking(config_.spi, &value, 1);
  }

  gpio_put(config_.chip_select, true);
}
