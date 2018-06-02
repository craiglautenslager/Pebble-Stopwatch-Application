#include <pebble.h>
#include "main_header.h"
#include "pin_window.h"


Window *fwCustMenuWindow;
MenuLayer *fwCustMenuLayer;
PinWindowCallbacks pincall;
PinWindow *newPin;

uint16_t fwCust_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;

}

uint16_t fwCust_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch(section_index){
    case 0:
      return 5;
    case 1:
      return 1;
    default: 
      return 0;
  }
  
}

int16_t fwCust_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void fwCust_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch(section_index){
      case 0:
        menu_cell_basic_header_draw(ctx, cell_layer, "Select Workout Format");
        break;
    }
}

void fwCust_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch(cell_index->section){
      case 0:
        switch(cell_index->row){
          case 0:
            menu_cell_basic_draw(ctx, cell_layer, "Same Time", "Repeat One Time", NULL);
            break;
          case 1:
            menu_cell_basic_draw(ctx, cell_layer, "Same Time", "Plus Rest", NULL);
            break;
          case 2:
            menu_cell_basic_draw(ctx, cell_layer, "Decending", "Time Decreases", NULL);
            break;
          case 3:
            menu_cell_basic_draw(ctx, cell_layer, "Accending", "Time Imcreases", NULL);
            break;
          case 4:
            menu_cell_basic_draw(ctx, cell_layer, "Custom Timer", "Set every Interval", NULL);

        }
        break;
    }
}

void fwCust_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch(cell_index->section){
    case 0:
      switch(cell_index->row){
        case 0:
          if (window_stack_contains_window(splash_window_get_window())){
            window_stack_remove(menu_window_get_window(), true);
            window_stack_pop(true);
          }
          else{
            window_stack_remove(menu_window_get_window(), true);
            window_stack_pop(true);
          }
          curTimer = (TimerInfo*)malloc(sizeof(TimerInfo));
          curTimer->numIntervals = 1;
          curTimer->rest[2] = 0;
          curTimer->text = "Single Interval";
          PinInfo *newInfo = (PinInfo*)malloc(sizeof(PinInfo));
          newInfo->numCells = 1;
          newInfo->upperText = "Select Reps";
          newInfo->lowerText = "How many Intervals are you doing";
          newInfo->background = RESOURCE_ID_REP_NUM_BACK;
          newInfo->nextWindow = NUM_FW_INT;
          newPin  = pin_window_create(pincall, newInfo);
          window_stack_push(newPin->window, true);
        
          break;
        case 1:
          
          break;
        case 2:
          
          break;
  }
	
  }
}

static void fwCust_click_config_provider(void *context){
  /*ButtonId selID = BUTTON_ID_SELECT;  // The Select button
  ButtonId upID = BUTTON_ID_UP;//The Up Button*/
  ButtonId backID = BUTTON_ID_BACK;
  /*window_single_click_subscribe(selID, select_click_handler);
  window_single_click_subscribe(upID, up_click_handler);*/
  //window_single_click_subscribe(backID, fwCust_back_click_handler);
}

static void fwCust_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(menu_window_get_window(), true);
  window_stack_push(fw1_menu_window_get_window(), true);
}

void fwCust_setup_menu_layer(Window *window) {
	Layer *window_layer = window_get_root_layer(window);

  fwCustMenuLayer = menu_layer_create(GRect(0, 0, 144, 168));
  menu_layer_set_callbacks(fwCustMenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = fwCust_menu_get_num_sections_callback,
    .get_num_rows = fwCust_menu_get_num_rows_callback,
    .get_header_height = fwCust_menu_get_header_height_callback,
    .draw_header = fwCust_menu_draw_header_callback,
    .draw_row = fwCust_menu_draw_row_callback,
    .select_click = fwCust_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(fwCustMenuLayer, window);

  layer_add_child(window_layer, menu_layer_get_layer(fwCustMenuLayer));
    
  
}

void fwCust_menu_window_load(Window *window){
  fwCust_setup_menu_layer(window);
}

void fwCust_menu_window_unload(Window *window){
  menu_layer_destroy(fwCustMenuLayer);
  
}

void fwCust_menu_window_create(){
  fwCustMenuWindow = window_create();
  window_set_window_handlers(fwCustMenuWindow, (WindowHandlers){
    .load = fwCust_menu_window_load,
    .unload = fwCust_menu_window_unload
  });
}

void fwCust_menu_window_destroy(){
  window_destroy(fwCustMenuWindow);
}

Window *fwCust_menu_window_get_window(){
  return fwCustMenuWindow;
}