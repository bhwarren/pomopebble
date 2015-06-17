#include <pebble.h>
#include "PomoTimer.h"

static Window *window;
static TextLayer *text_layer;

int minutos = 25;
bool running = false;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  if(units_changed & SECOND_UNIT){
    APP_LOG(APP_LOG_LEVEL_INFO,"segundos");
  }
  if(units_changed & MINUTE_UNIT){
    APP_LOG(APP_LOG_LEVEL_INFO, "minutos");
  }
//     minutos--;    
//     static char str[] = "";
//     snprintf(str, 5, "%d", minutos);
//     text_layer_set_text(text_layer, str);
//     if (minutos == 0){
//       tick_timer_service_unsubscribe();
//       text_layer_set_text(text_layer, "25");
//     }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (running){
    tick_timer_service_unsubscribe();
    window_set_background_color(window, GColorDukeBlue);
    running = false;
    minutos = 25;
    text_layer_set_text(text_layer, "25");
  }else{
    window_set_background_color(window, GColorRed);
    running = true;
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);  
  }
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  char * str = texto();
  text_layer_set_text(text_layer, str);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorDukeBlue);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}