#include "display.h"

#include <lvgl.h>

namespace {

Config const config{

    .display = {.clock = 18,
                .mosi = 19,
                .chip_select = 17,
                .data_command = 20,
                .reset = 21,
                .spi = spi0}

};

lv::Display display{config.display};

lv_obj_t *meter = nullptr;

void set_value(void *indic, int32_t v) {
  lv_meter_set_indicator_end_value(
      meter, static_cast<lv_meter_indicator_t *>(indic), v);
}

} // namespace

int main() {
  lv_init();

  display.initialize();

  // Demo [[

  meter = lv_meter_create(lv_scr_act());
  lv_obj_center(meter);
  lv_obj_set_size(meter, 120, 120);

  /*Remove the circle from the middle*/
  lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
  lv_obj_set_style_pad_all(meter, 0, LV_PART_MAIN);

  /*Add a scale first*/
  lv_meter_scale_t *scale = lv_meter_add_scale(meter);
  lv_meter_set_scale_ticks(meter, scale, 21, 1, 22, lv_color_hex3(0x888));
  lv_meter_set_scale_major_ticks(meter, scale, 5, 2, 22, lv_color_hex3(0x222),
                                 12);
  lv_meter_set_scale_range(meter, scale, 0, 100, 270, 135);

  // lv_meter_

  /*Add a three arc indicator*/
  lv_meter_indicator_t *indic1 =
      lv_meter_add_arc(meter, scale, 6, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_indicator_t *indic2 =
      lv_meter_add_arc(meter, scale, 6, lv_palette_main(LV_PALETTE_GREEN), -8);
  lv_meter_indicator_t *indic3 =
      lv_meter_add_arc(meter, scale, 6, lv_palette_main(LV_PALETTE_BLUE), -16);

  /*Create an animation to set the value*/
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_exec_cb(&a, set_value);
  lv_anim_set_values(&a, 0, 100);
  lv_anim_set_repeat_delay(&a, 100);
  lv_anim_set_playback_delay(&a, 100);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

  lv_anim_set_time(&a, 2000);
  lv_anim_set_playback_time(&a, 500);
  lv_anim_set_var(&a, indic1);
  lv_anim_start(&a);

  lv_anim_set_time(&a, 1000);
  lv_anim_set_playback_time(&a, 1000);
  lv_anim_set_var(&a, indic2);
  lv_anim_start(&a);

  lv_anim_set_time(&a, 1000);
  lv_anim_set_playback_time(&a, 2000);
  lv_anim_set_var(&a, indic3);
  lv_anim_start(&a);

  // ]]

  for (;;)
    ;
}
