/*
 *	mars_entity.c
 *	Definition for base game entity struct
 */
#include "mars_entity.h"

Entity* create_entity() {
	Entity* new_entity = malloc(sizeof(Entity));
	new_entity->uuid = _uuid_iterator++;	// TODO Implement proper random UUID system
	return new_entity;
}