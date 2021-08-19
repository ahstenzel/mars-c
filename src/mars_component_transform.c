/*
 *	mars_component_transform.c
 *	Definition for Transform component
 */
#include "mars_component_transform.h"

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