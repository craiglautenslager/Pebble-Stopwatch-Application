#include <pebble.h>
#include <math.h>
#include "main_header.h"
#include <string.h>

#define true 1
#define false 0

Window *timerWindow;
GBitmap *timerRun;
GBitmap *timerPause;
BitmapLayer *timerImageLayer;
TextLayer *timerImageTitle;
TextLayer *timerTimeText;
TextLayer *timerNextText;

//static char yes[] = "yes", no[] = "no";
//char TR_data[30][24];
int runT = false, clearT = true, noReps;
char timerText[10], nextTime[10];
int *start;

static void updateTimer();
static void back_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);

void timer_window_load(Window *window){
  //Create the parent window layer
  Layer *window_layer = window_get_root_layer(window);
  start = curTimer->intervals[0];
  noReps = curTimer->numIntervals;
  snprintf(timerText, 10, "%02d:%02d:%02d", start[0], start[1], start[2]);
  snprintf(nextTime, 10, "%02d:%02d:%02d", start[0], start[1], start[2]);
  //create a bitmap layer for the background, convert it to a generic layer and
  //add it as a child to the parent layer
  timerRun = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIMER_RUN);
  timerPause =  gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIMER_PAUSE);
  timerImageLayer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(timerImageLayer, timerPause);
  bitmap_layer_set_compositing_mode(timerImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(timerImageLayer));
  //create a text layer for the 'laps' text, convert it to a generic layer and
  //add it as a child to the parent layer
  timerImageTitle = text_layer_create(GRect(0, 2, 144, 20));
  text_layer_set_text(timerImageTitle, curTimer->text);
  text_layer_set_text_alignment(timerImageTitle, GTextAlignmentCenter);
  text_layer_set_background_color(timerImageTitle, GColorClear);
  text_layer_set_text_color(timerImageTitle, GColorWhite);
  text_layer_set_font(timerImageTitle, fonts_get_system_font(FONT_KEY_GOTHIC_09));
  layer_add_child(window_layer, text_layer_get_layer(timerImageTitle));
  //create a text layer that displays the clock, convert it to a generic layer and
  //add it as a child to the parent layer
  timerTimeText = text_layer_create(GRect(15,0, 138, 52));
  text_layer_set_text(timerTimeText, timerText);
  text_layer_set_font(timerTimeText, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DELIRIUM_52)));
  text_layer_set_text_alignment(timerTimeText, GTextAlignmentLeft);
  text_layer_set_text_color(timerTimeText, GColorWhite);
  text_layer_set_background_color(timerTimeText, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(timerTimeText));
  
  timerNextText = text_layer_create(GRect(15,50, 144, 52));
  text_layer_set_text(timerNextText, nextTime);
  text_layer_set_font(timerNextText, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DELIRIUM_52)));
  text_layer_set_text_alignment(timerNextText, GTextAlignmentLeft);
  text_layer_set_text_color(timerNextText, GColorWhite);
  text_layer_set_background_color(timerNextText, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(timerNextText));
  //create a text layer that displays the last lap, convert it to a generic layer and
  //add it as a child to the parent layer
  
}

//Update the reading on the main clock when this is function is called by calling the update function
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  
  updateTimer();
}

//Function that updates the main clock
static void updateTimer(){
  start[2]--;
  if(start[2]==0){
    if(start[1] != 0){
      start[2] = 59;
      start[1]--;
    }
  }
  if(start[1] == 0){
    if(start[0]!=0){
      start[1] = 59;
      start[0]--;
    }
  }
  snprintf(timerText, 10, "%02d:%02d:%02d", start[0], start[1], start[2]);
  
  if((start[0] <= 0)&&(start[1] <= 0)&&(start[2] <= 0)){
    if(1){
      noReps--;
      void vibes_short_pulse(void);
      start = curTimer->intervals[0];
      if(noReps != 0){
        snprintf(timerText, 10, "%02d:%02d:%02d", start[0], start[1], start[2]);
      }
    }
  }
  
  text_layer_set_text(timerTimeText, timerText);
}


static void click_config_provider(void *context) {
  // Subcribe to button click events here
  ButtonId selID = BUTTON_ID_SELECT;  // The Select button
  ButtonId upID = BUTTON_ID_UP;//The Up Button
  ButtonId backID = BUTTON_ID_BACK;
  window_single_click_subscribe(selID, select_click_handler);
  window_single_click_subscribe(upID, up_click_handler);
  window_single_click_subscribe(backID, back_click_handler);
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(menu_window_get_window(), true);
  window_stack_push(fw1_menu_window_get_window(), true);
  window_stack_push(fwCust_menu_window_get_window(), true);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  int test = 5;
}


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  // A single click has just occured
  if(clearT == true){
    
  }
  
  if(runT == false){
    bitmap_layer_set_bitmap(timerImageLayer, timerRun);
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    runT = true;
  }
  else{
    tick_timer_service_unsubscribe();
    
    bitmap_layer_set_bitmap(timerImageLayer, timerPause);
    //stopTimer();
    runT = false;
  }
}

void timer_window_unload(Window *window){
  gbitmap_destroy(timerRun);
  gbitmap_destroy(timerPause);
  bitmap_layer_destroy(timerImageLayer);
  text_layer_destroy(timerImageTitle);
  text_layer_destroy(timerTimeText);
}

void timer_window_create(){
  timerWindow = window_create();
  
  window_set_window_handlers(timerWindow, (WindowHandlers){
    .load = timer_window_load,
    .unload = timer_window_unload
  });
  
  
  
  // Use this provider to add button click subscriptions
  window_set_click_config_provider(timerWindow, click_config_provider);
}

void timer_window_destroy(){
  window_destroy(timerWindow);
  
}

Window *timer_window_get_window(){
  return timerWindow;
}

