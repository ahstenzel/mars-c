/*
 *  core.h
 *  Declaration for core functionality to be used throughout the project.
 */
#ifndef MARS_CORE_H
#define MARS_CORE_H

/*=======================================================*/
/* Includes                                              */
/*=======================================================*/
#include "internal/platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES
#include <math.h>


/*=======================================================*/
/* Internal Predefines                                   */
/*=======================================================*/
#ifndef C_VECTOR_H
typedef struct vector vector;
#endif
#ifndef C_STACK_H
typedef struct stack stack;
#endif
#ifndef C_UMAP_H
typedef struct unordered_map unordered_map;
#endif


/*=======================================================*/
/* Defines                                               */
/*=======================================================*/
#ifdef MARS_SHARED_DEFINE
  #ifdef MARS_EXPORTS
    #define MARS_API __declspec(dllexport)
  #else
    #define MARS_API __declspec(dllimport)
  #endif
#else
  #define MARS_API
#endif

/* Output logging */
#ifndef NDEBUG
  extern uint8_t __mars_verbosity;
#endif
#define MARS_VERB_ERROR 0x1   // 0000 0001
#define MARS_VERB_WARNING 0x2 // 0000 0010
#define MARS_VERB_NOTICE 0x4  // 0000 0100

typedef uint64_t id_t;   // Use 64-bit keys for tables
#define ID_NULL 0x8000000000000000
#define ID_MASK 0x7FFFFFFFFFFFFFFF

/* Useful functions */
#define radtodeg(a) (a * (180.0 / M_PI))
#define degtorad(a) (a * (M_PI / 180.0))



/*=======================================================*/
/* Typedefs                                              */
/*=======================================================*/
typedef uint8_t (*fptr_t)(size_t, void**);    // Function pointer type with list of void* arguments


/*=======================================================*/
/* Environment-specific code                             */
/*=======================================================*/
/* Platform specific */
#if defined(_WIN32)
#include <Windows.h>
  // Mimic linux timeval struct
  typedef struct {
    long tv_sec;
    long tv_usec;
  } timeval;

  // Mimic linux gettimeofday function for wall clock time measurements
  MARS_API int gettimeofday(struct timeval * tp, struct timezone * tzp);
#elif defined(__linux__)
	#include <sys/time.h>
#endif

/* Compiler specific */
#if defined(MARS_CMP_MSVC)
#include <intrin.h>   // CPU feature detection
#endif


/*=======================================================*/
/* Global functions                                      */
/*=======================================================*/
MARS_API id_t uuid_generate();

MARS_API void mars_dlog(uint8_t, const char*, ...);


/*=======================================================================================*/
/* Entity                                                                                */
/* Basic game object. Has a unique ID that is used to link it to components.             */
/*=======================================================================================*/
typedef struct {
	id_t uuid;		        // Unique ID
} Entity;

// Create and initialize an entity
MARS_API Entity* entity_create();

// Free the resources associated with the entity
MARS_API void entity_destroy(Entity*);


/*=======================================================================================*/
/* System                                                                                */
/* Structs that manage a collection of components                                        */
/*=======================================================================================*/
typedef struct {
  unordered_map* components;  // Container for components
  fptr_t init;                // Function to run when initializing component
  fptr_t update;              // Function to run when updating component
  fptr_t destroy;             // Function to run when freeing component
  id_t uuid;                  // Unique ID
  size_t component_size;      // Size (in bytes) of each component
} System;

// Create and initialize a system
MARS_API System* system_create(size_t, fptr_t, fptr_t, fptr_t);

// Create a new component, add it to the system, and return a reference to it
MARS_API uint8_t system_new_component(System*, id_t);

// Add a component to a system
MARS_API uint8_t system_add_component(System*, id_t, void*);

// Get the component of a system
MARS_API void* system_get_component(System*, id_t);

// Update all components in the system
MARS_API void system_update(System*, float*);

// Free all memory for this system
MARS_API void system_destroy(System*);


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
MARS_API Engine* engine_create(fptr_t, fptr_t, int, char**);

// Create a new system, add it to the engine, and return a reference to it
MARS_API id_t engine_new_system(Engine*, size_t, fptr_t, fptr_t, fptr_t);

// Add a system to the engine
MARS_API uint8_t engine_add_system(Engine*, System*);

// Get a pointer to the given system
MARS_API System* engine_get_system(Engine*, id_t);

// Create a new entity, add it to the engine, and return a reference to it
MARS_API id_t engine_new_entity(Engine*);

// Add an entity to the engine
MARS_API uint8_t engine_add_entity(Engine*, Entity*);

// Get a pointer to the given entity
MARS_API Entity* engine_get_entity(Engine*, id_t);

// Give a component for the given system to the given entity
MARS_API uint8_t engine_new_entity_component(Engine*, id_t, id_t);

// Get the component for the given entity from the given system
MARS_API void* engine_get_entity_component(Engine*, id_t, id_t);

// Updates the given engine game state
MARS_API void engine_update(Engine*);

// Free all modules associated with the engine
MARS_API void engine_destroy(Engine*);

#endif  // MARS_CORE_H