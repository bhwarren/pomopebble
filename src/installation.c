#include <pebble.h>
#include "installation.h"
#include "constantes.h"

void installConfig(){
  persist_write_int(CONFIG_WORK_TIME, 1);
  persist_write_int(CONFIG_PAUSE_TIME, 1);
  persist_write_int(CONFIG_P_COUNTER, 0);
  persist_write_bool(INSTALLED, true);
}