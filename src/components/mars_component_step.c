#ifndef MARS_EXPORTS
#define MARS_EXPORTS
#endif
#include "internal/internal.h"
#include "mars/components/mars_component_step.h"

uint8_t component_step_init(size_t num, void** args) {
  // Get reference
  ComponentStep *data = (ComponentStep*)args[0];
  id_t uuid = *(id_t*)args[1];
  // Set values
  data->entity_id = uuid;
  data->event = NULL;
  return 0;
}

uint8_t component_step_update(size_t num, void** args) {
  // Get reference
  ComponentStep *data = (ComponentStep*)args[0];
  float* dt = (float*)args[1];
	// Perform event
  if (data->event) { 
    void* args[] = {&data->entity_id, dt};
    return data->event(2, args); 
  }
  return 0;
}