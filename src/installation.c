#include <pebble.h>
#include "installation.h"
#include "constantes.h"

void installConfig(){
  persist_write_int(CONFIG_WORK_TIME, 25);
  persist_write_int(CONFIG_PAUSE_TIME, 5);
  persist_write_bool(INSTALLED, true);
}