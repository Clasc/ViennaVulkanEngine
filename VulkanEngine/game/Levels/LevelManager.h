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
        void loadPlayer();
        void loadTree(VESceneNode *pScene);
        void loadTree2(VESceneNode *pScene);
        void loadFruit(VESceneNode *pScene);
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

        loadPlayer();
        loadTree(pScene);
        loadTree2(pScene);
        loadFruit(pScene);

        return pScene;
    }

    void LevelManager::loadFruit(VESceneNode *pScene)
    {
        auto model = getSceneManagerPointer()->loadModel(EntityName::FruitModel1, "game/Assets/Models/Starfruit", "Starfruit.obj");
        auto fruit = getSceneManagerPointer()->createSceneNode(EntityName::Fruit1, pScene);
        model->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        fruit->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 50.0f)));
        fruit->addChild(model);
    }

    void LevelManager::loadTree2(VESceneNode *pScene)
    {
        auto model = getSceneManagerPointer()->loadModel(EntityName::TreeModel2, "game/Assets/Models/Tree2", "lowpolytree.obj");
        auto tree = getSceneManagerPointer()->createSceneNode(EntityName::Tree2, pScene);
        model->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
        model->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 0.0f)));
        tree->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 0.0f, 100.0f)));
        tree->addChild(model);
    }

    void LevelManager::loadTree(VESceneNode *pScene)
    {
        auto model = getSceneManagerPointer()->loadModel(EntityName::TreeModel1, "game/Assets/Models/Tree", "low-poly-tree.obj");
        auto tree = getSceneManagerPointer()->createSceneNode(EntityName::Tree1, pScene);
        model->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
        tree->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 100.0f)));
        tree->addChild(model);
    }

    void LevelManager::loadPlayer()
    {
        auto model = getSceneManagerPointer()->loadModel(EntityName::PlayerModel, "game/Assets/Models/Bird", "bird.obj");
        auto camera = getSceneManagerPointer()->getCamera();
        auto player = getSceneManagerPointer()->getSceneNode(EntityName::Player);

        model->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f)));
        model->multiplyTransform(glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        player->addChild(model);
        camera->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, -12.0f)));
    }
}
#endif