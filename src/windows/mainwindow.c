#include <pebble.h>
#include "mainwindow.h"
#include "../constantes.h"
#include "configwindow.h"
  
int m = 0;
int s = 0;
int worktime;
int resttime;
int counter = 0;
int mode = MODE_PAUSED;

static char timeText[10] = "";
static char counterText[10] = "";

static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_roboto_condensed_21;
static GBitmap *s_res_play_btn;
static GBitmap *s_res_x_btn;
static GBitmap *s_res_trash_btn;
static TextLayer *timer_layer;
static TextLayer *counter_layer;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorClear);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_play_btn = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BTN);
  s_res_x_btn = gbitmap_create_with_resource(RESOURCE_ID_X_BTN);
  s_res_trash_btn = gbitmap_create_with_resource(RESOURCE_ID_TRASH_BTN);

  timer_layer = text_layer_create(GRect(-3, 45, 117, 61));
  text_layer_set_background_color(timer_layer, GColorClear);
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_font(timer_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);

  counter_layer = text_layer_create(GRect(0, 90, 114, 25));
  text_layer_set_background_color(counter_layer, GColorClear);
  // text_layer_set_text(counter_layer,"Done: 00") ;
  text_layer_set_text_alignment(counter_layer, GTextAlignmentCenter);
  text_layer_set_text_color(counter_layer, GColorWhite);
  text_layer_set_font(counter_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)counter_layer); 

  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorClear);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_trash_btn);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  text_layer_destroy(counter_layer);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_play_btn);
  gbitmap_destroy(s_res_x_btn);
  gbitmap_destroy(s_res_trash_btn);
}

static void configRunningUI(){
  window_set_background_color(s_window, GColorDarkCandyAppleRed);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_x_btn);
}

static void configPauseUI(){
  window_set_background_color(s_window, GColorJaegerGreen);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_x_btn);
}

static void configDefaultUI(){
  window_set_background_color(s_window, GColorCobaltBlue);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
}

static void updateCounter(bool zerar){
  if(zerar){
    counter = 0;
  }else{
    counter++; 
  }
  snprintf(counterText, 10, "Done: %.2d", counter);
  text_layer_set_text(counter_layer, counterText);
  persist_write_int(DATA_COUNTER,counter);
}

static void updateTimer(struct tm *tick_time, TimeUnits units_changed){

  if(s == 0 && m > 0){
    s = 59;
    m--;
  }else if(s == 0 && m == 0){
    if(mode == MODE_RUNNING_WORK){
      m = resttime;
      configPauseUI();
      mode = MODE_RUNNING_PAUSE;
      vibes_short_pulse();
      light_enable_interaction();
      updateCounter(false);
    }else{
      mode = MODE_RUNNING_WORK;
      m = worktime;
      configRunningUI();
      vibes_short_pulse();
      light_enable_interaction();
    }
    s = 0;
    
  }else{
    s--;
  }
  
  snprintf(timeText, 7, "%.2d:%.2d", m,s);
  text_layer_set_text(timer_layer, timeText);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(mode == MODE_RUNNING_WORK || mode == MODE_RUNNING_PAUSE){
    configDefaultUI();
    tick_timer_service_unsubscribe();
    mode = MODE_PAUSED;
    snprintf(timeText, 7, "%.2d:00", worktime);
    text_layer_set_text(timer_layer, timeText);
  }else{
    m = worktime;
    s = 0;
    mode = MODE_RUNNING_WORK;
    configRunningUI();
    tick_timer_service_subscribe(SECOND_UNIT, updateTimer);  
  }
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context){
  updateCounter(true);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context){
	show_configwindow();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){
	worktime = persist_read_int(CONFIG_WORK);
	resttime = persist_read_int(CONFIG_REST);
	snprintf(timeText, 7, "%.2d:00", worktime);
	text_layer_set_text(timer_layer, timeText);

  counter = persist_read_int(DATA_COUNTER);
  snprintf(counterText, 10, "Done: %.2d", counter);
  text_layer_set_text(counter_layer, counterText);
}

void show_mainwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  configDefaultUI();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
  });
  window_stack_push(s_window, true);
}

void hide_mainwindow(void) {
  window_stack_remove(s_window, true);
}
