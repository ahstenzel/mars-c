/*
 *	mars_engine.c
 *	Definition for high-level engine struct.
 */

#include "mars_engine.h"

int engine_init(Engine* engine) {
	// Initialize values
	engine->new_time = (const struct timeval){0};
	engine->old_time = (const struct timeval){0};
	engine->time_accum = 0.0f;
	engine->render_alpha = 0.0f;
	engine->dt = 0.0f;

	// Get current time
	gettimeofday(&(engine->old_time), 0);

	// Run engine init function
	if (engine->on_init != NULL) { engine->on_init(); }
	return 0;
}

int engine_update(Engine* engine) {
	// Get frame time
	gettimeofday(&(engine->new_time), 0);
	long int sec = engine->new_time.tv_sec - engine->old_time.tv_sec;
	long int usec = engine->new_time.tv_usec - engine->old_time.tv_usec;
	engine->time_accum += sec+(usec*0.000001f);
	engine->old_time = engine->new_time;

	// Consume frame time in discrete dt-sized bits
	while (engine->time_accum >= engine->dt) {
		// Update modules
		// TODO

		// Reduce remaining time
		engine->time_accum -= engine->dt;
	}

	// Normalize remaining time
	engine->render_alpha = engine->time_accum / engine->dt;

	// Update renderer state
	// TODO

	return 0;
}

int engine_free(Engine* engine) {
	if (engine->on_free != NULL) { engine->on_free(); }
	return 0;
}