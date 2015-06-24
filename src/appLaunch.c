#include <pebble.h>
#include "windows/mainwindow.h"
#include "constantes.h"
    
static void init(void) {
	if(!persist_exists(CONFIG_WORK)){
		persist_write_int(CONFIG_WORK,25);
		persist_write_int(CONFIG_REST,5);
	}
	show_mainwindow();
}

static void deinit(void) {
  hide_mainwindow();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
