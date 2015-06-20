#include <pebble.h>
#include "windows/mainwindow.h"
#include "installation.h"
#include "constantes.h"
    
static void init(void) {
  if (!persist_exists(INSTALLED)){
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