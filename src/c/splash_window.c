#include <pebble.h>
#include <math.h>
#include "main_header.h"
#include <string.h>

#define true 1
#define false 0

Window *splashWindow;
GBitmap *splashSWRun;
GBitmap *splashSWPause;
BitmapLayer *splashImageLayer;
TextLayer *splashImageTitle;
TextLayer *splashTimeText;
TextLayer *splashSplitText;
TextLayer *splashLap1Text;
TextLayer *splashLap2Text;
TextLayer *splashLap3Text;
TextLayer *splashLap4Text;
TextLayer *splashLap5Text;
TextLayer *splashLap6Text;

time_t startTime;
uint16_t STmilli;

time_t lastSplit;
uint16_t LSPmilli;

time_t thisSplit;
uint16_t TSPmilli;

time_t lastStop;
uint16_t LSmilli;

time_t pauseTime;
uint16_t PTmilli;

time_t totalPause;
uint16_t PAmilli;

time_t splitPause;

//static char yes[] = "yes", no[] = "no";
char SW_data[30][24];
int run = false, clear = true, curLap = 1, curSlot = 1;

// Get a tm structure
struct tm *tick_time;
struct tm *split_time;
struct tm *lap_time;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void updateTimer();
static void stopTimer();
static void click_config_provider(void *context);
static void back_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void addLap();
int isRun();



void splash_window_load(Window *window){
  //Create the parent window layer
  Layer *window_layer = window_get_root_layer(window);
  
  //create a bitmap layer for the background, convert it to a generic layer and
  //add it as a child to the parent layer
  splashSWRun = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SW_RUN);
  splashSWPause =  gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SW_PAUSE);
  splashImageLayer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(splashImageLayer, splashSWPause);
  bitmap_layer_set_compositing_mode(splashImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(splashImageLayer));
  //create a text layer for the 'laps' text, convert it to a generic layer and
  //add it as a child to the parent layer
  splashImageTitle = text_layer_create(GRect(0, 82, 144, 20));
  text_layer_set_text(splashImageTitle, " #|       LAP        |         SPLIT");
  text_layer_set_text_alignment(splashImageTitle, GTextAlignmentLeft);
  text_layer_set_background_color(splashImageTitle, GColorClear);
  text_layer_set_text_color(splashImageTitle, GColorBlack);
  text_layer_set_font(splashImageTitle, fonts_get_system_font(FONT_KEY_GOTHIC_09));
  layer_add_child(window_layer, text_layer_get_layer(splashImageTitle));
  //create a text layer that displays the clock, convert it to a generic layer and
  //add it as a child to the parent layer
  splashTimeText = text_layer_create(GRect(0,-13, 138, 52));
  text_layer_set_text(splashTimeText, "00:00:00:--");
  text_layer_set_font(splashTimeText, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DELIRIUM_52)));
  text_layer_set_text_alignment(splashTimeText, GTextAlignmentLeft);
  text_layer_set_text_color(splashTimeText, GColorWhite);
  text_layer_set_background_color(splashTimeText, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(splashTimeText));
  
  splashSplitText = text_layer_create(GRect(0,28, 144, 52));
  text_layer_set_text(splashSplitText, "00:00:00:--");
  text_layer_set_font(splashSplitText, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DELIRIUM_52)));
  text_layer_set_text_alignment(splashSplitText, GTextAlignmentLeft);
  text_layer_set_text_color(splashSplitText, GColorWhite);
  text_layer_set_background_color(splashSplitText, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(splashSplitText));
  //create a text layer that displays the last lap, convert it to a generic layer and
  //add it as a child to the parent layer
  splashLap1Text = text_layer_create(GRect(0, 88, 144, 18));
  text_layer_set_text(splashLap1Text, "");
  text_layer_set_font(splashLap1Text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(splashLap1Text, GColorClear);
  text_layer_set_text_color(splashLap1Text, GColorBlack);
  text_layer_set_text_alignment(splashLap1Text, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(splashLap1Text));
  
  splashLap2Text = text_layer_create(GRect(0, 100, 144, 18));
  text_layer_set_text(splashLap2Text, "");
  text_layer_set_font(splashLap2Text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(splashLap2Text, GColorClear);
  text_layer_set_text_color(splashLap2Text, GColorBlack);
  text_layer_set_text_alignment(splashLap2Text, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(splashLap2Text));
  
  splashLap3Text = text_layer_create(GRect(0, 112, 144, 18));
  text_layer_set_text(splashLap3Text, "");
  text_layer_set_font(splashLap3Text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(splashLap3Text, GColorClear);
  text_layer_set_text_color(splashLap3Text, GColorBlack);
  text_layer_set_text_alignment(splashLap3Text, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(splashLap3Text));
  
  splashLap4Text = text_layer_create(GRect(0, 124, 144, 18));
  text_layer_set_text(splashLap4Text, "");
  text_layer_set_font(splashLap4Text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(splashLap4Text, GColorClear);
  text_layer_set_text_color(splashLap4Text, GColorBlack);
  text_layer_set_text_alignment(splashLap4Text, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(splashLap4Text));
  
  splashLap5Text = text_layer_create(GRect(0, 136, 144, 18));
  text_layer_set_text(splashLap5Text, "");
  text_layer_set_font(splashLap5Text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(splashLap5Text, GColorClear);
  text_layer_set_text_color(splashLap5Text, GColorBlack);
  text_layer_set_text_alignment(splashLap5Text, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(splashLap5Text));
  
  splashLap6Text = text_layer_create(GRect(0, 148, 144, 18));
  text_layer_set_text(splashLap6Text, "");
  text_layer_set_font(splashLap6Text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(splashLap6Text, GColorClear);
  text_layer_set_text_color(splashLap6Text, GColorBlack);
  text_layer_set_text_alignment(splashLap6Text, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(splashLap6Text));
}

//Update the reading on the main clock when this is function is called by calling the update function
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  
  updateTimer();
}

//Function that updates the main clock
static void updateTimer(){
  
  time_t curTime = time (NULL);
  time_t temp = difftime(curTime, startTime) - totalPause;
  tick_time = gmtime(&temp);
  
  static char s_buffer[12];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M:%S:--" : "%I:%M:%S:--", tick_time);
  
  text_layer_set_text(splashTimeText, s_buffer);
}

static void stopTimer(){

  int tempMilli = PTmilli - PAmilli;
  //static char test[10];
    //snprintf(test, 10, "%d", tempMilli);
    //text_layer_set_text(splashLap1Text, test);

  if(tempMilli < 0){
    totalPause += 1;
    //text_layer_set_text(splashLap2Text, yes);
    tempMilli += 1000;
  }
  //tempMilli += LSmilli;

  /*if(tempMilli > 999){
    totalPause += 1;
    tempMilli -= 1000;
    text_layer_set_text(splashLap2Text, no);
  }*/
  time_t temp = difftime(pauseTime, startTime) - totalPause;
  tick_time = gmtime(&temp);
  

  uint16_t hund = tempMilli / 10;
  static char string[20];
   
  static char s_buffer[10];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M:%S:" : "%I:%M:%S:", tick_time);
  snprintf(string,20, "%s%u", s_buffer, hund);
  text_layer_set_text(splashTimeText, string);
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
  window_stack_push(sw1_menu_window_get_window(), true);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  if(run == true){
    thisSplit = time(NULL);
    TSPmilli = time_ms(NULL, NULL);

    int tempMilli = TSPmilli - LSPmilli;
    int splitMilli = TSPmilli - STmilli;
    time_t totPause = totalPause;

    if( splitMilli < 0){
      splitMilli += 1000;
      totPause += 1;
    }
    //static char test[10];
    //snprintf(test, 10, "%d", tempMilli);
    //text_layer_set_text(splashLap1Text, test);
   
    if(tempMilli < 0){
      tempMilli += 1000;
      splitPause += 1;
      //text_layer_set_text(splashLap2Text, no);
    }
    else if(tempMilli > 999){
      tempMilli -= 1000;
      //text_layer_set_text(splashLap2Text, yes);
    }
    time_t lap = difftime(thisSplit, lastSplit) - splitPause;
    time_t split = difftime(thisSplit, startTime) - totPause;
    
    lap_time = gmtime(&lap);
    
  

  uint16_t hund = tempMilli / 10;
  static char string1[12];
  uint16_t hund1 = splitMilli / 10;
  static char string2[24];
  
  static char l_buffer[9];
  strftime(l_buffer, sizeof(l_buffer), clock_is_24h_style() ?
                                          "%H:%M:%S" : "%I:%M:%S", lap_time);
  snprintf(string1,12, "%s:%u", l_buffer, hund);
  text_layer_set_text(splashSplitText, string1);

  split_time = gmtime(&split);

  static char s_buffer[9];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                         "%H:%M:%S:" : "%I:%M:%S:", split_time);

  snprintf(string2, 24, "%d|%8s|%s:%u", curLap, l_buffer, s_buffer, hund1);
  snprintf(SW_data[curLap-1], 24, "%s", string2);
  addLap();
  lastSplit = time(NULL);
  splitPause = 0;
  curLap++;

  }
  // A single click has just occured
  /*thisSplit = time(NULL);
  time_t split = difftime(thisSplit, lastSplit);
  lastSplit = time(NULL);
  split_time = gmtime(&split);
  // Write the current hours and minutes into a buffer
  static char s_buffer[9];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M:%S" : "%I:%M:%S", split_time);
  text_layer_set_text(splashLap1Text, s_buffer);*/
  
}


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  // A single click has just occured
  if(clear == true){
    startTime = time(NULL);
    STmilli = time_ms(NULL, NULL);

    lastSplit = time(NULL);
    LSPmilli = time_ms(NULL, NULL);
    
    pauseTime = time(NULL);
    PTmilli = time_ms(NULL, NULL);
    lastStop = time(NULL);
    LSmilli = time_ms(NULL, NULL);
    
    totalPause = 0;
    splitPause = 0;
    clear = false;
  }
  
  if(run == false){
    bitmap_layer_set_bitmap(splashImageLayer, splashSWRun);
    lastStop = time(NULL);
    LSmilli = time_ms(NULL,NULL);

    totalPause = difftime(lastStop, pauseTime) + totalPause;
    splitPause = difftime(lastStop, pauseTime) + splitPause;
    int tempMilli = LSmilli - PTmilli;
    if (tempMilli < 0){
      //totalPause -= 1;
      tempMilli += 1000;
    }
    PAmilli = tempMilli;
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    run = true;
  }
  else{
    tick_timer_service_unsubscribe();
    pauseTime = time(NULL);
    PTmilli = time_ms(NULL, NULL);

    /*thisSplit = time(NULL);
    TSPmilli = time_ms(NULL, NULL);*/
    
    bitmap_layer_set_bitmap(splashImageLayer, splashSWPause);
    stopTimer();
    run = false;
  }
}

static void addLap(){
  
  if (curSlot == 1){
    text_layer_set_text(splashLap1Text, SW_data[curLap-1]);
    curSlot++;
  }
  else if (curSlot == 2){
    text_layer_set_text(splashLap2Text, SW_data[curLap-2]);
    text_layer_set_text(splashLap1Text, SW_data[curLap-1]);
    curSlot++;
  }
  else if (curSlot == 3){
    text_layer_set_text(splashLap3Text, SW_data[curLap-3]);
    text_layer_set_text(splashLap2Text, SW_data[curLap-2]);
    text_layer_set_text(splashLap1Text, SW_data[curLap-1]);
    curSlot++;
  }
  else if (curSlot == 4){
    text_layer_set_text(splashLap4Text, SW_data[curLap-4]);
    text_layer_set_text(splashLap3Text, SW_data[curLap-3]);
    text_layer_set_text(splashLap2Text, SW_data[curLap-2]);
    text_layer_set_text(splashLap1Text, SW_data[curLap-1]);
    curSlot++;
  }
  else if (curSlot == 5){
    text_layer_set_text(splashLap5Text, SW_data[curLap-5]);
    text_layer_set_text(splashLap4Text, SW_data[curLap-4]);
    text_layer_set_text(splashLap3Text, SW_data[curLap-3]);
    text_layer_set_text(splashLap2Text, SW_data[curLap-2]);
    text_layer_set_text(splashLap1Text, SW_data[curLap-1]);
    curSlot++;
  }
  else if (curSlot == 6){
    text_layer_set_text(splashLap6Text, SW_data[curLap-6]);
    text_layer_set_text(splashLap5Text, SW_data[curLap-5]);
    text_layer_set_text(splashLap4Text, SW_data[curLap-4]);
    text_layer_set_text(splashLap3Text, SW_data[curLap-3]);
    text_layer_set_text(splashLap2Text, SW_data[curLap-2]);
    text_layer_set_text(splashLap1Text, SW_data[curLap-1]);
  }
}

void splash_window_unload(Window *window){
  gbitmap_destroy(splashSWRun);
  gbitmap_destroy(splashSWPause);
  bitmap_layer_destroy(splashImageLayer);
  text_layer_destroy(splashImageTitle);
  text_layer_destroy(splashTimeText);
  text_layer_destroy(splashLap1Text);
}

void splash_window_create(){
  splashWindow = window_create();
  
  window_set_window_handlers(splashWindow, (WindowHandlers){
    .load = splash_window_load,
    .unload = splash_window_unload
  });
  
  startTime = time(NULL);
  
  // Use this provider to add button click subscriptions
  window_set_click_config_provider(splashWindow, click_config_provider);
}

void splash_window_destroy(){
  window_destroy(splashWindow);
  
}

Window *splash_window_get_window(){
  return splashWindow;
}

char * splashconvertNumberIntoArray(unsigned int number) {
    unsigned int length = (int)(log10((float)number)) + 1;
    char * arr = (char *) malloc(length * sizeof(char)), * curr = arr;
    do {
    	*curr++ = number % 10;
    	number /= 10;
    } while (number != 0);
    return arr;
}

int isRun(){
  return run;
}