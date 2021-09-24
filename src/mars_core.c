#ifndef MARS_EXPORTS
  #define MARS_EXPORTS
#endif
#include "mars/mars_core.h"

/*=======================================================*/
/* Definitions                                           */
/*=======================================================*/
#ifndef MARS_RAND_H
  #define RAND rand()
  #define RAND_SEED(s) srand(s)
#endif
#ifndef NDEBUG
  uint8_t __mars_verbosity;
#endif


/*=======================================================*/
/* Environment-specific code                             */
/*=======================================================*/
#if defined(_WIN32)
  int gettimeofday(struct timeval* tp, struct timezone* tzp) {
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
  }
#endif


/*=======================================================*/
/* Global functions                                      */
/*=======================================================*/
id_t uuid_generate() {
  return (id_t)(RAND & (ID_NULL - 1));
}

void mars_dlog(uint8_t level, const char* format, ...) {
  #ifndef NDEBUG
    if (level & __mars_verbosity) {
      va_list args;
      va_start(args, format);
      vprintf(format, args);
    }
  #endif
}


/*=======================================================*/
/* System                                                */
/*=======================================================*/

System* system_create(size_t component_size, fptr_t init, fptr_t update, fptr_t destroy) {
  // Assign default values
  System* system = malloc(sizeof(*system));
  if (!system) { 
    mars_dlog(MARS_VERB_ERROR, "[system_create] malloc failed!\n");
    return NULL; 
  }
  system->components = unordered_map_create(void*);
  if (!system->components) {
    mars_dlog(MARS_VERB_ERROR, "[system_create] Failed to create component map!\n");
    free(system);
    return NULL;
  }
  system->init = init;
  system->update = update;
  system->destroy = destroy;
  system->uuid = uuid_generate();
  system->component_size = component_size;

  return system;
}

uint8_t system_new_component(System* system, id_t entity_id) {
  // Error check
  if (!system) { return 1; }
  
  // Allocate space for component
  void* component = malloc(system->component_size);
  if (!component) { return 1; }

  // Run init function
  if (system->init) {
    void* args[] = {component, &entity_id};
    system->init(2, args);
  }

  // Attempt to insert
  return unordered_map_insert(system->components, entity_id, &component);
}

uint8_t system_add_component(System* system, id_t entity_id, void* component) {
  // Error check
  if (!system) { return 1; }

  // Run function
  if (system->init) {
    void* args[] = {component, &entity_id};
    system->init(2, args);
  }

  // Attemp to insert
  return unordered_map_insert(system->components, entity_id, &component);
}

void* system_get_component(System* system, id_t entity_id) {
  // Error check
  if (!system) { return NULL; }

  // Attempt to find
  void** data = unordered_map_find(system->components, entity_id);
  return (data) ? (*data) : NULL;
}

void system_update(System* system, float* dt) {
  // Error check
  if (!system) { 
    mars_dlog(MARS_VERB_ERROR, "[system_update] System reference NULL!\n");
    return; 
  }

  // Iterate through components
  if (system->update) {
    for(umap_it_t* it = unordered_map_it(system->components); it; unordered_map_it_next(it)) {
      void* args[] = {*(void**)(it->data), dt};
      system->update(2, args);
    }
  }
}

void system_destroy(System* system) {
  if (system) {
    // Iterate through components
    if (system->destroy) {
      for(umap_it_t* it = unordered_map_it(system->components); it; unordered_map_it_next(it)) {
        // Get double reference
        void** data = it->data;
        
        // Run destroy function
        void* args[] = {*data};
        system->destroy(1, args);

        // Destroy base object
        free(*data);
      }
    }

    // Destroy component map
    unordered_map_destroy(system->components);
  }

  // Destroy struct
  free(system);
}


/*=======================================================*/
/* Engine                                                */
/*=======================================================*/

Engine* engine_create(fptr_t init, fptr_t destroy, int argc, char** argv) {
  // Assign default values
  Engine* engine = malloc(sizeof(*engine));
  if (!engine) { return NULL; }
  engine->init = init;
  engine->destroy = destroy;
  engine->old_time = (const struct timeval){0};
  engine->new_time = (const struct timeval){0};
  engine->time_accum = 0.0f;
  engine->render_alpha = 0.0f;
  engine->dt = 0.01f;
  engine->run = true;
  engine->systems = unordered_map_create(System*);
  engine->entities = unordered_map_create(Entity*);

  // Error check
  if (!engine->systems || !engine->entities) {
    free(engine);
    return NULL;
  }

  // Get current time
	gettimeofday(&(engine->old_time), 0);

  // Process command line flags
  #ifndef NDEBUG
    __mars_verbosity = 0;
    for (size_t optind = 1; optind < argc && argv[optind][0] == '-'; ++optind) {
      switch (argv[optind][1]) {
        case 'v':   // Verbosity flags
          if (optind < (argc - 1)) {
            __mars_verbosity = atoi(argv[optind + 1]);
          }
        break;
      }
    }
  #endif

  // Run function
  if (init) {
    void* args[] = { engine };
    init(1, args);
  }

  return engine;
}

id_t engine_new_system(Engine* engine, size_t component_size, fptr_t init, fptr_t update, fptr_t destroy) {
  // Error check
  if (!engine) { 
    mars_dlog(MARS_VERB_ERROR, "[engine_new_system] Engine reference NULL!\n");
    return ID_NULL; 
  }

  // Allocate space for system
  System* system = system_create(component_size, init, update, destroy);
  if (!system) { 
    mars_dlog(MARS_VERB_ERROR, "[engine_new_system] Failed to create system!\n");
    return ID_NULL; 
  }

  // Attempt to insert
  uint8_t result = unordered_map_insert(engine->systems, system->uuid, &system);
  if (result > 0) { 
    mars_dlog(MARS_VERB_ERROR, "[engine_new_system] Insert failed!\n"); 
    return ID_NULL;
  }
  return system->uuid;
}

uint8_t engine_add_system(Engine* engine, System* system) {
  // Error check
  if (!engine) { return 1; }

  // Attempt to insert
  return unordered_map_insert(engine->systems, system->uuid, &system);
}

System* engine_get_system(Engine* engine, id_t uuid) {
  // Error check
  if (!engine) { return NULL; }

  // Attempt to find
  void** data = unordered_map_find(engine->systems, uuid);
  return (data) ? (System*)(*data) : NULL;
}

id_t engine_new_entity(Engine* engine) {
  if (!engine) { return ID_NULL; }

  // Allocate space for entity
  Entity* entity = entity_create();
  if (!entity) { return ID_NULL; }

  // Attempt to insert
  uint8_t result = unordered_map_insert(engine->entities, entity->uuid, &entity);
  return (result > 0) ? ID_NULL : entity->uuid;
}

uint8_t engine_add_entity(Engine* engine, Entity* entity) {
  // Error check
  if (!engine) { return 1; }

  // Attempt to insert
  return unordered_map_insert(engine->entities, entity->uuid, &entity);
}

Entity* engine_get_entity(Engine* engine, id_t uuid) {
  // Error check
  if (!engine) { return NULL; }

  // Attempt to find
  void** data = unordered_map_find(engine->entities, uuid);
  return (data) ? (Entity*)(*data) : NULL;
}

uint8_t engine_new_entity_component(Engine* engine, id_t system_id, id_t entity_id) {
  // Error check
  if (!engine) { return 1; }

  // Get system
  System* system = engine_get_system(engine, system_id);
  if (!system) { return 1; }

  // Create component in system for entity
  return system_new_component(system, entity_id);
}

void* engine_get_entity_component(Engine* engine, id_t system_id, id_t entity_id) {
  // Error check
  if (!engine) { return NULL; }

  // Get system
  System* system = engine_get_system(engine, system_id);
  if (!system) { return NULL; }

  // Create component in system for entity
  return system_get_component(system, entity_id);
}

void engine_update(Engine* engine) {
  while(engine->run) {
    // Get frame time
    gettimeofday(&(engine->new_time), 0);
    long int sec = engine->new_time.tv_sec - engine->old_time.tv_sec;
    long int usec = engine->new_time.tv_usec - engine->old_time.tv_usec;
    engine->time_accum += sec+(usec*0.000001f);
    engine->old_time = engine->new_time;

    // Consume frame time in discrete dt-sized bits
    while (engine->time_accum >= engine->dt) {
      // Update systems
      for(umap_it_t* it = unordered_map_it(engine->systems); it; unordered_map_it_next(it)) {
        system_update(*(System**)(it->data), &(engine->dt));
      }

      // Reduce remaining time
      engine->time_accum -= engine->dt;
    }

    // Normalize remaining time
    engine->render_alpha = engine->time_accum / engine->dt;

    // Update renderer state
    // TODO
  }
}

void engine_destroy(Engine* engine) {
  if (engine) {
    // Iterate through systems
    for(umap_it_t* it = unordered_map_it(engine->systems); it; unordered_map_it_next(it)) {
      system_destroy(*(System**)it->data);
    }

    // Destroy system map
    unordered_map_destroy(engine->systems);

    // Iterate through entities
    for(umap_it_t* it = unordered_map_it(engine->entities); it; unordered_map_it_next(it)) {
      entity_destroy(*(Entity**)it->data);
    }

    // Destroy entity map
    unordered_map_destroy(engine->entities);
  }

  // Destroy struct
  free(engine);
}


/*=======================================================*/
/* Entity                                               */
/*=======================================================*/

Entity* entity_create() {
  // Assign default values
  Entity* entity = malloc(sizeof(*entity));
  if (!entity) { return NULL; }
  entity->uuid = uuid_generate();
  return entity;
}

void entity_destroy(Entity* entity) {
  free(entity);
}