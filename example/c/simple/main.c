/*
 *	main.c
 *	Simple main loop to show what a game implementing this library would look like
 */
#include "cmars.h"

uint8_t myentity_step(void* arg) {
  float dt = *((float*)arg);
  printf("Step: %f\n", dt);
  return 0;
}

uint8_t myengine_init(void* arg) {
	// Validate argument
	Engine* engine = (Engine*)arg;
  engine->dt = 0.5f;

	// Create game object
	Entity* myentity = create_entity();

	// Give entity properties
  system_transform_add_component(engine->sys_transform, myentity->uuid);
  system_step_add_component(engine->sys_step, myentity->uuid);

  // Get entity position
  ComponentTransform* transform = system_transform_get_component(engine->sys_transform, myentity->uuid);

  // Print position
  printf("Made an entity with coordinates (%f,%f)\n", transform->x, transform->y);

  // Give entity step function
  ComponentStep* step = system_step_get_component(engine->sys_step, myentity->uuid);
  step->event = myentity_step;
  return 0;
}

uint8_t myengine_free(void* arg) {
	printf("Goodbye world!\n");
	return 0;
}

int main(void) {
	// Create engine
	Engine* myengine = malloc(sizeof(Engine));
	if (myengine) {
		myengine->on_init = myengine_init;
		myengine->on_free = myengine_free;

		// Initialize game
		engine_init(myengine);

		// Update game
		engine_update(myengine);

		// End game
		engine_free(myengine);
		free(myengine);
	}
	return 0;
}