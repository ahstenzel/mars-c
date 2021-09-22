/*
 *  mars_core.h
 *  Declaration for core functionality to be used throughout the project.
 */
#ifndef MARS_CORE_H
#define MARS_CORE_H

/*=======================================================*/
/* Includes                                              */
/*=======================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "containers/vector.h"          // Custom container
#include "containers/stack.h"           // Custom container
#ifdef MARS_32  // Use 32-bit hashing
#define __UMAP_32
#endif
#include "containers/unordered_map.h"   // Custom container
#ifdef MARS_32  // Use 32-bit keys
#define __LOT_32
#endif
#include "containers/lot.h"             // Custom container
#include "exports/export.h"             // CMake generated header

/*=======================================================*/
/* Typedefs                                              */
/*=======================================================*/
typedef uint8_t (*fptr_t)(size_t, void**);    // Function pointer type with list of void* arguments

/*=======================================================*/
/* Environment-specific code                             */
/*=======================================================*/
// Platform specific
#if defined(_WIN32)
#include <Windows.h>
  // Mimic linux timeval struct
  typedef struct {
    long tv_sec;
    long tv_usec;
  } timeval;

  // Mimic linux gettimeofday function for wall clock time measurements
  MARS_EXPORT int gettimeofday(struct timeval * tp, struct timezone * tzp);
#elif defined(__linux__)
	#include <sys/time.h>
#endif

// Architecture specific
#ifdef MARS_32
typedef uint32_t id_t;   // Use 32-bit keys for tables
#else
typedef uint64_t id_t;   // Use 64-bit keys for tables
#endif

/*=======================================================*/
/* Global functions                                      */
/*=======================================================*/
MARS_EXPORT id_t uuid_generate();


/*=======================================================================================*/
/* Entity                                                                                */
/* Basic game object. Has a unique ID that is used to link it to components.             */
/*=======================================================================================*/
typedef struct {
	id_t uuid;		        // Unique ID
} Entity;

// Create and initialize an entity
MARS_EXPORT Entity* entity_create();

// Free the resources associated with the entity
MARS_EXPORT void entity_destroy(Entity*);


/*=======================================================================================*/
/* System                                                                                */
/* Structs that manage a collection of components                                        */
/*=======================================================================================*/
typedef struct {
  unordered_map* components;  // Container for components
  fptr_t init;                // Function to run when initializing component
  fptr_t update;              // Function to run when updating component
  fptr_t destroy;             // Function to run when freeing component
  id_t uuid;                 // Unique ID
} System;

// Create and initialize a system
MARS_EXPORT System* system_create(size_t, fptr_t, fptr_t, fptr_t);

// Add a component to a system
MARS_EXPORT uint8_t system_add_component(System*, id_t, void*);

// Get the component of a system
MARS_EXPORT void* system_get_component(System*, id_t);

// Update all components in the system
MARS_EXPORT void system_update(System*, float*);

// Free all memory for this system
MARS_EXPORT void system_destroy(System*);


/*=======================================================================================*/
/* Engine                                                                                */
/* Highest level container for game state. Contains pointers to other critical modules,  */
/* timing information for measuring time between frames, and pointers to initialization  */
/* and destruction functions.                                                            */
/*=======================================================================================*/
typedef struct {
	fptr_t init;                      // Function run when engine is created
	fptr_t destroy;                   // Function run when engine is destroyed
	struct timeval old_time;          // Time used when calculating dt between frames
	struct timeval new_time;
	float time_accum;                 // Accumulator for time measured between frames
	float render_alpha;               // Scalar for frame interpolation
	float dt;                         // Time (in seconds) that should pass between game cycles
	bool run;                         // Continue running the game loop
	unordered_map* systems;           // Hash table containing all systems
	unordered_map* entities;          // Hash table containing all entities
} Engine;

// Create and initialize an engine
MARS_EXPORT Engine* engine_create(fptr_t, fptr_t);

// Add a system to the engine
MARS_EXPORT uint8_t engine_add_system(Engine*, id_t, System*);

// Get a pointer to the given system
MARS_EXPORT System* engine_get_system(Engine*, id_t);

// Add an entity to the engine
MARS_EXPORT uint8_t engine_add_entity(Engine*, id_t, Entity*);

// Get a pointer to the given entity
MARS_EXPORT Entity* engine_get_entity(Engine*, id_t);

// Updates the given engine game state
MARS_EXPORT void engine_update(Engine*);

// Free all modules associated with the engine
MARS_EXPORT void engine_destroy(Engine*);

#endif  // MARS_CORE_H