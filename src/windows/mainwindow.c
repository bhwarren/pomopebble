#include <pebble.h>
#include "mainwindow.h"
#include "../constantes.h"
#include "configwindow.h"

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
	
	
int m = 0;
int s = 0;
int worktime;
int resttime;
int counter = 0;
int mode = MODE_PAUSED;

static char timeText[10] = "";
static char counterText[10] = "";

static WakeupId s_wakeup_id;

static Window *s_window;
static StatusBarLayer *stat_bar;
static GFont s_res_bitham_42_light;
static GFont s_res_roboto_condensed_21;
static GBitmap *s_res_play_btn;
static GBitmap *s_res_x_btn;
static GBitmap *s_res_trash_btn;
static GBitmap *s_res_config_btn;
static TextLayer *timer_layer;
static TextLayer *counter_layer;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorClear);

  s_res_bitham_42_light = fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_play_btn = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BTN);
  s_res_x_btn = gbitmap_create_with_resource(RESOURCE_ID_X_BTN);
  s_res_trash_btn = gbitmap_create_with_resource(RESOURCE_ID_TRASH_BTN);
  s_res_config_btn = gbitmap_create_with_resource(RESOURCE_ID_CONFIG_BTN);  
	
  timer_layer = text_layer_create(GRect(-3, 45, 117, 61));
  text_layer_set_background_color(timer_layer, GColorClear);
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_font(timer_layer, s_res_bitham_42_light);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);

  counter_layer = text_layer_create(GRect(0, 90, 114, 25));
  text_layer_set_background_color(counter_layer, GColorClear);
  text_layer_set_text_alignment(counter_layer, GTextAlignmentCenter);
  text_layer_set_text_color(counter_layer, GColorWhite);
  text_layer_set_font(counter_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)counter_layer); 

  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_trash_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_config_btn);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
	
	stat_bar = status_bar_layer_create();
	layer_add_child(window_get_root_layer(s_window), (Layer*) stat_bar);

}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  text_layer_destroy(counter_layer);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_play_btn);
  gbitmap_destroy(s_res_x_btn);
  gbitmap_destroy(s_res_trash_btn);
  gbitmap_destroy(s_res_config_btn);
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
	GColor back_color;
	if( mode == MODE_RUNNING_PAUSE){
		back_color = GColorJaegerGreen;
	}else if( mode == MODE_RUNNING_WORK ){
		back_color = GColorDarkCandyAppleRed;
	}else{
		back_color = GColorCobaltBlue;
	}
  window_set_background_color(s_window, back_color);
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
	APP_LOG(APP_LOG_LEVEL_DEBUG,"%d,%d",m,s);
	//only show minutes to save battery
  if(m > 3){
	  snprintf(timeText, 7, "%d m", m);
		m--;
	//if we hit 3 min mark, switch to minutes+seconds
  }else if(m == 3){
    s = 59;
    m = 2;
  	tick_timer_service_subscribe(SECOND_UNIT, updateTimer);
		snprintf(timeText, 7, "%d:%.2d", m, s);
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
    //we've switched to the opposite now, so make sure we got back to minutes
	  if(m > 3){
		  tick_timer_service_subscribe(MINUTE_UNIT, updateTimer);
	  }

    s = 0;		
  //down to < 3 minutes
  }else{ 
    if(s == 0){
		  s = 59;
		  m--;
	  }else{
		  s--;
	  }
	  snprintf(timeText, 7, "%d:%.2d", m,s);
	
  }
  
  text_layer_set_text(timer_layer, timeText);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(mode == MODE_RUNNING_WORK || mode == MODE_RUNNING_PAUSE){
    mode = MODE_PAUSED;
		configDefaultUI();
    tick_timer_service_unsubscribe();
		
		if(worktime>9){
				snprintf(timeText, 7, "%.2d:00", worktime);
		}else{
				snprintf(timeText, 7, "%d:00", worktime);
		}		
		text_layer_set_text(timer_layer, timeText);
  
	}else{
    m = worktime;
    s = 0;
    mode = MODE_RUNNING_WORK;
    configRunningUI();
    tick_timer_service_subscribe(MINUTE_UNIT, updateTimer);  
  }
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context){
  updateCounter(true);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context){
	show_configwindow();
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context){

	//set timer 
	if (!wakeup_query(s_wakeup_id, NULL) && mode != MODE_PAUSED) {
		//give back the minute we already subtracted in the ticker handler
		if(m>3){
			m++;
		}

    // Current time + however many seconds is left
    time_t future_time = time(NULL) + m*60 + s;

    // Schedule wakeup event and keep the WakeupId
    s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
		persist_write_int(PREV_MODE, mode);
  } 
	
	//exit app
	window_stack_remove(s_window, true);

}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}


static void wakeup_handler(WakeupId id, int32_t reason) {
  // The app has awoken!
  // check what we're coming from
	int coming_from = persist_read_int(PREV_MODE);
	time_t future_time;
	
	//notify user of what finished and set timer for opposite, 
	vibes_short_pulse();
	if(coming_from == MODE_RUNNING_WORK){
		APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", "came from running, switching to rest");
		persist_write_int(PREV_MODE, MODE_RUNNING_PAUSE);
		future_time = time(NULL) + resttime*60;
	}else{
		APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", "came from rest, switching to running");
		persist_write_int(PREV_MODE, MODE_RUNNING_WORK);
		future_time = time(NULL) + worktime*60;
		
	}
	
	s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
	persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
	
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){		
	
	worktime = persist_read_int(CONFIG_WORK);
	resttime = persist_read_int(CONFIG_REST);
	
	//check if we have a scheduled wakeup
	s_wakeup_id = persist_read_int(PERSIST_KEY_WAKEUP_ID);
	if (wakeup_query(s_wakeup_id, NULL)) {
		
		//disable wakup
		time_t timestamp = 0;
    wakeup_query(s_wakeup_id, &timestamp);
		wakeup_cancel(s_wakeup_id);		
		
		//set the time left, 
    int seconds_remaining = timestamp - time(NULL);
		m = seconds_remaining / 60;
		s = seconds_remaining % 60;
		
		//continue countdown
		mode = persist_read_int(PREV_MODE);
		if( mode == MODE_RUNNING_PAUSE){
			configPauseUI();
	  }else{
			configRunningUI();
		}
		
    //subscribe to seconds and let them finish before going back to minutes
		tick_timer_service_subscribe(MINUTE_UNIT, updateTimer);  
		
	
	} else if(mode != MODE_PAUSED){
		if(m > 9){
				snprintf(timeText, 7, "%.2d m", m);
		}	else{
				if(m > 3){
					snprintf(timeText, 7, "%d m", m);
				}else{
					snprintf(timeText, 7, "%d:%.2d", m, s);
				}
		}
	}else{
		//show worktime, and let user start program
		if(worktime > 9){
				snprintf(timeText, 7, "%.2d:00", worktime);
		}else{
				snprintf(timeText, 7, "%d:00", worktime);
		}	
			APP_LOG(APP_LOG_LEVEL_DEBUG,"else: %s",timeText);

	}
	
	text_layer_set_text(timer_layer, timeText);
  counter = persist_read_int(DATA_COUNTER);
  snprintf(counterText, 10, "Done: %.2d", counter);
  text_layer_set_text(counter_layer, counterText);
}

void show_mainwindow(void) {
	// Subscribe to Wakeup API
  wakeup_service_subscribe(wakeup_handler);

  // Was this a wakeup launch?
  if (launch_reason() == APP_LAUNCH_WAKEUP) {
    // The app was started by a wakeup
    WakeupId id = 0;
    int32_t reason = 0;
		
		worktime = persist_read_int(CONFIG_WORK);
		resttime = persist_read_int(CONFIG_REST);

    // Get details and handle the wakeup
    wakeup_get_launch_event(&id, &reason);
    wakeup_handler(id, reason);
		//wakeup handler will log, then close on its own
  } else{

			initialise_ui();
			window_set_click_config_provider(s_window, click_config_provider);
			configDefaultUI();
			window_set_window_handlers(s_window, (WindowHandlers) {
				.unload = handle_window_unload,
				.appear = handle_window_appear,
			});
			window_stack_push(s_window, true);
	}
}

void hide_mainwindow(void) {
  window_stack_remove(s_window, true);
}
