#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "../VEInclude.h"
#include "Levels/LevelManager.h"

using namespace ve;

namespace game
{

    class GameEngine : public VEEngine
    {
    private:
        LevelManager m_levelManager;

    protected:
    public:
        /**
		* \brief Constructor of my engine
		* \param[in] debug Switch debuggin on or off
		*/
        GameEngine(bool debug = false) : VEEngine(debug)
        {
            m_levelManager = LevelManager();
        };
        ~GameEngine(){};

        ///Register an event listener to interact with the user
        virtual void registerEventListeners()
        {
            VEEngine::registerEventListeners();
        };

        void loadLevel(uint32_t numLevel = 1)
        {
            VEEngine::loadLevel(numLevel);
            auto pScene = m_levelManager.getLevelScene(numLevel);
        }
    };
}
#endif