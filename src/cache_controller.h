bool writeToCache = false;
uint32_t ck_cache_values = 1;
time_t maxCacheAge = (60 * 60 * 2);

typedef struct 
{
  char yield[12]; 
  char soc[9];
  char state[23];
  char load[10];
  time_t age;
} cache_values;
static cache_values cache;

static void InitCacheValues()
{
  snprintf(cache.load, sizeof(cache.load), "%s", "N/A");
  snprintf(cache.soc, sizeof(cache.soc), "%s", "N/A");
  snprintf(cache.yield, sizeof(cache.yield), "%s", "N/A");
  snprintf(cache.state, sizeof(cache.state), "%s", "");
  cache.age = 0;
}

static bool RetrieveCachedData()
{  
  if (persist_exists(ck_cache_values))
  {
    persist_read_data(ck_cache_values, &cache, sizeof(cache));
    return true;
  }
  else
  {
    return false;
  }
}

static void SaveCacheData()
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Saving cache values");
  persist_write_data(ck_cache_values, &cache, sizeof(cache));
}

static void ValidateCache()
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Validating cache");
  if (persist_get_size(ck_cache_values) != sizeof(cache_values))
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "Erasing cache");
    persist_delete(ck_cache_values);
  }
}