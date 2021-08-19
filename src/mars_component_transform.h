/*
 *	mars_component_transform.h
 *	Declaration for Transform component
 */
#ifndef MARS_COMP_TRANSFORM_H
#define MARS_COMP_TRANSFORM_H

#include "mars_common.h"

typedef struct {
	uuid entity_id;			// Entity this component is bound to
	float x, y;					// Current position
	float l_x, l_y;			// Previous position
	float acc;					// Acceleration
} ComponentTransform;

// Initialize component
DllExport void component_transform_init(ComponentTransform* component);

// Update all instances of this component
DllExport void component_transform_update(ComponentTransform* component, float dt);

#endif