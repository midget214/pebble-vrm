#include "includes_variables.h"
#include "cache_controller.h"
#include "draw_commands.h"
#include "app_message.h"

static void UpdateTime(struct tm *tick_time)
{
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(text_layers[time_layer], s_buffer);
  
   // Copy date into buffer from tm structure
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);

  // Show the date
  text_layer_set_text(text_layers[date_layer], date_buffer);
  
  if(tick_time->tm_min % 30 == 0) 
  {
    RequestVRMData();
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
  UpdateTime(tick_time);
}

static void battery_callback(BatteryChargeState state) 
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Checking battery state");
  s_battery_level = state.charge_percent;
  UpdateBatteryLevel();
}

static void main_window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  s_main_window_bounds = layer_get_bounds(window_layer);
  
  if (!RetrieveCachedData())
  {
     InitCacheValues();
  }
  
  DrawDateTime(window_layer);
  DrawBattery(window_layer);
  DrawSolarIcons(window_layer);
  DrawSolarText(window_layer);
  DrawVictronLogo(window_layer);
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  UpdateTime(tick_time);
  
  battery_callback(battery_state_service_peek());
}

static void main_window_unload(Window *window) 
{
  if (writeToCache)
  {
    SaveCacheData();
  }
  
  for (int i = 0; i < solar_bitmap_count; ++i)
  {
      gbitmap_destroy(solar_background_bitmaps[i]);
      bitmap_layer_destroy(solar_background_layers[i]);
  }
  
  for (int i = 0; i < text_layers_count; ++i)
  {
      text_layer_destroy(text_layers[i]);
  }
  
  for (int i = 0; i < battery_bitmap_count; ++i)
  {
    gbitmap_destroy(battery_bitmaps[i]);
  }
  bitmap_layer_destroy(battery_bitmap_layer);
  
  gbitmap_destroy(victron_logo_bitmap);
  bitmap_layer_destroy(victron_logo_bitmap_layer); 
}

static void init() 
{ 
  ValidateCache();
  
  // Register callbacks
  register_app_message_callbacks();
  
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) 
  {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_callback);
}

static void deinit() 
{
  window_destroy(s_main_window);
}

int main(void) 
{  
  init();
  app_event_loop();
  deinit();
}