#pragma once
#define FW_SINGLE 1
#define FW_SINGLE_REST 2
#define FW_DEC 3
#define FW_ACC 4
#define FW_CUST 5

int curFunc, noIntervals;

void return_to_menu(void);
void next_window(int next);

typedef struct{
  int numIntervals;
  int rest[3];
  int intervals[30][3];
  char* text;
  int type;
}TimerInfo;

TimerInfo *curTimer;

void splash_window_load(Window *window);
void splash_window_unload(Window *window);
void splash_window_create();
void splash_window_destroy();
//static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
//static void updateTimer();
//static void click_config_provider(void *context);
//static void select_click_handler(ClickRecognizerRef recognizer, void *context);
//static void up_click_handler(ClickRecognizerRef recognizer, void *context);
//static void back_click_handler(ClickRecognizerRef recognizer, void *context);
//static void addLap();
char * convertNumberIntoArray(unsigned int number);
//static void stopTimer();
Window *splash_window_get_window();

void timer_window_load(Window *window);
void timer_window_unload(Window *window);
void timer_window_create();
void timer_window_destroy();
Window *timer_window_get_window();

//Menu Window functions
void menu_window_load(Window *window);
void menu_window_unload(Window *window);
void menu_window_create();
void menu_window_destroy();

Window *menu_window_get_window();

//Stopwatch Menu 1 functions
static void sw1_back_click_handler(ClickRecognizerRef recognizer, void *context);
void sw1_menu_window_load(Window *window);
void sw1_menu_window_unload(Window *window);
void sw1_menu_window_create();
void sw1_menu_window_destroy();
Window *sw1_menu_window_get_window();

//Fartlek Menu 1 Functions
void fw1_menu_window_load(Window *window);
void fw1_menu_window_unload(Window *window);
void fw1_menu_window_create();
void fw1_menu_window_destroy();
Window *fw1_menu_window_get_window();

//Fartlek Custom Workout Menu Functions
void fwCust_menu_window_load(Window *window);
void fwCust_menu_window_unload(Window *window);
void fwCust_menu_window_create();
void fwCust_menu_window_destroy();
Window *fwCust_menu_window_get_window();

//Interval Workout Menu 1
void iw1_menu_window_load(Window *window);
void iw1_menu_window_unload(Window *window);
void iw1_menu_window_create();
void iw1_menu_window_destroy();
Window *iw1_menu_window_get_window();

void iwCust_menu_window_load(Window *window);
void iwCust_menu_window_unload(Window *window);
void iwCust_menu_window_create();
void iwCust_menu_window_destroy();
Window *iwCust_menu_window_get_window();
