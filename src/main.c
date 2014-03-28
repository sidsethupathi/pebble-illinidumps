#include <pebble.h>

Window *my_window;
TextLayer *name_layer, *score_layer, *smell_layer, *crowd_layer, *clean_layer;
static bool dataInited;

static char name[64];
static char score[32];
static char smell[16];
static char crowd[17];
static char clean[17];

enum {
  MSG_KEY_INIT = 0x0,
  MSG_KEY_NAME = 0x1,
  MSG_KEY_SCORE = 0x2,
  MSG_KEY_SMELL = 0x3,
  MSG_KEY_CROWD = 0x4,
  MSG_KEY_CLEAN = 0x5,
};

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
  text_layer_set_font(score_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(score_layer, GTextAlignmentLeft);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message received.");
  
  Tuple *init_tuple = dict_find(iter, MSG_KEY_INIT);
  Tuple *name_tuple = dict_find(iter, MSG_KEY_NAME);
  Tuple *score_tuple = dict_find(iter, MSG_KEY_SCORE);
  Tuple *smell_tuple = dict_find(iter, MSG_KEY_SMELL);
  Tuple *crowd_tuple = dict_find(iter, MSG_KEY_CROWD);
  Tuple *clean_tuple = dict_find(iter, MSG_KEY_CLEAN);
  
  if (name_tuple) {
      strncpy(name, name_tuple->value->cstring, 64);
      text_layer_set_text(name_layer, name);
  }
  
  if (score_tuple) {
      strncpy(score, score_tuple->value->cstring, 32);
      text_layer_set_text(score_layer, score);
  }
  
  if (smell_tuple) {
      strncpy(smell, smell_tuple->value->cstring, 16);
      text_layer_set_text(smell_layer, smell);
  }
  
  if (crowd_tuple) {
      strncpy(crowd, crowd_tuple->value->cstring, 17);
      text_layer_set_text(crowd_layer, crowd);
  }
  
  if (clean_tuple) {
      strncpy(clean, clean_tuple->value->cstring, 17);
      text_layer_set_text(clean_layer, clean);
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Layers set.");

  
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped! Reason: %d - %s", reason, translate_error(reason));
}

void handle_init(void) {
  my_window = window_create();

  name_layer = text_layer_create(GRect(0, 0, 144, 40));
  score_layer = text_layer_create(GRect(0, 40, 144, 20));
  smell_layer = text_layer_create(GRect(0, 60, 144, 20));
  crowd_layer = text_layer_create(GRect(0, 80, 144, 20));
  clean_layer = text_layer_create(GRect(0, 100, 144, 20));
  
  text_layer_set_font(name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_font(score_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_font(smell_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_font(crowd_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_font(clean_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  
  //text_layer_set_text_alignment(name_layer, GTextAlignmentLeft);  

  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(name_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(score_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(smell_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(crowd_layer));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(clean_layer));
  
  text_layer_set_overflow_mode(name_layer, GTextOverflowModeWordWrap);
  
  text_layer_set_font(score_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(score_layer, GTextAlignmentCenter);
  text_layer_set_text(score_layer, "Waiting on GPS fix...");
  
  window_stack_push(my_window, true);
  
  // Handles AppMessage inits
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_open(256, 256);
}

void handle_deinit(void) {
  text_layer_destroy(name_layer);
  text_layer_destroy(score_layer);
  text_layer_destroy(smell_layer);
  text_layer_destroy(crowd_layer);
  text_layer_destroy(clean_layer);
  
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
