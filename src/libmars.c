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
/* Entity                                                                                */
/*=======================================================================================*/
Entity* create_entity() {
	Entity* new_entity = malloc(sizeof(Entity));
	new_entity->uuid = _uuid_iterator++;	// TODO Implement proper random UUID system
	return new_entity;
}


/*=======================================================================================*/
/* Components                                                                            */
/*=======================================================================================*/

/*=======================================================*/
/* Transform Component                                   */
/*=======================================================*/
void component_transform_init(ComponentTransform* component) {
	component->entity_id = 0;
	component->x = 0.0f;
	component->y = 0.0f;
	component->l_x = 0.0f;
	component->l_y = 0.0f;
	component->acc = 0.0f;
}

void component_transform_update(ComponentTransform* component, float dt) {
	// Verlet integration
	float _x_ = component->x;
	float _y_ = component->y;
	component->x = (2.0f*_x_) - (component->l_x) + (dt*dt*(component->acc));
	component->y = (2.0f*_y_) - (component->l_y) + (dt*dt*(component->acc));
	component->l_x = _x_;
	component->l_y = _y_;
}


/*=======================================================================================*/
/* Systems                                                                               */
/*=======================================================================================*/

/*=======================================================*/
/* Transform System                                      */
/*=======================================================*/
uint8_t system_transform_init(SystemTransform* system) {
	// Initialize values
	system->size = 8;
	system->num = 0;
	// Allocate memory
	system->components = malloc(system->size*sizeof(ComponentTransform));
	if (!system->components) { return 1; }
	else { return 0; }
}

ComponentTransform* system_transform_get_component(SystemTransform* system, uint32_t id) {
	if (id < system->num) { return &(system->components[id]); }
	else { return NULL; }
}

uint8_t system_transform_add_component(SystemTransform* system, uint32_t id) {
	// Allocate more space if needed
	if (id >= system->size) {
		system->size *= 2;
		ComponentTransform *_temp = realloc(system->components, system->size*sizeof(ComponentTransform));
		if (!_temp) { return 1; }
		else { system->components = _temp; }
	}
	// Initialize component
	component_transform_init(&(system->components[id]));
	// Register component in list
	system->components[id].entity_id = id;
	system->num++;
	return 0;
}

void system_transform_update(SystemTransform* system, float dt) {
	for(size_t i=0; i<system->num; ++i) {
		component_transform_update(&(system->components[i]), dt);
	}
}

void system_transform_free(SystemTransform* system) {
	free(system->components);
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
	engine->sys_transform = malloc(sizeof(SystemTransform));

	// Get current time
	gettimeofday(&(engine->old_time), 0);

	// Run engine init function
	if (engine->on_init) { engine->on_init(engine); }
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
			system_transform_update(engine->sys_transform, engine->dt);

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

uint8_t engine_free(Engine* engine) {
	if (engine->on_free) { engine->on_free(engine); }
	free(engine->sys_transform);
	return 0;
}