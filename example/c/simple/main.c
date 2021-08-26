/*
 *	main.c
 *	Simple main loop to show what a game implementing this library would look like
 */
#include "cmars.h"
#include <time.h>

Engine* game = NULL;
char* SystemTransform = NULL;
char* SystemStep = NULL;

uint8_t myentity_step(void** args, size_t num) {
  // Decode arguments
  char* entity_id = (char*)args[0];
  float dt = *((float*)args[1]);

  // Get position
  ComponentTransform* component_transform = entity_get_component(game, SystemTransform, entity_id);
  if (component_transform) {
    printf("Step: (%f, %f)\n", component_transform->x, component_transform->y);
  }
  else {
    printf("Failed to get entity component data!\n");
  }
  
  return 0;
}

uint8_t myengine_init(void** args, size_t num) {
	// Validate argument
	Engine* engine = (Engine*)args[0];

  // Assign engine properties
  engine->dt = 0.5f;

  // Create systems
  System* system_transform = malloc(sizeof(System));                  // Allocate space
  system_init(system_transform);                                      // Initialize memory & generate unique ID
  system_transform->data_size = sizeof(ComponentTransform);           // Record size of corresponding component struct
  system_transform->init = component_transform_init;                  // Record component init function
  system_transform->update = component_transform_update;              // Record component update function
  engine_add_system(game, system_transform->uuid, system_transform);  // Add system to engine
  SystemTransform = system_transform->uuid;                           // Save ID for reference later

  System* system_step = malloc(sizeof(System));                       // etc...
  system_init(system_step);
  system_step->data_size = sizeof(ComponentStep);
  system_step->init = component_step_init;
  system_step->update = component_step_update;
  engine_add_system(game, system_step->uuid, system_step);
  SystemStep = system_step->uuid;

	// Create game object
	Entity* myentity = entity_create(game);
  printf("Made entity with id %s\n", myentity->uuid);

  // Give entity components
  ComponentTransform* component_transform = malloc(sizeof(ComponentTransform));
  system_init_component(system_transform, (void*)component_transform);
  component_transform->l_x = 1.0f;
  component_transform->l_y = 1.0f;
  component_transform->x = 1.0f;
  component_transform->y = 1.0f;
  system_add_component(system_transform, myentity->uuid, (void*)component_transform);

  ComponentStep* component_step = malloc(sizeof(ComponentStep));
  system_init_component(system_step, (void*)component_step);
  component_step->event = myentity_step;
  system_add_component(system_step, myentity->uuid, (void*)component_step);

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