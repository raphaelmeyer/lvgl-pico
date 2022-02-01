#include "hw_config.h"

#include "st7735.h"

#include <lvgl.h>

#include <array>

namespace lv {

class Display {
public:
  static constexpr size_t Width = 128;
  static constexpr size_t Height = 128;

  Display(Config::Display const &config);

  void initialize();

private:
  static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                       lv_color_t *color_p);

  static bool tick_inc(repeating_timer_t *);
  static bool timer_handler(repeating_timer_t *);

  void draw(const lv_area_t *area, lv_color_t *color_p);

  static constexpr size_t BufferSize = 16 * Width;

  std::array<lv_color_t, BufferSize> buffer_1_{};
  std::array<lv_color_t, BufferSize> buffer_2_{};

  lv_disp_draw_buf_t draw_buffer_{};
  lv_disp_drv_t driver_{};

  repeating_timer_t timer_tick_inc_{};
  repeating_timer_t timer_timer_handler_{};

  St7735 lcd_;
};

} // namespace lv
