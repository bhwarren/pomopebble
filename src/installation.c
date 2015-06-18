#include <pebble.h>
#include "installation.h"
#include "constants.h"
  
// const uint32_t INSTALLED = 0;
// const uint32_t CONFIG_WORK_TIME = 1;
// const uint32_t CONFIG_PAUSE_TIME = 2;

void installConfig(){
  persist_write_int(CONFIG_WORK_TIME, 25);
  persist_write_int(CONFIG_PAUSE_TIME, 5);
  persist_write_bool(INSTALLED, true);
}