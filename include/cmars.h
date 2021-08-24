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
		char* key;    // Key is NULL if this slot is empty
		void* value;
} HashEntry;

/*=======================================================*/
/* Hash                                                  */
/* Overall container for hash entries                    */
/*=======================================================*/
typedef struct {
	HashEntry* entries;   // Hash slots
	size_t capacity;      // Size of internal array
	size_t length;        // Number of entries in the table
} Hash;

// Create hash table and return pointer to it, or NULL if out of memory
MARS_API Hash* hash_create(void);

// Free memory allocated for hash table, including allocated keys
MARS_API void hash_destroy(Hash* table);

// Return 64-bit FNV-1a hash for key (NUL-terminated)
MARS_API uint32_t hash_key(char* key);

// Get item with given key (NUL-terminated) from hash table, or NULL
MARS_API void* hash_get(Hash* table, char* key);

// Remove item with given key (NUL-terminated) from hash table
MARS_API bool hash_remove(Hash* table, char* key);

// Internal function to set an entry (without expanding table).
MARS_API char* hash_set_entry(HashEntry* entries, size_t capacity, char* key, 
	void* value, size_t* plength);

// Expand hash table to twice its current size
MARS_API bool hash_expand(Hash* table);

// Add item with given key (NUL-terminated) to value
MARS_API char* hash_set(Hash* table, char* key, void* value);

// Get number of items in hash table
MARS_API size_t hash_length(Hash* table);

/*=======================================================*/
/* Hash Iterator                                         */
/* Used for iterating over an entire table               */
/*=======================================================*/
typedef struct {
	char* key;    	// Current key
	void* value;        	// Current value

	// Don't use these fields directly.
	Hash* _table;       	// reference to hash table being iterated
	size_t _index;      	// current index into Hash._entries
} HashIt;

// Create new hash table iterator
MARS_API HashIt hash_iterator(Hash* table);

// Move iterator to next item in hash table
MARS_API bool hash_next(HashIt* it);


/*=======================================================================================*/
/* Components                                                                            */
/* Structs that give entities certain properties or functionality                        */
/*=======================================================================================*/
typedef struct {
	char* entity_id;  // Entity this component is bound to
	void *data;             // Data held by this component
	fptr_t init;            // Function run on initialization
	fptr_t update;          // Function run on update every game cycle
	fptr_t free;            // Function run on freeing component
} Component;

MARS_API uint8_t component_init(Component* component);

MARS_API uint8_t component_data_init(Component* component, int type);

MARS_API uint8_t component_update(Component* component, float dt);

MARS_API uint8_t component_free(Component* component);

/*=======================================================*/
/* Transform Component                                   */
/* Gives an entity position and movement                 */
/*=======================================================*/
#define MARS_COMP_TRANSFORM 0

typedef struct {
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
#define MARS_COMP_STEP 1

typedef struct {
	fptr_t event;
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
	Component *components;
	size_t size_components;
	size_t num_components;
	int type;
} System;

MARS_API uint8_t system_init(System* system);

MARS_API Component* system_add_component(System* system, char* id);

MARS_API Component* system_get_component(System* system, char* id);

MARS_API uint8_t system_update(System* system, float dt);

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
	System* systems;                  // Systems to update
	size_t size_systems;              // Size of systems array
	size_t num_systems;               // Number of systems
	Hash* entities;										// Hash table containing all entities
} Engine;

// Initializes the given Engine struct
MARS_API uint8_t engine_init(Engine* engine);

// Updates the given Engines game state
MARS_API uint8_t engine_update(Engine* engine);

// Add a new system to the engine
MARS_API System* engine_add_system(Engine* engine, int type);

// Get a pointer to the given system
MARS_API System* engine_get_system(Engine* engine, int type);

// Frees the modules associated with the given Engine struct
MARS_API uint8_t engine_free(Engine* engine);


/*=======================================================================================*/
/* Entity                                                                                */
/* Basic game object. Has a unique ID that is used to link it to components.             */
/*=======================================================================================*/
typedef struct {
	char* uuid;		  // Unique entity ID
} Entity;

#define UUID_BYTES 8    // Number of bytes in a UUID string

// Generate a random unique string to identify each entity
MARS_API void uuid_generate(char* uuid);

// Allocate an entity struct and assign it an ID
MARS_API Entity* create_entity(Engine* engine);

// Get a reference to the entity with the given ID
MARS_API Entity* get_entity(Engine* engine, char* id);

// Get the component type for the entity with the given ID
MARS_API Component* get_entity_component(Engine* engine, char* id, int type);

// Free the resources associated with the given entity
MARS_API uint8_t destroy_entity(Engine* engine, Entity* entity);

#endif
