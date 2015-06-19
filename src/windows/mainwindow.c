#include <pebble.h>
#include "mainwindow.h"
#include "constantes.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_28_bold;
static GBitmap *s_res_play_btn;
static GBitmap *s_res_cog_btn;
static GBitmap *s_res_reload_btn;
static TextLayer *s_textlayer_1;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  s_res_play_btn = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BTN);
  s_res_cog_btn = gbitmap_create_with_resource(RESOURCE_ID_COG_BTN);
  s_res_reload_btn = gbitmap_create_with_resource(RESOURCE_ID_RELOAD_BTN);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(18, 60, 100, 30));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "Lucas!!");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_cog_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_reload_btn);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_play_btn);
  gbitmap_destroy(s_res_cog_btn);
  gbitmap_destroy(s_res_reload_btn);
}
// END AUTO-GENERATED UI CODE

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {

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

void show_mainwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_mainwindow(void) {
  window_stack_remove(s_window, true);
}
