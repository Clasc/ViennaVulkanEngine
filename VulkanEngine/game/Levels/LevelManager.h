#include "../../VEInclude.h"
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

        VESceneNode *skybox = getSceneManagerPointer()->loadModel("SkyBox", "../../media/models/test/sky/cloudy", "browncloud_bk.jpg", 0, pScene);
        skybox->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

        VESceneNode *plane = getSceneManagerPointer()->loadModel("Plane", "../../media/models/test", "plane_t_n_s.obj", 0, pScene);
        plane->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

        VESceneNode *pointLight = getSceneManagerPointer()->getSceneNode("StandardPointLight");
        if (pointLight != nullptr)
        {
            VESceneNode *eL = getSceneManagerPointer()->loadModel("LightSource", "../../media/models/test/sphere", "sphere.obj", 0, pointLight);
            eL->multiplyTransform(glm::scale(glm::vec3(0.02f, 0.02f, 0.02f)));

            VEEntity *pE = (VEEntity *)getSceneManagerPointer()->getSceneNode("LightSource/sphere.obj/default/Entity_0");
            pE->m_castsShadow = false;
        }

        VESceneNode *e1 = getSceneManagerPointer()->loadModel("Cube", "../../media/models/test/crate0", "cube.obj");
        e1->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)));
        e1->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 5.0f, 10.0f)));
        pScene->addChild(e1);

        return pScene;
    }
}
#endif