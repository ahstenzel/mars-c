/*
 *	mars.c
 *	Definitions for all MARS functionality
 */
#define MARS_EXPORTS
#include "cmars.h"

// Platform specific code
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


/*=======================================================================================*/
/* Hash Tables                                                                           */
/*=======================================================================================*/

Hash* hash_create(void) {
  // Allocate space for hash table struct
  Hash* table = malloc(sizeof(Hash));
  if (table == NULL) {
    return NULL;
  }
  table->length = 0;
  table->capacity = 16;

  // Allocate (zero'd) space for entry buckets
  table->entries = calloc(table->capacity, sizeof(HashEntry));
  if (table->entries == NULL) {
    free(table); // error, free table before we return!
    return NULL;
  }
  return table;
}

void hash_destroy(Hash* table) {
  // First free allocated keys
  for (size_t i = 0; i < table->capacity; i++) {
    if (table->entries[i].key != NULL) {
      free((void*)table->entries[i].key);
    }
  }

  // Then free entries array and table itself
  free(table->entries);
  free(table);
}

uint32_t hash_key(char* key) {
  uint32_t hash = FNV_OFFSET;
  for (char* p = key; *p; p++) {
    hash ^= (uint32_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}

void* hash_get(Hash* table, char* key) {
  // AND hash with capacity-1 to ensure it's within entries array
  uint32_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint32_t)(table->capacity - 1));

  // Loop till we find an empty entry
  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {
      // Found key, return value
      return table->entries[index].value;
    }
    // Key wasn't in this slot, move to next (linear probing)
    index++;
    if (index >= table->capacity) {
      // At end of entries array, wrap around
      index = 0;
    }
  }
  return NULL;
}

bool hash_remove(Hash* table, char* key) {
  // AND hash with capacity-1 to ensure it's within entries array
  uint32_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint32_t)(table->capacity - 1));

  // Loop till we find an empty entry
  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {
      // Found key, set to NULL
      table->entries[index].key = NULL;
      table->entries[index].value = NULL;
      return true;
    }
    // Key wasn't in this slot, move to next (linear probing)
    index++;
    if (index >= table->capacity) {
      // At end of entries array, wrap around
      index = 0;
    }
  }
  return false;
}

char* hash_set_entry(HashEntry* entries, size_t capacity,
  char* key, void* value, size_t* plength) {
  // AND hash with capacity-1 to ensure it's within entries array
  uint32_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint32_t)(capacity - 1));

  // Loop till we find an empty entry.
  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      // Found key (it already exists), update value
      entries[index].value = value;
      return entries[index].key;
    }
    // Key wasn't in this slot, move to next (linear probing)
    index++;
    if (index >= capacity) {
      // At end of entries array, wrap around
      index = 0;
    }
  }

  // Didn't find key, allocate+copy if needed, then insert it
  if (plength != NULL) {
    key = strdup(key);
    if (key == NULL) {
        return NULL;
    }
    (*plength)++;
  }
  entries[index].key = (char*)key;
  entries[index].value = value;
  return key;
}

bool hash_expand(Hash* table) {
  // Allocate new entries array.
  size_t new_capacity = table->capacity * 2;
  if (new_capacity < table->capacity) {
    return false;  // overflow (capacity would be too big)
  }
  HashEntry* new_entries = calloc(new_capacity, sizeof(HashEntry));
  if (new_entries == NULL) {
    return false;
  }

  // Iterate entries, move all non-empty ones to new table's entries
  for (size_t i = 0; i < table->capacity; i++) {
    HashEntry entry = table->entries[i];
    if (entry.key != NULL) {
      hash_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
    }
  }

  // Free old entries array and update this table's details
  free(table->entries);
  table->entries = new_entries;
  table->capacity = new_capacity;
  return true;
}

char* hash_set(Hash* table, char* key, void* value) {
  assert(value != NULL);
  if (value == NULL) {
    return NULL;
  }

  // If length will exceed 75% current capacity, expand it
  if (table->length >= (table->capacity * 3) / 4) {
    if (!hash_expand(table)) {
      return NULL;
    }
  }

  // Set entry and update length.
  return hash_set_entry(table->entries, table->capacity, key, value, &table->length);
}

size_t hash_length(Hash* table) {
  return table->length;
}

HashIt hash_iterator(Hash* table) {
  HashIt it;
  it._table = table;
  it._index = 0;
  return it;
}

bool hash_next(HashIt* it) {
  // Loop till we've hit end of entries array
  Hash* table = it->_table;
  while (it->_index < table->capacity) {
    size_t i = it->_index;
    it->_index++;
    if (table->entries[i].key != NULL) {
      // Found next non-empty item, update iterator key and value
      HashEntry entry = table->entries[i];
      it->key = entry.key;
      it->value = entry.value;
      return true;
    }
  }
  return false;
}


/*=======================================================================================*/
/* Components                                                                            */
/*=======================================================================================*/

uint8_t component_init(Component* component) {
  // Initialize component data
  component->entity_id = NULL;
  component->data = NULL;
  component->init = NULL;
  component->update = NULL;
  component->free = NULL;
  return 0;
}

uint8_t component_data_init(Component* component, int type) {
  // TODO Arrange in a global hash
  // Define component type
  switch(type) {
    case MARS_COMP_TRANSFORM: 
      component->data = malloc(sizeof(ComponentTransform*));
      component->init = component_transform_init;
      component->update = component_transform_update;
    break;
    case MARS_COMP_STEP: 
      component->data = malloc(sizeof(ComponentStep*));
      component->init = component_step_init;
      component->update = component_step_update;
    break;
  }
  // Init component data
  if (component->init) { 
    void* args[] = {component->entity_id, component->data};
    component->init(args, 2); 
  }
  return 0;
}

uint8_t component_update(Component* component, float dt) {
  // Update component data
  if (component->update) { 
    void* args[] = {component->entity_id, component->data, &dt};
    component->update(args, 3); 
  }
  return 0;
}

uint8_t component_free(Component* component) {
  // Free component data
  if (component->free) { 
    void* args[] = {component->entity_id, component->data};
    component->free(args, 2); 
  }
  // Free base component values
  if (component->data) { free(component->data); }
  return 0;
}

/*=======================================================*/
/* Transform Component                                   */
/*=======================================================*/
uint8_t component_transform_init(void** args, size_t num) {
  // Get reference
  ComponentTransform *data = (ComponentTransform*)args[1];
  // Set values
	data->x = 0.0f;
	data->y = 0.0f;
	data->l_x = 0.0f;
	data->l_y = 0.0f;
	data->acc = 0.0f;
  return 0;
}

uint8_t component_transform_update(void** args, size_t num) {
  // Get reference
  ComponentTransform *data = (ComponentTransform*)args[1];
  float dt = *((float*)args[2]);
	// Verlet integration
	float _x_ = data->x;
	float _y_ = data->y;
	data->x = (2.0f*_x_) - (data->l_x) + (dt*dt*(data->acc));
	data->y = (2.0f*_y_) - (data->l_y) + (dt*dt*(data->acc));
	data->l_x = _x_;
	data->l_y = _y_;
  return 0;
}

/*=======================================================*/
/* Step Component                                        */
/*=======================================================*/
uint8_t component_step_init(void** args, size_t num) {
  // Get reference
  ComponentStep *data = (ComponentStep*)args[1];
  // Set values
  data->event = NULL;
  return 0;
}

uint8_t component_step_update(void** args, size_t num) {
  // Get reference
  char* entity_id = (char*)args[0];
  ComponentStep *data = (ComponentStep*)args[1];
  float dt = *((float*)args[2]);
	// Perform event
  if (data->event) { 
    void* event_args[] = {entity_id, &dt};
    return data->event(event_args, 2); 
  }
  return 0;
}


/*=======================================================================================*/
/* Systems                                                                               */
/*=======================================================================================*/

uint8_t system_init(System* system) {
  // Initialize values
  system->num_components = 0;
  system->size_components = 8;
  system->type = -1;
  system->components = calloc(system->size_components, sizeof(Component*));
  // TODO allocate space for components and their data types side-by-side
  return 0;
}

Component* system_add_component(System* system, char* id) {
  // Allocate more memory if needed
  while (system->num_components >= system->size_components) {
    system->size_components *= 2;
    system->components = realloc(system->components, system->size_components*sizeof(Component*));
  }
  
  // Initialize new component
  Component* component = &system->components[system->num_components];
  system->num_components++;
  component_init(component);
  component->entity_id = id;

  // Assign component data
  component_data_init(component, system->type);

  return component;
}

Component* system_get_component(System* system, char* id) {
  for(size_t i=0; i<system->num_components; ++i) { // TODO save components as hash
    if (strcmp(system->components[i].entity_id, id) == 0) {
      return &system->components[i];
    }
  }
  return NULL;
}

uint8_t system_update(System* system, float dt) {
  for(size_t i=0; i<system->num_components; ++i) {
    component_update(&system->components[i], dt);
  }
  return 0;
}

uint8_t system_free(System* system) {
  // Free components
  for(size_t i=0; i<system->num_components; ++i) {
    component_free(&system->components[i]);
  }
  free(system->components);
  return 0;
}


/*=======================================================================================*/
/* Engine                                                                                */
/*=======================================================================================*/
uint8_t engine_init(Engine* engine) {
	// Initialize values
	engine->new_time = (const struct timeval){0};
	engine->old_time = (const struct timeval){0};
	engine->time_accum = 0.0f;
	engine->render_alpha = 0.0f;
	engine->dt = 0.0f;
	engine->run = true;
  engine->num_systems = 0;
  engine->size_systems = 8;
  engine->systems = calloc(engine->size_systems, sizeof(System*));
  engine->entities = hash_create();

	// Get current time
	gettimeofday(&(engine->old_time), 0);

	// Run engine init function
  void* args[] = {engine};
	if (engine->on_init) { engine->on_init(args, 1); }
	return 0;
}

uint8_t engine_update(Engine* engine) {
	while (engine->run) {
		// Get frame time
		gettimeofday(&(engine->new_time), 0);
		long int sec = engine->new_time.tv_sec - engine->old_time.tv_sec;
		long int usec = engine->new_time.tv_usec - engine->old_time.tv_usec;
		engine->time_accum += sec+(usec*0.000001f);
		engine->old_time = engine->new_time;

		// Consume frame time in discrete dt-sized bits
		while (engine->time_accum >= engine->dt) {
			// Update systems
      for(size_t i=0; i<engine->num_systems; ++i) {
        system_update(&engine->systems[i], engine->dt);
      }

			// Reduce remaining time
			engine->time_accum -= engine->dt;
		}

		// Normalize remaining time
		engine->render_alpha = engine->time_accum / engine->dt;

		// Update renderer state
		// TODO
	}
	return 0;
}

System* engine_add_system(Engine* engine, int type) {
  // Allocate more memory if needed
  while (engine->num_systems >= engine->size_systems) {
    engine->size_systems *= 2;
    engine->systems = realloc(engine->systems, engine->size_systems*sizeof(System*));
  }
  
  // Initialize new system
  System* system = &engine->systems[engine->num_systems];
  engine->num_systems++;
  system_init(system);
  
  // Set system properties
  system->type = type;

  return system;
}

System* engine_get_system(Engine* engine, int type) {
  for(size_t i=0; i<engine->num_systems; ++i) { // TODO save system as hash(?)
    if (engine->systems[i].type == type) {
      return &engine->systems[i];
    }
  }
  return NULL;
}

uint8_t engine_free(Engine* engine) {
  void* args[] = {engine};
	if (engine->on_free) { engine->on_free(args, 1); }
  for(size_t i=0; i<engine->num_systems; ++i) {
    system_free(&engine->systems[i]);
  }
  free(engine->systems);
  hash_destroy(engine->entities);
	return 0;
}


/*=======================================================================================*/
/* Entity                                                                                */
/*=======================================================================================*/
void uuid_generate(char* uuid) {
  uint8_t buf[UUID_BYTES];

  // Generate random numbers
  for(uint8_t i=0; i<(UUID_BYTES/4); ++i) {
    uint32_t a = RAND;
    buf[(i*4)]   = (a & 0xFF);
    buf[(i*4)+1] = ((a >> 8) & 0xFF);
    buf[(i*4)+2] = ((a >> 16) & 0xFF);
    buf[(i*4)+3] = ((a >> 24) & 0xFF);
  }

  // Convert to characters
  uint8_t *pin = buf;
  const char *hex = "0123456789ABCDEF";
  char *pout = uuid;
  for(; pin < buf+UUID_BYTES; pout+=2, pin++){
    pout[0] = hex[(*pin>>4) & 0xF];
    pout[1] = hex[ *pin     & 0xF];
  }

  // Null terminate
  uuid[2*UUID_BYTES] = '\0';
  return;
}

Entity* create_entity(Engine* engine) {
	Entity* new_entity = malloc(sizeof(Entity));
  new_entity->uuid = calloc((2*UUID_BYTES)+1, sizeof(char));
	uuid_generate(new_entity->uuid);
  hash_set(engine->entities, new_entity->uuid, new_entity);
	return new_entity;
}

Entity* get_entity(Engine* engine, char* id) {
  return hash_get(engine->entities, id);
}

Component* get_entity_component(Engine* engine, char* id, int type) {
  System* system = engine_get_system(engine, type);
  if (system) {
    Component* component = system_get_component(system, id);
    if (component) {
      return component;
    }
  }
  return NULL;
}

uint8_t destroy_entity(Engine* engine, Entity* entity) {
  if (entity) {
    free(entity->uuid);
    free(entity); 
  }
  return 0;
}