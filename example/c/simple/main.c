/*
 *	main.c
 *	Simple main loop to show what a game implementing this library would look like
 */
#include "mars/mars.h"
#include <time.h>

Engine* game = NULL;
id_t systemTransformId;
id_t systemStepId;

uint8_t myentity_step(size_t num, void** args) {
  // Decode arguments
  id_t entity_id = *(id_t*)args[0];
  float dt = *(float*)args[1];

  // Get position
  ComponentTransform* componentTransform = engine_get_entity_component(game, systemTransformId, entity_id);
  if (componentTransform) {
    printf("Object pos: (%f,%f)\n", componentTransform->x, componentTransform->y);
  }
  else {
    mars_dlog(MARS_VERB_ERROR, "Could not get object transform component!\n");
  }
  
  return 0;
}

uint8_t myengine_init(size_t num, void** args) {
	// Decode arguments
	Engine* engine = (Engine*)args[0];
  if (!engine) {
    mars_dlog(MARS_VERB_ERROR, "Failed to decode engine!\n");
  }

  // Assign engine properties
  engine->dt = 1.0f;

  // Create systems
  systemTransformId = engine_new_system(engine,
    sizeof(ComponentTransform),
    component_transform_init, 
    component_transform_update, 
    NULL);
  systemStepId = engine_new_system(engine,
    sizeof(ComponentStep),
    component_step_init, 
    component_step_update, 
    NULL);
  if (systemTransformId == ID_NULL) {
    mars_dlog(MARS_VERB_ERROR, "Failed to create transform system!\n");
  }
  if (systemStepId == ID_NULL) {
    mars_dlog(MARS_VERB_ERROR, "Failed to create step system!\n");
  }

  // Create entity
  id_t entityId = engine_new_entity(engine);
  if (entityId == ID_NULL) {
    mars_dlog(MARS_VERB_ERROR, "Failed to create Entity!\n");
  }

  // Add components for entity
  uint8_t result = engine_new_entity_component(engine, systemTransformId, entityId);
  if (result > 0) {
    mars_dlog(MARS_VERB_ERROR, "Failed to give the entity a transform component!\n");
  }
  result = engine_new_entity_component(engine, systemStepId, entityId);
  if (result > 0) {
    mars_dlog(MARS_VERB_ERROR, "Failed to give the entity a step component!\n");
  }

  // Configure component
  ComponentStep* componentStep = engine_get_entity_component(engine, systemStepId, entityId);
  if (componentStep) {
    componentStep->event = myentity_step;
  }
  else {
    mars_dlog(MARS_VERB_ERROR, "Failed to get Step component for entity!\n");
  }

  mars_dlog(MARS_VERB_NOTICE, "Initialized!\n");
  return 0;
}

uint8_t myengine_destroy(size_t num, void** args) {
	mars_dlog(MARS_VERB_NOTICE, "Goodbye world!\n");
	return 0;
}

int main(int argc, char* argv[]) {
	// Create engine
  game = engine_create(myengine_init, myengine_destroy, argc, argv);
	if (game) {
		// Update game
		engine_update(game);

		// End game
		engine_destroy(game);
	}
	return 0;
}