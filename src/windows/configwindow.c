#include <pebble.h>
#include "configwindow.h"
#include "../constantes.h"

int configWork;
int configRest;
static Window *s_window;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorRed);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
}

static void destroy_ui(void) {
  window_destroy(s_window);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context){
	APP_LOG(APP_LOG_LEVEL_DEBUG,"down");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context){
	APP_LOG(APP_LOG_LEVEL_DEBUG,"up");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){

}

void show_configwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
  });
  window_stack_push(s_window, true);
}

void hide_configwindow(void) {
  window_stack_remove(s_window, true);
}
