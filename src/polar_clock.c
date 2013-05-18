/*

  Polar Clock watch w/ 5 Arcs

  Thanks to Team Pebble's Segment Six watchface...it was a big help!
  Most of this code was created by Niraj Sanghvi
  Code for day and month arc added by George Culver
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0xBD, 0x2A, 0xCC, 0xCA, 0xD9, 0x4B, 0x4B, 0x0C, 0xA0, 0x95, 0x17, 0x8D, 0xEA, 0xF9, 0xBB, 0xBD }


PBL_APP_INFO(MY_UUID, 
	"Polar 5 Arc", "N Sanghvi and G Culver",
	1, 0,
	RESOURCE_ID_IMAGE_MENU_ICON,
	APP_INFO_WATCH_FACE);

#define SHOW_TEXT_TIME 0
#define SHOW_TEXT_DATE 1
#define ROW_DATE 0
#define TWENTY_FOUR_HOUR_DIAL 0
#define SHOW_DAY_ARC 1
#define SHOW_MONTH_ARC 1

Window window;

Layer minute_display_layer;
Layer hour_display_layer;
Layer second_display_layer;

#if SHOW_DAY_ARC
Layer day_display_layer;
#endif
	
#if SHOW_MONTH_ARC
Layer month_display_layer;
#endif
	
#if SHOW_TEXT_TIME
TextLayer text_time_layer;
#endif

#if SHOW_TEXT_DATE
TextLayer text_date_layer;
#endif

const GPathInfo SECOND_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-7, -70}, // 70 = radius + fudge; 7 = 70*tan(6 degrees); 6 degrees per minute;
    {7,  -70},
  }
};

GPath second_segment_path;


const GPathInfo MINUTE_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-6, -58}, // 58 = radius + fudge; 6 = 58*tan(6 degrees); 6 degrees per minute; 
    {6,  -58},
  }
};

GPath minute_segment_path;


const GPathInfo HOUR_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-5, -48}, // 48 = radius + fudge; 5 = 48*tan(6 degrees); 30 degrees per hour;
    {5,  -48},
  }
};

GPath hour_segment_path;

const GPathInfo DAY_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-4, -38}, // 38 = radius + fudge; ~12 degrees per day;
    {4,  -38},
  }
};

GPath day_segment_path;

const GPathInfo MONTH_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0, 0},
    {-3, -28}, // 28 = radius + fudge; 30 degrees per month;
    {3,  -28},
  }
};

GPath month_segment_path;

void second_display_layer_update_callback(Layer *me, GContext* ctx) {

  PblTm t;

  get_time(&t);

  unsigned int angle = t.tm_sec * 6;

  GPoint center = grect_center_point(&me->frame);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, center, 65);

  graphics_context_set_fill_color(ctx, GColorBlack);

  for(; angle < 355; angle += 6) {

    gpath_rotate_to(&second_segment_path, (TRIG_MAX_ANGLE / 360) * angle);

    gpath_draw_filled(ctx, &second_segment_path);

  }

  graphics_fill_circle(ctx, center, 60);

}


void minute_display_layer_update_callback(Layer *me, GContext* ctx) {

  PblTm t;

  get_time(&t);

  unsigned int angle = t.tm_min * 6;

  GPoint center = grect_center_point(&me->frame);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, center, 55);

  graphics_context_set_fill_color(ctx, GColorBlack);

  for(; angle < 355; angle += 6) {

    gpath_rotate_to(&minute_segment_path, (TRIG_MAX_ANGLE / 360) * angle);

    gpath_draw_filled(ctx, &minute_segment_path);

  }

  graphics_fill_circle(ctx, center, 50);

}


void hour_display_layer_update_callback(Layer *me, GContext* ctx) {

  PblTm t;

  get_time(&t);

  unsigned int angle;

  #if TWENTY_FOUR_HOUR_DIAL
    angle = (t.tm_hour * 15) + (t.tm_min / 4);
  #else
    angle = (( t.tm_hour % 12 ) * 30) + (t.tm_min / 2);
  #endif

  angle = angle - (angle % 6);

  GPoint center = grect_center_point(&me->frame);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, center, 45);

  graphics_context_set_fill_color(ctx, GColorBlack);

  for(; angle < 355; angle += 6) {

    gpath_rotate_to(&hour_segment_path, (TRIG_MAX_ANGLE / 360) * angle);

    gpath_draw_filled(ctx, &hour_segment_path);
  }

  graphics_fill_circle(ctx, center, 40);
}

void day_display_layer_update_callback(Layer *me, GContext* ctx) {

  PblTm t;

  get_time(&t);
  
  unsigned int angle;

  unsigned int maxdays;
	
  switch (t.tm_mon)  //figure out max days in the month
	{
	case 3:  //April
	case 5:  //June
	case 8:  //Sep
	case 10: //Nov
   		maxdays = 30;
	  
   		break;
	case 1:
   		maxdays = 28;
	 
   		break;
	default:
    	maxdays = 31;
	  
  }
	
  angle = (( t.tm_mday % maxdays ) * 360 / maxdays ) + (t.tm_hour);

  angle = angle - (angle % 6);

  GPoint center = grect_center_point(&me->frame);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, center, 35);

  graphics_context_set_fill_color(ctx, GColorBlack);

  for(; angle < 355; angle += 6) {

    gpath_rotate_to(&day_segment_path, (TRIG_MAX_ANGLE / 360) * angle);

    gpath_draw_filled(ctx, &day_segment_path);
  }

  graphics_fill_circle(ctx, center, 30);
}
//added by gac and is probably wrong
void month_display_layer_update_callback(Layer *me, GContext* ctx) {

  PblTm t;

  get_time(&t);
  
  unsigned int angle;
  
  //angle = (( t.tm_mon % 12 ) * 30) + (t.tm_mday / 2);
  angle = (( ( t.tm_mon + 1) % 12 ) * 30);
  angle = angle - (angle % 6);

  GPoint center = grect_center_point(&me->frame);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_circle(ctx, center, 25);

  graphics_context_set_fill_color(ctx, GColorBlack);

  for(; angle < 355; angle += 6) {

    gpath_rotate_to(&month_segment_path, (TRIG_MAX_ANGLE / 360) * angle);

    gpath_draw_filled(ctx, &month_segment_path);
  }

  graphics_fill_circle(ctx, center, 20);
}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  layer_mark_dirty(&second_display_layer);
  layer_mark_dirty(&minute_display_layer);
  layer_mark_dirty(&hour_display_layer);
	
  #if SHOW_DAY_ARC
	  layer_mark_dirty(&day_display_layer);
  #endif
	  
  #if SHOW_MONTH_ARC
	  layer_mark_dirty(&month_display_layer);
  #endif
	  
  #if SHOW_TEXT_TIME

  // Need to be static because it's used by the system later.
  static char time_text[] = "00:00";

  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  text_layer_set_text(&text_time_layer, time_text);

  #endif

  #if SHOW_TEXT_DATE
  
    /*
	#if ROW_DATE
    static char date_text[] = "00 Xxx";
    string_format_time(date_text, sizeof(date_text), "%e %b", t->tick_time);
    #else
    static char date_text[] = "Xxx 00";
    string_format_time(date_text, sizeof(date_text), "%b %e", t->tick_time);
    #endif
	*/
    static char date_text[] = "00";
    string_format_time(date_text, sizeof(date_text), "%e", t->tick_time);
      
  text_layer_set_text(&text_date_layer, date_text);

  #endif

}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Polar Clock watch");
  window_stack_push(&window, true);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);

  // Init the layer for the second display
  layer_init(&second_display_layer, window.layer.frame);
  second_display_layer.update_proc = &second_display_layer_update_callback;
  layer_add_child(&window.layer, &second_display_layer);

  // Init the second segment path
  gpath_init(&second_segment_path, &SECOND_SEGMENT_PATH_POINTS);
  gpath_move_to(&second_segment_path, grect_center_point(&second_display_layer.frame));

  // Init the layer for the minute display
  layer_init(&minute_display_layer, window.layer.frame);
  minute_display_layer.update_proc = &minute_display_layer_update_callback;
  layer_add_child(&window.layer, &minute_display_layer);

  // Init the minute segment path
  gpath_init(&minute_segment_path, &MINUTE_SEGMENT_PATH_POINTS);
  gpath_move_to(&minute_segment_path, grect_center_point(&minute_display_layer.frame));

  // Init the layer for the hour display
  layer_init(&hour_display_layer, window.layer.frame);
  hour_display_layer.update_proc = &hour_display_layer_update_callback;
  layer_add_child(&window.layer, &hour_display_layer);

  // Init the hour segment path
  gpath_init(&hour_segment_path, &HOUR_SEGMENT_PATH_POINTS);
  gpath_move_to(&hour_segment_path, grect_center_point(&hour_display_layer.frame));

	#if SHOW_DAY_ARC
  // Init the layer for the day arc display
  layer_init(&day_display_layer, window.layer.frame);
  day_display_layer.update_proc = &day_display_layer_update_callback;
  layer_add_child(&window.layer, &day_display_layer);

  // Init the day arc segment path
  gpath_init(&day_segment_path, &DAY_SEGMENT_PATH_POINTS);
  gpath_move_to(&day_segment_path, grect_center_point(&day_display_layer.frame));
	#endif

	#if SHOW_MONTH_ARC
  // Init the layer for the month arc display
  layer_init(&month_display_layer, window.layer.frame);
  month_display_layer.update_proc = &month_display_layer_update_callback;
  layer_add_child(&window.layer, &month_display_layer);

  // Init the month arc segment path
  gpath_init(&month_segment_path, &MONTH_SEGMENT_PATH_POINTS);
  gpath_move_to(&month_segment_path, grect_center_point(&month_display_layer.frame));
	#endif
		
  #if SHOW_TEXT_TIME

  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorWhite);
  text_layer_set_background_color(&text_time_layer, GColorClear);

  #if SHOW_TEXT_DATE
  layer_set_frame(&text_time_layer.layer, GRect(47, 57, 144-47, 168-57));
  #else
  layer_set_frame(&text_time_layer.layer, GRect(47, 70, 144-47, 168-70));
  #endif

  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_time_layer.layer);

  #endif
 
  #if SHOW_TEXT_DATE

  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);
  text_layer_set_background_color(&text_date_layer, GColorClear);
    #if SHOW_TEXT_TIME
    layer_set_frame(&text_date_layer.layer, GRect(44, 80, 144-44, 168-80));
    #else
    layer_set_frame(&text_date_layer.layer, GRect(64, 70, 144-64, 168-70));
    #endif
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_date_layer.layer);

  #endif

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    // Handle time updates
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
