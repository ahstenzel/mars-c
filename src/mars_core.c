#include "mars/mars_core.h"

/*=======================================================*/
/* Definitions                                           */
/*=======================================================*/
#ifndef MARS_RAND_H
  #define RAND rand()
  #define RAND_SEED(s) srand(s)
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
  return (id_t)(RAND);
}


/*=======================================================*/
/* System                                                */
/*=======================================================*/

System* system_create(size_t component_size, fptr_t init, fptr_t update, fptr_t destroy) {
  // Assign default values
  System* system = malloc(sizeof(*system));
  if (!system) { return NULL; }
  system->components = __umap_factory(component_size, __UMAP_DEFAULT_CAPACITY);
  system->init = init;
  system->update = update;
  system->destroy = destroy;
  system->uuid = uuid_generate();
  
  // Error check
  if (!system->components) {
    free(system);
    return NULL;
  }

  return system;
}

uint8_t system_add_component(System* system, id_t uuid, void* component) {
  // Error check
  if (!system) { return 1; }

  // Run function
  if (system->init) {
    void* args[] = {component};
    system->init(1, args);
  }

  // Attemp to insert
  return unordered_map_insert(system->components, uuid, component);
}

void* system_get_component(System* system, id_t uuid) {
  // Error check
  if (!system) { return NULL; }

  // Attempt to find
  return unordered_map_find(system->components, uuid);
}

void system_update(System* system, float* dt) {
  // Error check
  if (!system) { return; }

  // Iterate through components
  if (system->update) {
    for(umap_it_t* it = unordered_map_it(system->components); it; unordered_map_it_next(it)) {
      void* args[] = {it->data, dt};
      system->update(2, args);
    }
  }
}

void system_destroy(System* system) {
  if (system) {
    // Iterate through components
    if (system->destroy) {
      for(umap_it_t* it = unordered_map_it(system->components); it; unordered_map_it_next(it)) {
        void* args[] = {it->data};
        system->destroy(1, args);
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

Engine* engine_create(fptr_t init, fptr_t destroy) {
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
  engine->systems = unordered_map_create(System);
  engine->entities = unordered_map_create(Entity);

  // Error check
  if (!engine->systems || !engine->entities) {
    free(engine);
    return NULL;
  }

  // Run function
  if (init) {
    void* args[] = {engine};
    init(1, args);
  }

  return engine;
}

uint8_t engine_add_system(Engine* engine, id_t uuid, System* system) {
  // Error check
  if (!engine) { return 1; }

  // Attempt to insert
  return unordered_map_insert(engine->systems, uuid, system);
}

System* engine_get_system(Engine* engine, id_t uuid) {
  // Error check
  if (!engine) { return NULL; }

  // Attempt to find
  return (System*)unordered_map_find(engine->systems, uuid);
}

uint8_t engine_add_entity(Engine* engine, id_t uuid, Entity* entity) {
  // Error check
  if (!engine) { return 1; }

  // Attempt to insert
  return unordered_map_insert(engine->entities, uuid, entity);
}

Entity* engine_get_entity(Engine* engine, id_t uuid) {
  // Error check
  if (!engine) { return NULL; }

  // Attempt to find
  return (Entity*)unordered_map_find(engine->entities, uuid);
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
        system_update((System*)it->data, &engine->dt);
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
      system_destroy((System*)it->data);
    }

    // Destroy system map
    unordered_map_destroy(engine->systems);

    // Iterate through entities
    for(umap_it_t* it = unordered_map_it(engine->entities); it; unordered_map_it_next(it)) {
      entity_destroy((Entity*)it->data);
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