/*
 *	mars_system_transform.h
 *	Declaration for Transform system
 */
#ifndef MARS_SYSTEM_TRANSFORM_H
#define MARS_SYSTEM_TRANSFORM_H

#include "mars_common.h"
#include "mars_component_transform.h"

typedef struct {
	ComponentTransform *components;		// Dynamic list of all components
	size_t size;											// Size of dynamic list
	size_t num;												// Number of elements in the list
} SystemTransform;	// TODO use bst keyed with UUID instead of plain array

// Initialize the given system
DllExport unsigned int system_transform_init(SystemTransform* system);

// Add a component to the system bound to the given entity
DllExport unsigned int system_transform_add_component(SystemTransform* system, uuid id);

// Returns a pointer to the component associated with the given UUID, or NULL if not found
DllExport ComponentTransform* system_transform_get_component(SystemTransform* system, uuid id);

// Update all components in the system
DllExport void system_transform_update(SystemTransform* system, float dt);

// Free all components in the system
DllExport void system_transform_free(SystemTransform* free);

#endif