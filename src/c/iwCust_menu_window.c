#include <pebble.h>
#include "main_header.h"
#include "pin_window.h"


Window *iwCustMenuWindow;
MenuLayer *iwCustMenuLayer;
PinWindowCallbacks pincall;
PinWindow *newPin;

uint16_t iwCust_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;

}

uint16_t iwCust_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch(section_index){
    case 0:
      return 6;
    case 1:
      return 1;
    default: 
      return 0;
  }
  
}

int16_t iwCust_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void iwCust_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch(section_index){
      case 0:
        menu_cell_basic_header_draw(ctx, cell_layer, "Select Workout Format");
        break;
    }
}

void iwCust_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch(cell_index->section){
      case 0:
        switch(cell_index->row){
          case 0:
            menu_cell_basic_draw(ctx, cell_layer, "Same Distance", "No Rest", NULL);
            break;
          case 1:
            menu_cell_basic_draw(ctx, cell_layer, "Same Distance", "Plus Rest", NULL);
            break;
          case 2:
            menu_cell_basic_draw(ctx, cell_layer, "Decending", "Dist Decreases", NULL);
            break;
          case 3:
            menu_cell_basic_draw(ctx, cell_layer, "Accending", "Dist Imcreases", NULL);
            break;
          case 4:
            menu_cell_basic_draw(ctx, cell_layer, "Create Ladder", "Accending and Decending", NULL);
            break;
          case 5:
            menu_cell_basic_draw(ctx, cell_layer, "Custom Workout", "Set Every Distance", NULL);
            break;

        }
        break;
    }
}

void iwCust_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
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
          //newPin  = pin_window_create(pincall);
          //window_stack_push(newPin->window, true);
        
          break;
        case 1:
          
          break;
        case 2:
          
          break;
  }
	
  }
}

static void iwCust_click_config_provider(void *context){
  /*ButtonId selID = BUTTON_ID_SELECT;  // The Select button
  ButtonId upID = BUTTON_ID_UP;//The Up Button*/
  ButtonId backID = BUTTON_ID_BACK;
  /*window_single_click_subscribe(selID, select_click_handler);
  window_single_click_subscribe(upID, up_click_handler);*/
  //window_single_click_subscribe(backID, iwCust_back_click_handler);
}

static void iwCust_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(menu_window_get_window(), true);
  window_stack_push(iw1_menu_window_get_window(), true);
}

void iwCust_setup_menu_layer(Window *window) {
	Layer *window_layer = window_get_root_layer(window);

  iwCustMenuLayer = menu_layer_create(GRect(0, 0, 144, 168));
  menu_layer_set_callbacks(iwCustMenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = iwCust_menu_get_num_sections_callback,
    .get_num_rows = iwCust_menu_get_num_rows_callback,
    .get_header_height = iwCust_menu_get_header_height_callback,
    .draw_header = iwCust_menu_draw_header_callback,
    .draw_row = iwCust_menu_draw_row_callback,
    .select_click = iwCust_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(iwCustMenuLayer, window);

  layer_add_child(window_layer, menu_layer_get_layer(iwCustMenuLayer));
    
  
}

void iwCust_menu_window_load(Window *window){
  iwCust_setup_menu_layer(window);
}

void iwCust_menu_window_unload(Window *window){
  menu_layer_destroy(iwCustMenuLayer);
  
}

void iwCust_menu_window_create(){
  iwCustMenuWindow = window_create();
  window_set_window_handlers(iwCustMenuWindow, (WindowHandlers){
    .load = iwCust_menu_window_load,
    .unload = iwCust_menu_window_unload
  });
}

void iwCust_menu_window_destroy(){
  window_destroy(iwCustMenuWindow);
}

Window *iwCust_menu_window_get_window(){
  return iwCustMenuWindow;
}