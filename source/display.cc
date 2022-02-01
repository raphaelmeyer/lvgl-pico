#include "display.h"

namespace lv {

Display::Display(Config::Display const &config) : lcd_{config} {}

void Display::initialize() {
  lv_disp_draw_buf_init(&draw_buffer_, buffer_1_.data(), buffer_2_.data(),
                        buffer_1_.size());

  lv_disp_drv_init(&driver_);

  driver_.draw_buf = &draw_buffer_;
  driver_.hor_res = Width;
  driver_.ver_res = Height;
  driver_.flush_cb = &Display::flush_cb;
  driver_.user_data = this;

  lv_disp_drv_register(&driver_);

  add_repeating_timer_ms(-5, &Display::tick_inc, NULL, &timer_tick_inc_);
  add_repeating_timer_ms(5, &Display::timer_handler, NULL,
                         &timer_timer_handler_);
}

void Display::flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                       lv_color_t *color_p) {
  auto self = static_cast<Display *>(disp_drv->user_data);
  if (self) {
    self->draw(area, color_p);
  }
  lv_disp_flush_ready(disp_drv);
}

void Display::draw(const lv_area_t *area, lv_color_t *color_p) {
  auto const width = area->x2 + 1 - area->x1;
  auto const height = area->y2 + 1 - area->y1;

  auto color = color_p;

  lcd_.set_window(area->x1, area->y1, width, height);
  for (int i = 0; i < width * height; ++i) {
    lcd_.colorize_next_pixel(color->full);
    ++color;
  }
}

bool Display::tick_inc(repeating_timer_t *) {
  lv_tick_inc(10);
  return true;
}

bool Display::timer_handler(repeating_timer_t *) {
  lv_timer_handler();
  return true;
}

} // namespace lv
