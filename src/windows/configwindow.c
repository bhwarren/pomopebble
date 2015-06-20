#include <pebble.h>
#include "configwindow.h"
#include "../constantes.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_condensed_21;
static TextLayer *textLayer_work;
static TextLayer *textLayer_rest;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // textLayer_work
  textLayer_work = text_layer_create(GRect(0, 33, 120, 27));
  text_layer_set_background_color(textLayer_work, GColorClear);
  text_layer_set_text(textLayer_work, "Work Time:25");
  text_layer_set_text_alignment(textLayer_work, GTextAlignmentCenter);
  text_layer_set_font(textLayer_work, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)textLayer_work);
  
  // textLayer_rest
  textLayer_rest = text_layer_create(GRect(0, 101, 120, 27));
  text_layer_set_background_color(textLayer_rest, GColorClear);
  text_layer_set_text(textLayer_rest, "Rest Time:05");
  text_layer_set_text_alignment(textLayer_rest, GTextAlignmentCenter);
  text_layer_set_font(textLayer_rest, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)textLayer_rest);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(textLayer_work);
  text_layer_destroy(textLayer_rest);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){
  window_set_background_color(s_window, GColorCobaltBlue);
  text_layer_set_text_color(textLayer_rest, GColorWhite);
  text_layer_set_text_color(textLayer_work, GColorWhite);
}

static void changeWorkTime(ClickRecognizerRef recognizer, void *context){
  
}

static void changeRestTime(ClickRecognizerRef recognizer, void *context){
  int rest = persist_read_int(CONFIG_PAUSE_TIME);
  if (rest < 10){
    rest++;
  }else{
    rest = 1;
  }
  static char text[] = "";
  snprintf(text, 14, "Rest Time:%.02d", rest);
  text_layer_set_text(textLayer_rest, text);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", text);
  persist_write_int(CONFIG_PAUSE_TIME, rest);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, changeWorkTime);
  window_single_click_subscribe(BUTTON_ID_DOWN, changeRestTime);
}

void show_configwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_background_color(s_window, GColorCobaltBlue);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
  });
  window_stack_push(s_window, true);
}

void hide_configwindow(void) {
  window_stack_remove(s_window, true);
}
