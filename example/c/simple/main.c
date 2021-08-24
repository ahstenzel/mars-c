/*
 *	main.c
 *	Simple main loop to show what a game implementing this library would look like
 */
#include "cmars.h"

Engine* game = NULL;

uint8_t myentity_step(void** args, size_t num) {
  // Decode arguments
  char* entity_id = (char*)args[0];
  float dt = *((float*)args[1]);

  // Get position
  Component* transform_component = get_entity_component(game, entity_id, MARS_COMP_TRANSFORM);
  if (transform_component) {
    ComponentTransform* transform_data = (ComponentTransform*)transform_component->data;
    // Print position
    printf("Step: (%f, %f), Acc: (%f)\n", transform_data->x, transform_data->y, transform_data->acc);
  }
  else {
    printf("Failed to get entity component data!");
  }
  
  return 0;
}

uint8_t myengine_init(void** args, size_t num) {
	// Validate argument
	Engine* engine = (Engine*)args[0];

  // Assign engine properties
  engine->dt = 0.5f;
  System* transform_system = engine_add_system(engine, MARS_COMP_TRANSFORM);
  System* step_system = engine_add_system(engine, MARS_COMP_STEP);

	// Create game object
	Entity* myentity = create_entity(game);
  printf("Made entity with id %s\n", myentity->uuid);

  // Give entity properties
  Component* transform_component = system_add_component(transform_system, myentity->uuid);
  Component* step_component = system_add_component(step_system, myentity->uuid);

  // Assign entity properties
  ComponentTransform* transform_data = (ComponentTransform*)transform_component->data;
  transform_data->x = 1.0f;
  transform_data->y = 1.0f;
  transform_data->l_x = 1.0f;
  transform_data->l_y = 1.0f;
  ComponentStep* step_data = (ComponentStep*)step_component->data;
  step_data->event = myentity_step;

  printf("Initialized!\n");
  return 0;
}

uint8_t myengine_free(void** args, size_t num) {
	printf("Goodbye world!\n");
	return 0;
}

int main(void) {
	// Create engine
  game = malloc(sizeof(Engine));
	if (game) {
		game->on_init = myengine_init;
		game->on_free = myengine_free;

		// Initialize game
		engine_init(game);

		// Update game
		engine_update(game);

		// End game
		engine_free(game);
		free(game);
	}
	return 0;
}