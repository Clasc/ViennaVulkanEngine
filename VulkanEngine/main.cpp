/**
* The Vienna Vulkan Engine
*
* (c) bei Helmut Hlavacs, University of Vienna
*
*/

#include "VEInclude.h"
#include "MyVulkanEngine.h"
#include "game/GameEngine.h"

int main()
{

	bool debug = false;
#ifdef _DEBUG
	debug = true;
#endif

	ve::MyVulkanEngine mve(false); //enable or disable debugging (=callback, validation layers)
	game::GameEngine gameEngine(false);
	gameEngine.initEngine();
	gameEngine.loadLevel(1);
	gameEngine.run();

	// mve.initEngine();
	// mve.loadLevel(1);
	// mve.run();

	return 0;
}
