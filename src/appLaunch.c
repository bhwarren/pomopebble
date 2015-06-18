#include <pebble.h>
#include "mainwindow.h"
#include "installation.h"
#include "constants.h"
  
  
static void init(void) {
  if (persist_read_bool(INSTALLED)){
    installConfig();
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