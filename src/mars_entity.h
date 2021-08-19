/*
 *	mars_entity.h
 *	Declaration for base game entity struct
 */
#ifndef MARS_ENTITY_H
#define MARS_ENTITY_H

#include "mars_common.h"

typedef struct {
	unsigned long int uuid;		// Unique entity ID
} Entity;

// Allocate an entity struct and assign it an ID
DllExport Entity* create_entity();

#endif