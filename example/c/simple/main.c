/*
 *	main.c
 *	Simple main loop to show what a game implementing this library would look like
 */
#include "cmars.h"

uint8_t myengine_init(void* arg) {
	// Validate argument
	Engine* engine = (Engine*)arg;

	// Create game object
	Entity* myentity = create_entity();

	// Give entity properties
	if (system_transform_add_component(engine->sys_transform, myentity->uuid) > 0) {
		printf("Failed to create entity!\n");
		return 1;
	}
	else {
		// Get entity position
		ComponentTransform* transform = system_transform_get_component(engine->sys_transform, myentity->uuid);

		// Print position
		printf("Made an entity with coordinates (%f,%f)\n", transform->x, transform->y);
		return 0;
	}
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
		//engine_update(myengine);

		// End game
		engine_free(myengine);
		free(myengine);
	}
	return 0;
}