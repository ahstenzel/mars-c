/*
 *	cmars.h
 *	Common declarations and inclusions to be used throughout the project.
 */
#ifndef MARS_C_H
#define MARS_C_H

// Library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Defines
#ifdef MARS_EXPORTS
	#define MARS_API __declspec(dllexport)	// Export functions to DLL, used when building library
#else
	#define MARS_API __declspec(dllimport)	// Import functions from DLL, used when building your game
#endif

// Global variables
static uint32_t _uuid_iterator = 0;				// Next assigned UUID

// Typedefs
typedef uint8_t (*fptr_t)(void*);					// Function pointer type with void* argument

// Platform specific code
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


/*=======================================================================================*/
/* Entity                                                                                */
/* Basic game object. Has a unique ID that is used to link it to components.             */
/*=======================================================================================*/
typedef struct {
	unsigned long int uuid;		// Unique entity ID
} Entity;

// Allocate an entity struct and assign it an ID
MARS_API Entity* create_entity();


/*=======================================================================================*/
/* Components                                                                            */
/* Structs that give entities certain properties or functionality                        */
/*=======================================================================================*/

/*=======================================================*/
/* Transform Component                                   */
/* Gives an entity position and movement                 */
/*=======================================================*/
typedef struct {
	uint32_t entity_id;		// Entity this component is bound to
	float x, y;						// Current position
	float l_x, l_y;				// Previous position
	float acc;						// Acceleration
} ComponentTransform;

// Initialize component
MARS_API void component_transform_init(ComponentTransform* component);

// Update all instances of this component
MARS_API void component_transform_update(ComponentTransform* component, float dt);

/*=======================================================*/
/* Step Component                                        */
/* Gives an entity a function call every game cycle      */
/*=======================================================*/
typedef struct {
	uint32_t entity_id;		// Entity this component is bound to
	fptr_t event;         // Function to perform on updating
} ComponentStep;

// Initialize component
MARS_API void component_step_init(ComponentStep* component);

// Update all instances of this component
MARS_API void component_step_update(ComponentStep* component, float dt);


/*=======================================================================================*/
/* Systems                                                                               */
/* Structs that manage a collection of componenets                                       */
/*=======================================================================================*/

/*=======================================================*/
/* Transform System                                      */
/* Advances all transform components                     */
/*=======================================================*/
typedef struct {
	ComponentTransform *components;		// Dynamic list of all components
	size_t size;											// Size of dynamic list
	size_t num;												// Number of elements in the list
} SystemTransform;	// TODO use bst keyed with UUID instead of plain array

// Initialize the given system
MARS_API uint8_t system_transform_init(SystemTransform* system);

// Add a component to the system bound to the given entity
MARS_API uint8_t system_transform_add_component(SystemTransform* system, uint32_t id);

// Returns a pointer to the component associated with the given UUID, or NULL if not found
MARS_API ComponentTransform* system_transform_get_component(SystemTransform* system, uint32_t id);

// Update all components in the system
MARS_API void system_transform_update(SystemTransform* system, float dt);

// Free all components in the system
MARS_API void system_transform_free(SystemTransform* free);

/*=======================================================*/
/* Step System                                           */
/* Advances all step components                          */
/*=======================================================*/
typedef struct {
	ComponentStep *components;		// Dynamic list of all components
	size_t size;									// Size of dynamic list
	size_t num;										// Number of elements in the list
} SystemStep;	// TODO use bst keyed with UUID instead of plain array

// Initialize the given system
MARS_API uint8_t system_step_init(SystemStep* system);

// Add a component to the system bound to the given entity
MARS_API uint8_t system_step_add_component(SystemStep* system, uint32_t id);

// Returns a pointer to the component associated with the given UUID, or NULL if not found
MARS_API ComponentStep* system_step_get_component(SystemStep* system, uint32_t id);

// Update all components in the system
MARS_API void system_step_update(SystemStep* system, float dt);

// Free all components in the system
MARS_API void system_step_free(SystemStep* free);


/*=======================================================================================*/
/* Engine                                                                                */
/* Highest level container for game state. Contains pointers to other critical modules,  */
/* timing information for measuring time between frames, and pointers to initialization  */
/* and destruction functions.                                                            */
/*=======================================================================================*/
typedef struct {
	fptr_t on_init;										// Function run when engine is created
	fptr_t on_free;										// Function run when engine is destroyed
	struct timeval old_time;					// Time used when calculating dt between frames
	struct timeval new_time;
	float time_accum;									// Accumulator for time measured between frames
	float render_alpha;								// Scalar for frame interpolation
	float dt;													// Time (in seconds) that should pass between game cycles
	bool run;													// Continue running the game loop
	SystemTransform* sys_transform;		// System for managing Transform components
  SystemStep* sys_step;		          // System for managing Step components
} Engine;

// Initializes the given Engine struct
MARS_API uint8_t engine_init(Engine* engine);

// Updates the given Engines game state
MARS_API uint8_t engine_update(Engine* engine);

// Frees the modules associated with the given Engine struct
MARS_API uint8_t engine_free(Engine* engine);

#endif
