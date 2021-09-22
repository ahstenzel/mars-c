/*
 *  mars_component_transform.h
 *  Declaration for simple transform component implementation.
 */
#ifndef MARS_COMPONENT_TRANSFORM_H
#define MARS_COMPONENT_TRANSFORM_H

#include "../mars_core.h"   // Core definitions
#include "exports/export.h" // CMake generated header

/*=======================================================*/
/* Transform Component                                   */
/* Gives an entity position and movement                 */
/*=======================================================*/
typedef struct {
  id_t entity_id;       // Entity this component is bound to
	float x, y;						// Current position
	float l_x, l_y;				// Previous position
	float acc;						// Acceleration
} ComponentTransform;

// Initialize component
MARS_EXPORT uint8_t component_transform_init(size_t, void**);

// Update all instances of this component
MARS_EXPORT uint8_t component_transform_update(size_t, void**);

#endif  // MARS_COMPONENT_TRANSFORM_H