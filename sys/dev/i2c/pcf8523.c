#include <stdio.h>
#include <dev/clock_subr.h>
#include <dev/i2c/pcf8523reg.h>

/*
Used to provide a clock interface with RTC 8523
*/

void todr_init(){
  return;
}

void todr_attach(todr_chip_handle_t attachPointer){
  return;
}

void todr_lock(){
  return;
}

void todr_unlock(){
  return;
}

bool todr_lock_owned(){
  return;
}

void todr_set_systime(time_t base){
  return;
}

void todr_save_systime(){
  return;
}
