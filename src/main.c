/*
 *	main.c
 *	Example main loop to show what a game implementing this library would look like
 */

#include "mars_common.h"
#include "mars_engine.h"

unsigned int myengine_init() {
	printf("Hello world!\n");
	return 0;
}

unsigned int myengine_free() {
	printf("Goodbye world!\n");
	return 0;
}

int main(void) {
	// Create engine
	Engine* myengine = malloc(sizeof(Engine));
	if (myengine) {
		myengine->on_init = myengine_init;
		myengine->on_free = myengine_init;

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