const int inbox_size = 128;
const int outbox_size = 128;

enum
{
  KEY_JS_READY = 0,
  KEY_BS = 1, // Battery State of Charge
  KEY_A1 = 2, // Load on AC1
  KEY_YT = 3, // Yield Today
  KEY_BST = 4 // Battery state
};

static void RequestVRMData()
{
  // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) 
{
    if (dict_find(iterator, KEY_JS_READY))
    {
      if (time(NULL) - cache.age >= maxCacheAge)
      {
        RequestVRMData();
      }
    }
    else
    {
      Tuple *t_yield = dict_find(iterator, KEY_YT);
      Tuple *t_load = dict_find(iterator, KEY_A1);
      Tuple *t_soc = dict_find(iterator, KEY_BS);
      Tuple *t_state = dict_find(iterator, KEY_BST);
      
      if(t_yield && t_load && t_soc && t_state) 
      {
          writeToCache = true;
        
          snprintf(cache.load, sizeof(cache.load), "%s", t_load->value->cstring);
          snprintf(cache.soc, sizeof(cache.soc), "%s", t_soc->value->cstring);
          snprintf(cache.yield, sizeof(cache.yield), "%s", t_yield->value->cstring);
          snprintf(cache.state, sizeof(cache.state), "%s", t_state->value->cstring);
          cache.age = time(NULL);
          UpdateSolarText();
      }
    }  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void register_app_message_callbacks()
{
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  app_message_open(inbox_size, outbox_size);
}