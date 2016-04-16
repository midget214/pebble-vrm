
static void DrawSolarIcons(Layer *window_layer)
{ 
  // Solar Yield
  solar_background_bitmaps[0] = gbitmap_create_with_resource(RESOURCE_ID_SOLAR_YIELD);
  solar_background_layers[0] = bitmap_layer_create(GRect(20, 44, 16, 16));
  bitmap_layer_set_bitmap(solar_background_layers[0], solar_background_bitmaps[0]);
  layer_add_child(window_layer, bitmap_layer_get_layer(solar_background_layers[0]));
  
  // Battery SoC
  solar_background_bitmaps[1] = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_SOC);
  solar_background_layers[1] = bitmap_layer_create(GRect(65, 9, 16, 16));
  bitmap_layer_set_bitmap(solar_background_layers[1], solar_background_bitmaps[1]);
  layer_add_child(window_layer, bitmap_layer_get_layer(solar_background_layers[1]));
  
  // Load
  solar_background_bitmaps[2] = gbitmap_create_with_resource(RESOURCE_ID_LOAD);
  solar_background_layers[2] = bitmap_layer_create(GRect(104, 45, 16, 16));
  bitmap_layer_set_bitmap(solar_background_layers[2], solar_background_bitmaps[2]);
  layer_add_child(window_layer, bitmap_layer_get_layer(solar_background_layers[2]));
}

static void SetSolarTextProperties(TextLayer * layer, GTextAlignment alignment)
{
  text_layer_set_background_color(layer, GColorBlack);
  text_layer_set_text_color(layer, GColorLightGray);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(layer, alignment);
}

static void UpdateSolarText()
{
   text_layer_set_text(text_layers[yield_today_layer], cache.yield);
   text_layer_set_text(text_layers[soc_layer], cache.soc);
   text_layer_set_text(text_layers[bat_state_layer], cache.state);
   text_layer_set_text(text_layers[load_layer], cache.load);
}

static void DrawSolarText(Layer *window_layer)
{
  // Solar Yield
  TextLayer * text_layer = (text_layers[yield_today_layer] = text_layer_create(GRect(38, 42, 54, 16)));
  SetSolarTextProperties(text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  // Battery SoC / State
  text_layer = (text_layers[soc_layer] = text_layer_create(GRect(82, 8, 50, 16)));
  SetSolarTextProperties(text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  text_layer = (text_layers[bat_state_layer] = text_layer_create(GRect(0, 24, s_main_window_bounds.size.w, 16)));
  SetSolarTextProperties(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  // Load
  text_layer = (text_layers[load_layer] = text_layer_create(GRect(123, 43, 50, 16)));
  SetSolarTextProperties(text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  UpdateSolarText();
}

static void DrawDateTime(Layer* window_layer)
{
  // Time
  TextLayer * text_layer = (text_layers[time_layer] = text_layer_create(GRect(0, 55, s_main_window_bounds.size.w, 50)));
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  // Date
  text_layer = (text_layers[date_layer] = text_layer_create(GRect(0, 105, s_main_window_bounds.size.w, 20)));
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorVividCerulean);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void UpdateBatteryLevel()
{
  if (s_battery_level == 100)
    bitmap_layer_set_bitmap(battery_bitmap_layer, battery_bitmaps[0]);
  else if (s_battery_level <= 100 && s_battery_level > 75)
    bitmap_layer_set_bitmap(battery_bitmap_layer, battery_bitmaps[1]);
  else if (s_battery_level <= 75 && s_battery_level > 25)
    bitmap_layer_set_bitmap(battery_bitmap_layer, battery_bitmaps[2]);
  else if (s_battery_level <= 25)
    bitmap_layer_set_bitmap(battery_bitmap_layer, battery_bitmaps[3]);
}

static void DrawBattery(Layer* window_layer)
{
  battery_bitmaps[0] = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_100);
  battery_bitmaps[1] = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_75);
  battery_bitmaps[2] = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_50);
  battery_bitmaps[3] = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_25);
  
  battery_bitmap_layer = bitmap_layer_create(GRect(10, 77, 12, 21));
  layer_add_child(window_layer, bitmap_layer_get_layer(battery_bitmap_layer));
}

static void DrawVictronLogo(Layer* window_layer)
{
  victron_logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VICTRON_LOGO);
  victron_logo_bitmap_layer = bitmap_layer_create(GRect(45, 145, 91, 20));
  bitmap_layer_set_bitmap(victron_logo_bitmap_layer, victron_logo_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(victron_logo_bitmap_layer));
}
