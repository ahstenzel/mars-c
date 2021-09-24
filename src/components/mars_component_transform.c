#ifndef MARS_EXPORTS
  #define MARS_EXPORTS
#endif
#include "mars/components/mars_component_transform.h"

uint8_t component_transform_init(size_t num, void** args) {
  // Get reference
  ComponentTransform *data = (ComponentTransform*)args[0];
  id_t uuid = *(id_t*)args[1];
  // Set values
  data->entity_id = uuid;
	data->x = 0.0f;
	data->y = 0.0f;
	data->l_x = 0.0f;
	data->l_y = 0.0f;
	data->acc = 0.0f;
  return 0;
}

uint8_t component_transform_update(size_t num, void** args) {
  // Get reference
  ComponentTransform *data = (ComponentTransform*)args[0];
  float dt = *(float*)args[1];
	// Verlet integration
	float _x_ = data->x;
	float _y_ = data->y;
	data->x = (2.0f*_x_) - (data->l_x) + (dt*dt*(data->acc));
	data->y = (2.0f*_y_) - (data->l_y) + (dt*dt*(data->acc));
	data->l_x = _x_;
	data->l_y = _y_;
  return 0;
}