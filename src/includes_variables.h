#include <pebble.h>

static Window *s_main_window;
static GRect s_main_window_bounds;

#define solar_bitmap_count 3
static GBitmap* solar_background_bitmaps[solar_bitmap_count];
static BitmapLayer* solar_background_layers[solar_bitmap_count];

#define battery_bitmap_count 4
static GBitmap* battery_bitmaps[battery_bitmap_count];
static BitmapLayer* battery_bitmap_layer;
static int s_battery_level;

static GBitmap* victron_logo_bitmap;
static BitmapLayer* victron_logo_bitmap_layer;

#define text_layers_count 6
#define yield_today_layer 0
#define soc_layer 1
#define bat_state_layer 2
#define load_layer 3
#define time_layer 4
#define date_layer 5

static TextLayer *text_layers[text_layers_count];

