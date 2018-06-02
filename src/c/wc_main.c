#include <pebble.h>
#include "main_header.h"
#include "pin_window.h"

PinWindowCallbacks pincall;
PinWindow *newPin;

void launch_menu_window(void *data){
  window_stack_push(menu_window_get_window(), true);
}

int main(void){
  
  menu_window_create();
  launch_menu_window(NULL);
  curFunc = 0;
  //app_timer_register(1500, launch_menu_window, NULL);
  
  app_event_loop();
  menu_window_destroy();
}

void return_to_menu(void){
  if(curFunc == 1){
    window_stack_push(menu_window_get_window(), true);
    window_stack_push(fw1_menu_window_get_window(), true);
    window_stack_push(fwCust_menu_window_get_window(), true);
  }
}

void next_window(int next){
  if(next == NUM_FW_INT){
    curTimer->type = 1;
    noIntervals = 0;
    PinInfo *newInfo = (PinInfo*)malloc(sizeof(PinInfo));
    newInfo->numCells = 3;
    newInfo->upperText = "Interval Length";
    newInfo->lowerText = "Enter the length of time in each Interval";
    newInfo->nextWindow = TIMER_WIN;
    newInfo->background = RESOURCE_ID_IMAGE_SELECT_BACK;
    newPin = pin_window_create(pincall, newInfo);
    window_stack_push(newPin->window, true);
  }
  else if(next == TIMER_WIN){
    timer_window_create();
    curTimer->text = "Same Time";
    window_stack_push(timer_window_get_window(), true);
  }
  
}