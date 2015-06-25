#include <pebble.h>
#include "configwindow.h"
#include "../constantes.h"

int configWork;
int configRest;

static char workText[10] = "";
static char restText[10] = "";

static Window *s_window;
static GFont s_res_bitham_30_black;
static TextLayer *work_layer;
static TextLayer *rest_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorWhite);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
    s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    work_layer = text_layer_create(GRect(-1, 36, 146, 42));
    text_layer_set_text(work_layer, "Work: 25");
    text_layer_set_text_color(work_layer,GColorDarkCandyAppleRed);
    text_layer_set_background_color(work_layer,GColorClear);
    text_layer_set_text_alignment(work_layer, GTextAlignmentCenter);
    text_layer_set_font(work_layer, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)work_layer);

    rest_layer = text_layer_create(GRect(0, 91, 144, 42));
    text_layer_set_text(rest_layer, "Rest: 05");
    text_layer_set_text_color(rest_layer,GColorJaegerGreen);
    text_layer_set_background_color(rest_layer,GColorClear);
    text_layer_set_text_alignment(rest_layer, GTextAlignmentCenter);
    text_layer_set_font(rest_layer, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)rest_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(rest_layer);
  text_layer_destroy(work_layer);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context){
	if(configRest == 10){
		configRest = 0;
	}
	configRest++;
	snprintf(restText, 10, "Rest: %.2d", configRest);
	text_layer_set_text(rest_layer, restText);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context){
	if(configWork == 60){
		configWork = 0;
	}
	configWork+=5;
	snprintf(workText, 10, "Work: %.2d", configWork);
	text_layer_set_text(work_layer, workText);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_disappear(Window* window) {
  persist_write_int(CONFIG_WORK,configWork);
  persist_write_int(CONFIG_REST,configRest);
}

static void handle_window_appear(Window* window){
	configWork = persist_read_int(CONFIG_WORK);
	configRest = persist_read_int(CONFIG_REST);
	snprintf(workText, 10, "Work: %.2d", configWork);
	text_layer_set_text(work_layer, workText);
	snprintf(restText, 10, "Rest: %.2d", configRest);
	text_layer_set_text(rest_layer, restText);
}

void show_configwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
    .disappear = handle_window_disappear,
  });
  window_stack_push(s_window, true);
}

void hide_configwindow(void) {
  window_stack_remove(s_window, true);
}
