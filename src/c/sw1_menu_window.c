#include <pebble.h>
#include "main_header.h"



Window *sw1MenuWindow;
MenuLayer *sw1MenuLayer;

uint16_t sw1_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;

}

uint16_t sw1_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch(section_index){
    case 0:
      return 3;
    case 1:
      return 1;
    default: 
      return 0;
  }
  
}

int16_t sw1_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void sw1_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch(section_index){
      case 0:
        menu_cell_basic_header_draw(ctx, cell_layer, "Chronograph Menu");
        break;
    }
}

void sw1_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch(cell_index->section){
      case 0:
        switch(cell_index->row){
          case 0:
            menu_cell_basic_draw(ctx, cell_layer, "Current Stopwatch", NULL, NULL);
            break;
          case 1:
            menu_cell_basic_draw(ctx, cell_layer, "Save Stopwatch", NULL, NULL);
            break;
          case 2:
            menu_cell_basic_draw(ctx, cell_layer, "Saved Workouts", NULL, NULL);
            break;
        }
        break;
    }
}

void sw1_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
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
            splash_window_create();
            window_stack_push(splash_window_get_window(), true);
          }
          break;
        case 1:
          
          break;
        case 2:
          
          break;
  }
	
  }
}

static void sw1_click_config_provider(void *context){
  /*ButtonId selID = BUTTON_ID_SELECT;  // The Select button
  ButtonId upID = BUTTON_ID_UP;//The Up Button*/
  ButtonId backID = BUTTON_ID_BACK;
  /*window_single_click_subscribe(selID, select_click_handler);
  window_single_click_subscribe(upID, up_click_handler);*/
  window_single_click_subscribe(backID, sw1_back_click_handler);
}

static void sw1_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_push(menu_window_get_window(), true);
}

void sw1_setup_menu_layer(Window *window) {
	Layer *window_layer = window_get_root_layer(window);

  sw1MenuLayer = menu_layer_create(GRect(0, 0, 144, 168));
  menu_layer_set_callbacks(sw1MenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = sw1_menu_get_num_sections_callback,
    .get_num_rows = sw1_menu_get_num_rows_callback,
    .get_header_height = sw1_menu_get_header_height_callback,
    .draw_header = sw1_menu_draw_header_callback,
    .draw_row = sw1_menu_draw_row_callback,
    .select_click = sw1_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(sw1MenuLayer, window);

  layer_add_child(window_layer, menu_layer_get_layer(sw1MenuLayer));
    
  //window_set_click_config_provider(sw1MenuWindow, sw1_click_config_provider);
}

void sw1_menu_window_load(Window *window){
  sw1_setup_menu_layer(window);
}

void sw1_menu_window_unload(Window *window){
  menu_layer_destroy(sw1MenuLayer);
  
}

void sw1_menu_window_create(){
  sw1MenuWindow = window_create();
  window_set_window_handlers(sw1MenuWindow, (WindowHandlers){
    .load = sw1_menu_window_load,
    .unload = sw1_menu_window_unload
  });
}

void sw1_menu_window_destroy(){
  window_destroy(sw1MenuWindow);
}

Window *sw1_menu_window_get_window(){
  return sw1MenuWindow;
}