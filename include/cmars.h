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
#include <stddef.h>
#include <assert.h>

// Defines
#ifdef MARS_EXPORTS
	#define MARS_API __declspec(dllexport)	// Export functions to DLL, used when building library
#else
	#define MARS_API __declspec(dllimport)	// Import functions from DLL, used when building your game
#endif

// Typedefs
typedef uint8_t (*fptr_t)(void**, size_t);    // Function pointer type with list of void* arguments

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
/* RNG                                                                                   */
/* Methods for random number generation                                                  */
/*=======================================================================================*/
#define MARS_RNG_MWC		// Declare which RNG system to use
//#define MARS_RNG_STD

/*=======================================================*/
/* MWC                                                   */
/* Multiply-with-carry. Fast and simple, but not the     */
/* most robust.                                          */
/*=======================================================*/
#if defined(MARS_RNG_MWC)
	static uint32_t mwc_z = 362436069;
	static uint32_t mwc_w = 521288629;
	#define MWC_ZNEW (mwc_z=36969*(mwc_z&65535)+(mwc_z>>16))
	#define MWC_WNEW (mwc_w=18000*(mwc_w&65535)+(mwc_z>>16))
	#define MWC ((MWC_ZNEW<<16)+MWC_WNEW)
	#define RAND MWC
/*=======================================================*/
/* std                                                   */
/* Use the standard C rand() function.                   */
/*=======================================================*/
#elif defined(MARS_RNG_STD)
	#define RAND rand()
#endif

void rng_seed(uint32_t seed) {
	#if defined(MARS_RNG_MWC)
		mwc_z = seed;
		mwc_w = 521288629;
	#elif defined(MARS_RNG_STD)
		srand(seed);
	#endif
}


/*=======================================================================================*/
/* Hash Table                                                                            */
/* Simple hash table implementation                                                      */
/*=======================================================================================*/
#define FNV_OFFSET 2166136261
#define FNV_PRIME 16777619

/*=======================================================*/
/* Hash Entry                                            */
/* Key value pair stored in the table                    */
/*=======================================================*/
typedef struct {
	char* key;
	void* data;
} HashEntry;

/*=======================================================*/
/* Hash                                                  */
/* Overall container for hash entries                    */
/*=======================================================*/
typedef struct {
	HashEntry* entries;
	size_t length;
	size_t capacity;
} Hash;

/*=======================================================*/
/* Hash Iterator                                         */
/* Used for iterating over all elements in a table       */
/*=======================================================*/
typedef struct {
	Hash* table;
	size_t index;
	HashEntry* entry;
} HashIt;

// Calculate hash index from key
MARS_API size_t hash_index(char* key);

// Initialize hash table
MARS_API uint8_t hash_init(Hash* table);

// Resize hash table
MARS_API uint8_t hash_resize(Hash* table, size_t new_size);

// Free memory associatetd with the hash table
MARS_API void hash_free(Hash* table);

// Add element to the hash table
MARS_API uint8_t hash_add(Hash* table, char* key, void* data);

// Get element from hash table
MARS_API void* hash_get(Hash* table, char* key);

// Remove element from hash table
MARS_API uint8_t hash_remove(Hash* table, char* key);

// Create hash iterator
MARS_API HashIt* hash_iterator(Hash* table);

// Increment hash iterator
MARS_API uint8_t hash_next(HashIt** it);


/*=======================================================================================*/
/* Components                                                                            */
/* Structs that give entities certain properties or functionality                        */
/*=======================================================================================*/

/*=======================================================*/
/* Transform Component                                   */
/* Gives an entity position and movement                 */
/*=======================================================*/
typedef struct {
  char* entity_id;      // Entity this component is bound to
	float x, y;						// Current position
	float l_x, l_y;				// Previous position
	float acc;						// Acceleration
} ComponentTransform;

// Initialize component
MARS_API uint8_t component_transform_init(void** args, size_t num);

// Update all instances of this component
MARS_API uint8_t component_transform_update(void** args, size_t num);

/*=======================================================*/
/* Step Component                                        */
/* Gives an entity a function call every game cycle      */
/*=======================================================*/
typedef struct {
  char* entity_id;      // Entity this component is bound to
	fptr_t event;         // Function to execute
} ComponentStep;

// Initialize component
MARS_API uint8_t component_step_init(void** args, size_t num);

// Update all instances of this component
MARS_API uint8_t component_step_update(void** args, size_t num);


/*=======================================================================================*/
/* Systems                                                                               */
/* Structs that manage a collection of componenets                                       */
/*=======================================================================================*/
typedef struct {
  char* uuid;               // Unique ID
  Hash* components;         // Hash table containing all components
  size_t data_size;         // Size of component data struct
  fptr_t init;              // Function to run when initializing component
  fptr_t update;            // Function to run when updating component
  fptr_t free;              // Function to run when freeing component
} System;

// Initialize system
MARS_API uint8_t system_init(System* system);

// Initialize a component with the given system
MARS_API uint8_t system_init_component(System* system, void* component);

// Add component to the system for the given entity
MARS_API uint8_t system_add_component(System* system, char* entity_id, void* component);

// Get the component bound to the given entity
MARS_API void* system_get_component(System* system, char* entity_id);

// Update all components under this system
MARS_API uint8_t system_update(System* system, float dt);

// Free all memory for this system
MARS_API uint8_t system_free(System* system);


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
	Hash* systems;                    // Hash table containing all systems
	Hash* entities;										// Hash table containing all entities
} Engine;

// Initializes the given Engine struct
MARS_API uint8_t engine_init(Engine* engine);

// Updates the given Engines game state
MARS_API uint8_t engine_update(Engine* engine);

// Add a new system to the engine
MARS_API uint8_t engine_add_system(Engine* engine, char* system_id, System* system);

// Get a pointer to the given system
MARS_API System* engine_get_system(Engine* engine, char* system_id);

// Frees the modules associated with the given Engine struct
MARS_API uint8_t engine_free(Engine* engine);


/*=======================================================================================*/
/* Entity                                                                                */
/* Basic game object. Has a unique ID that is used to link it to components.             */
/*=======================================================================================*/
typedef struct {
	char* uuid;		        // Unique entity ID
} Entity;

#define UUID_BYTES 8    // Number of bytes in a UUID string

// Generate a random unique string to identify each entity
MARS_API void uuid_generate(char* uuid);

// Allocate an entity struct and assign it an ID
MARS_API Entity* entity_create(Engine* engine);

// Get a reference to the entity with the given ID
MARS_API Entity* entity_get(Engine* engine, char* entity_id);

// Get the component type for the entity with the given ID
MARS_API void* entity_get_component(Engine* engine, char* system_id, char* entity_id);

// Free the resources associated with the given entity
MARS_API uint8_t entity_destroy(Engine* engine, Entity* entity);

#endif
