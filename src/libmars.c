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

size_t hash_index(char* key) {
	uint32_t hash = FNV_OFFSET;
	for (char* p = key; *p; p++) {
		hash ^= (uint32_t)(unsigned char)(*p);
		hash *= FNV_PRIME;
	}
	return hash;
}

uint8_t hash_init(Hash* table) {
	table->length = 0;
	table->capacity = 8;
	table->entries = NULL;
	return hash_resize(table, table->capacity);
}

uint8_t hash_resize(Hash* table, size_t new_capacity) {
	// Allocate new memory block
	if (new_capacity == 0) { return 1; }
	HashEntry* new_entries = malloc(new_capacity * sizeof(HashEntry));
	if (!new_entries) { return 1; }
	for (size_t i = 0; i < new_capacity; ++i) {
		new_entries[i].key = NULL;
		new_entries[i].data = NULL;
	}

	// Rehash old entries
	for (HashIt* it = hash_iterator(table); it; hash_next(&it)) {
		// Get index
		size_t index = hash_index(it->entry->key) % new_capacity;

		// Find empty bucket
		while (new_entries[index].key) {
			if (strcmp(new_entries[index].key, it->entry->key) == 0) {
				// Key already exists, overwrite data
				table->entries[index].data = it->entry->data;
				return 0;
			}
			else {
				// Check next index
				index = (index + 1) % new_capacity;
			}
		}

		// Write data to bucket
		new_entries[index].key = _strdup(it->entry->key);
		new_entries[index].data = it->entry->data;
	}

	// Free old entries
	for (HashIt* it = hash_iterator(table); it; hash_next(&it)) {
		free(it->entry->key);
	}
	free(table->entries);
	table->entries = new_entries;
	table->capacity = new_capacity;

	return 0;
}

void hash_free(Hash* table) {
	for (HashIt* it = hash_iterator(table); it; hash_next(&it)) {
		free(it->entry->key);
	}
	free(table->entries);
	table->length = 0;
	table->capacity = 0;
	return;
}

uint8_t hash_add(Hash* table, char* key, void* data) {
	// Expand if table is more than 3/4 full
	if (table->length >= (table->capacity * 3) / 4) {
		if (hash_resize(table, table->capacity * 2) == 1) {
			return 1;
		}
	}

	// Get index
	size_t index = hash_index(key) % table->capacity;

	// Find empty bucket
	while (table->entries[index].key) {
		if (strcmp(table->entries[index].key, key) == 0) {
			// Key already exists, overwrite data
			table->entries[index].data = data;
			return 0;
		}
		else {
			// Check next index
			index = (index + 1) % table->capacity;
		}
	}

	// Write data to bucket
	table->entries[index].key = _strdup(key);
	table->entries[index].data = data;
	table->length++;
	return 0;
}

void* hash_get(Hash* table, char* key) {
	// Get index
	size_t index = hash_index(key) % table->capacity;

	// Find matching bucket
	size_t count = 0;
	while (count < table->capacity) {
		if (table->entries[index].key && strcmp(table->entries[index].key, key) == 0) {
			break;
		}
		else {
			index = (index + 1) % table->capacity;
			count++;
		}
	}

	// Return what was found
	if (count >= table->capacity) { return NULL; }
	else { return table->entries[index].data; }
}

uint8_t hash_remove(Hash* table, char* key) {
	// Get index
	size_t index = hash_index(key);

	// Find matching bucket
	size_t count = 0;
	while (count < table->capacity) {
		if (table->entries[index].key && strcmp(table->entries[index].key, key) == 0) {
			break;
		}
		else {
			index = (index + 1) % table->capacity;
			count++;
		}
	}

	// Empty the bucket
	if (count >= table->capacity) { return 1; }
	else { 
		free(table->entries[index].key);
		table->entries[index].key = NULL;
		table->entries[index].data = NULL;
		table->length--;
		return 0;
	}
}

HashIt* hash_iterator(Hash* table) {
	if (table->length != 0) {
		// Start before the first entry
		HashIt* it = malloc(sizeof(HashIt));
		if (it) {
			it->table = table;
			it->index = -1;
			it->entry = NULL;

			// Move to the first valid entry
			hash_next(&it);
			return it;
		}
	}
	return NULL;
}

uint8_t hash_next(HashIt** it) {
	if (!(*it)) { return 1; }
	else {
		do {
			// Move to next valid element until you reach the end
			(*it)->index++;
			(*it)->entry = &((*it)->table->entries[(*it)->index]);
		} while ((*it)->index < (*it)->table->capacity && !((*it)->table->entries[(*it)->index].key));
		if ((*it)->index >= (*it)->table->capacity) {
			// Reached the end, no more valid entries
			free(*it);
			*it = NULL;
			return 1;
		}
		else {
			return 0;
		}
	}
}


/*=======================================================================================*/
/* Components                                                                            */
/*=======================================================================================*/

/*=======================================================*/
/* Transform Component                                   */
/*=======================================================*/
uint8_t component_transform_init(void** args, size_t num) {
  // Get reference
  ComponentTransform *data = (ComponentTransform*)args[0];
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
  ComponentTransform *data = (ComponentTransform*)args[0];
  char* entity_id = (char*)args[1];
  float dt = *((float*)args[1]);
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
  ComponentStep *data = (ComponentStep*)args[0];
  // Set values
  data->event = NULL;
  return 0;
}

uint8_t component_step_update(void** args, size_t num) {
  // Get reference
  ComponentStep *data = (ComponentStep*)args[0];
  char* entity_id = (char*)args[1];
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
  system->components = malloc(sizeof(Hash));
  hash_init(system->components);
  system->uuid = calloc((2*UUID_BYTES)+1, sizeof(char));
  uuid_generate(system->uuid);
  system->data_size = 0;
  system->init = NULL;
  system->update = NULL;
  system->free = NULL;
  return 0;
}

uint8_t system_init_component(System* system, void* component) {
  if (system->init) { 
    void* args[] = {component};
    system->init(args, 1); 
  }
  return 0;
}

uint8_t system_add_component(System* system, char* entity_id, void* component) {
  hash_add(system->components, entity_id, component);
  return 0;
}

void* system_get_component(System* system, char* entity_id) {
  return hash_get(system->components, entity_id);
}

uint8_t system_update(System* system, float dt) {
  if (system->update) {
    for (HashIt* it = hash_iterator(system->components); it; hash_next(&it)) {
      if (it->entry->key) {
        void* args[] = {it->entry->data, it->entry->key, &dt};
        system->update(args, 3);
      }
    }
  }
  return 0;
}

uint8_t system_free(System* system) {
  // User defined component free
  if (system->free) {
    for (HashIt* it = hash_iterator(system->components); it; hash_next(&it)) {
      if (it->entry->key) {
        void* args[] = {it->entry->data};
        system->free(args, 1);
      }
    }
  }
  // Destroy components
  hash_free(system->components);
  free(system->components);
  // Destroy system elements
  free(system->uuid);
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
  engine->systems = malloc(sizeof(Hash));
  hash_init(engine->systems);
  engine->entities = malloc(sizeof(Hash));
  hash_init(engine->entities);

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
      for (HashIt* it = hash_iterator(engine->systems); it; hash_next(&it)) {
        if (it->entry->key) {
          system_update((System*)it->entry->data, engine->dt);
        }
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

uint8_t engine_add_system(Engine* engine, char* system_id, System* system) {
  hash_add(engine->systems, system_id, (void*)system);
  return 0;
}

System* engine_get_system(Engine* engine, char* system_id) {
  return (System*)hash_get(engine->systems, system_id);
}

uint8_t engine_free(Engine* engine) {
  // User defined engine free
  void* args[] = {engine};
	if (engine->on_free) { engine->on_free(args, 1); }
  // Destroy entities
  hash_free(engine->entities);
  free(engine->entities);
  // User defined system free
  for (HashIt* it = hash_iterator(engine->systems); it; hash_next(&it)) {
    if (it->entry->key) {
      system_free(it->entry->data);
    }
  }
  // Destroy systems
  hash_free(engine->systems);
  free(engine->systems);
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

Entity* entity_create(Engine* engine) {
	Entity* new_entity = malloc(sizeof(Entity));
  new_entity->uuid = calloc((2*UUID_BYTES)+1, sizeof(char));
	uuid_generate(new_entity->uuid);
  hash_add(engine->entities, new_entity->uuid, (void*)new_entity);
	return new_entity;
}

Entity* entity_get(Engine* engine, char* entity_id) {
  return (Entity*)hash_get(engine->entities, entity_id);
}

void* entity_get_component(Engine* engine, char* system_id, char* entity_id) {
  System* system = engine_get_system(engine, system_id);
  if (system) {
    void* component = system_get_component(system, entity_id);
    if (component) {
      return component;
    }
  }
  return NULL;
}

uint8_t entity_destroy(Engine* engine, Entity* entity) {
  if (entity) {
    free(entity->uuid);
    free(entity); 
  }
  return 0;
}