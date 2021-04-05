#include "../../VEInclude.h"
#include "../Const/EntityName.h"

#ifndef LEVEL_MANAGER
#define LEVEL_MANAGER
using namespace ve;

namespace game
{

    class LevelManager
    {
    private:
    public:
        LevelManager(/* args */);
        ~LevelManager();

        VESceneNode *getLevelScene(const int &level);
        void setPlayer(VESceneNode *pScene);
    };

    LevelManager::LevelManager()
    {
    }

    LevelManager::~LevelManager()
    {
    }

    VESceneNode *LevelManager::getLevelScene(const int &level)
    {
        VESceneNode *pScene;
        pScene = getSceneManagerPointer()->createSceneNode("Level 1", getRoot());

        // VESceneNode *skybox = getSceneManagerPointer()->createSkybox("The Sky", "media/models/test/sky/cloudy",
        //                                                              {"bluecloud_bk.jpg"},
        //                                                              pScene);

        VESceneNode *plane = getSceneManagerPointer()->loadModel("Plane", "media/models/test", "plane_t_n_s.obj", 0, pScene);
        plane->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

        VESceneNode *pointLight = getSceneManagerPointer()->getSceneNode("StandardPointLight");
        if (pointLight != nullptr)
        {
            VESceneNode *eL = getSceneManagerPointer()->loadModel("LightSource", "media/models/test/sphere", "sphere.obj", 0, pointLight);
            eL->multiplyTransform(glm::scale(glm::vec3(0.02f, 0.02f, 0.02f)));

            VEEntity *pE = (VEEntity *)getSceneManagerPointer()->getSceneNode("LightSource/sphere.obj/default/Entity_0");
            pE->m_castsShadow = false;
        }

        setPlayer(pScene);

        return pScene;
    }

    void LevelManager::setPlayer(VESceneNode *pScene)
    {
        VESceneNode *player = getSceneManagerPointer()->loadModel(EntityName::Player, "game/Assets/Models/Bird", "bird.obj");
        player->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f)));
        player->multiplyTransform(glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        player->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 6.0f, 15.0f)));
        pScene->addChild(player);
    }
}
#endif