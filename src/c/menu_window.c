#include <pebble.h>

#include "main_header.h"

Window *menuWindow;
MenuLayer *mainMenuLayer;

uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  return 1;
}

uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch(section_index){
    case 0:
      return 3;
    case 1:
      return 1;
    default:
      return 0;
  }
  
}

int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch(section_index){
      case 0:
        menu_cell_basic_header_draw(ctx, cell_layer, "Wrist Coach Menu");
        break;
    }
}

void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch(cell_index->section){
      case 0:
        switch(cell_index->row){
          case 0:
            menu_cell_basic_draw(ctx, cell_layer, "Chronograph", NULL, NULL);
            break;
          case 1:
            menu_cell_basic_draw(ctx, cell_layer, "Fartlek Watch", NULL, NULL);
            break;
          case 2:
            menu_cell_basic_draw(ctx, cell_layer, "Interval Watch", NULL, NULL);
            break;
        }
        break;
    }
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch(cell_index->section){
    case 0:
      switch(cell_index->row){
        case 0:
          if(window_stack_contains_window(sw1_menu_window_get_window())){
            window_stack_push(sw1_menu_window_get_window(), true);
          }
          else{
            sw1_menu_window_create();
            window_stack_push(sw1_menu_window_get_window(), true);
          }
          break;
        case 1:
          if(window_stack_contains_window(fw1_menu_window_get_window())){
            window_stack_push(fw1_menu_window_get_window(), true);
          }
          else{
            fw1_menu_window_create();
            window_stack_push(fw1_menu_window_get_window(), true);
          }
          break;
        case 2:
          if(window_stack_contains_window(iw1_menu_window_get_window())){
            window_stack_push(iw1_menu_window_get_window(), true);
          }
          else{
            iw1_menu_window_create();
            window_stack_push(iw1_menu_window_get_window(), true);
          }
          break;
  }
	
  }
}
void setup_menu_layer(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  mainMenuLayer = menu_layer_create(GRect(0, 0, 144, 168));
  menu_layer_set_callbacks(mainMenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(mainMenuLayer, window);
  menu_layer_pad_bottom_enable(mainMenuLayer, true);

  layer_add_child(window_layer, menu_layer_get_layer(mainMenuLayer));
}

void menu_window_load(Window *window){
  setup_menu_layer(window);
}

void menu_window_unload(Window *window){
  menu_layer_destroy(mainMenuLayer);
  
}

void menu_window_create(){
  menuWindow = window_create();
  window_set_window_handlers(menuWindow, (WindowHandlers){
    .load = menu_window_load,
    .unload = menu_window_unload
  });
}

void menu_window_destroy(){
  window_destroy(menuWindow);
}

Window *menu_window_get_window(){
  return menuWindow;
}