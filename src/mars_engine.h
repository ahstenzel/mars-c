/*
 *	mars_engine.h
 *	Declaration for high-level engine struct.
 */
#ifndef MARS_ENGINE_H
#define MARS_ENGINE_H

#include "mars_common.h"

/*
 *	struct Engine
 *	Highest level container for game state. Contains pointers to other critical
 *	modules, timing information for measuring time between frames, and
 *	pointers to initialization & destruction functions.
 */
typedef struct {
	fptr_t on_init;						// Function run when engine is created
	fptr_t on_free;						// Function run when engine is destroyed
	struct timeval old_time;	// Time used when calculating dt between frames
	struct timeval new_time;
	float time_accum;					// Accumulator for time measured between frames
	float render_alpha;				// Scalar for frame interpolation
	float dt;									// Time (in seconds) that should pass between game cycles
} Engine;

// Initializes the given Engine struct
int engine_init(Engine* engine);

// Updates the given Engines game state
int engine_update(Engine* engine);

// Frees the modules associated with the given Engine struct
int engine_free(Engine* engine);

#endif
