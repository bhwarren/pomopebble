#include <pebble.h>
#include "mainwindow.h"
#include "constantes.h"
#include "configwindow.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GBitmap *s_res_cog_btn;
static GBitmap *s_res_play_btn;
static GBitmap *s_res_reload_btn;
static GFont s_res_roboto_condensed_21;
static TextLayer *timer_layer;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *counter_layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_cog_btn = gbitmap_create_with_resource(RESOURCE_ID_COG_BTN);
  s_res_play_btn = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BTN);
  s_res_reload_btn = gbitmap_create_with_resource(RESOURCE_ID_RELOAD_BTN);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // timer_layer
  timer_layer = text_layer_create(GRect(-3, 46, 117, 61));
  text_layer_set_background_color(timer_layer, GColorClear);
  
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_font(timer_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_cog_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_reload_btn);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // counter_layer
  counter_layer = text_layer_create(GRect(22, 92, 85, 24));
  text_layer_set_background_color(counter_layer, GColorClear);
  text_layer_set_text(counter_layer, "Done: 3");
  text_layer_set_font(counter_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)counter_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(counter_layer);
  gbitmap_destroy(s_res_cog_btn);
  gbitmap_destroy(s_res_play_btn);
  gbitmap_destroy(s_res_reload_btn);
}
// END AUTO-GENERATED UI CODE

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  int var = persist_read_int(CONFIG_WORK_TIME);
  int var2 = persist_read_int(CONFIG_PAUSE_TIME);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%d - %d", var, var2);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_configwindow();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){
  static char text[] = "";
  int minutos = persist_read_int(CONFIG_WORK_TIME);
  snprintf(text, 7, "%d:00", minutos);
  text_layer_set_text(timer_layer, text);
}

void show_mainwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_background_color(s_window, GColorPictonBlue);
//   window_set_background_color(s_window, GColorRed);
//   window_set_background_color(s_window, GColorKellyGreen);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
  });
  window_stack_push(s_window, true);
}

void hide_mainwindow(void) {
  window_stack_remove(s_window, true);
}
