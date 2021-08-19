/*
 *	mars_system_transform.c
 *	Definition for Transform system
 */
#include "mars_system_transform.h"

unsigned int system_transform_init(SystemTransform* system) {
	// Initialize values
	system->size = 8;
	system->num = 0;
	// Allocate memory
	system->components = malloc(system->size*sizeof(ComponentTransform));
	if (!system->components) { return 1; }
	else { return 0; }
}

ComponentTransform* system_transform_get_component(SystemTransform* system, uuid id) {
	if (id < system->num) { return &(system->components[id]); }
	else { return NULL; }
}

unsigned int system_transform_add_component(SystemTransform* system, uuid id) {
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