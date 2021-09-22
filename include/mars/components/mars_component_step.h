/*
 *  mars_component_step.h
 *  Declaration for simple step component implementation.
 */
#ifndef MARS_COMPONENT_STEP_H
#define MARS_COMPONENT_STEP_H

#include "../mars_core.h"   // Core definitions
#include "exports/export.h" // CMake generated header

/*=======================================================*/
/* Step Component                                        */
/* Gives an entity a function call every game cycle      */
/*=======================================================*/
typedef struct {
  id_t entity_id;       // Entity this component is bound to
	fptr_t event;         // Function to execute
} ComponentStep;

// Initialize component
MARS_EXPORT uint8_t component_step_init(size_t, void**);

// Update all instances of this component
MARS_EXPORT uint8_t component_step_update(size_t, void**);

#endif  // MARS_COMPONENT_STEP_H