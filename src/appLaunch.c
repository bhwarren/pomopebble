#include <pebble.h>
#include "mainwindow.h"
#include "installation.h"
#include "constantes.h"
    
static void init(void) {
  if (!persist_exists(INSTALLED)){
    APP_LOG(APP_LOG_LEVEL_INFO, "Instalando");
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