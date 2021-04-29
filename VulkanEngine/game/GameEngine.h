#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "../VEInclude.h"
#include "Levels/LevelManager.h"
#include "EventListeners/PlayerController.h"
#include "EventListeners/CollisionListener.h"
#include "EventListeners/MpegStreamListener.h"

using namespace ve;

namespace game
{

    class GameEngine : public VEEngine
    {
    private:
        LevelManager m_levelManager;
        ve::VECameraProjective *_camera;

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
            //VEEngine::registerEventListeners();
            registerEventListener(new PlayerController(), {veEvent::VE_EVENT_KEYBOARD, veEvent::VE_EVENT_FRAME_STARTED});
            registerEventListener(new CollisionListener(), {veEvent::VE_EVENT_FRAME_STARTED});
            registerEventListener(new MpegStreamListener(), {veEvent::VE_EVENT_FRAME_ENDED, veEvent::VE_EVENT_KEYBOARD, veEvent::VE_EVENT_DELETE_NODE});
        };

        void loadLevel(uint32_t numLevel = 1)
        {
            setupCameraAndPlayer();
            setupLights();
            auto pScene = m_levelManager.getLevelScene(numLevel);
            registerEventListeners();
        }

        void setupCameraAndPlayer()
        {
            auto *playerParent = getSceneManagerPointer()->createSceneNode(EntityName::PlayerParent, getRoot(),
                                                                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f)));

            auto *cameraPlayer = getSceneManagerPointer()->createSceneNode(EntityName::CameraPlayer, playerParent,
                                                                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

            auto *player = getSceneManagerPointer()->createSceneNode(EntityName::Player, cameraPlayer);

            //camera can only do yaw (parent y-axis) and pitch (local x-axis) rotations
            auto extent = getWindowPointer()->getExtent();
            _camera = (VECameraProjective *)getSceneManagerPointer()->createCamera("StandardCamera", VECamera::VE_CAMERA_TYPE_PROJECTIVE, cameraPlayer);
            _camera->m_nearPlane = 0.1f;
            _camera->m_farPlane = 500.1f;
            _camera->m_aspectRatio = extent.width / (float)extent.height;
            _camera->m_fov = 90.0f;
            _camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            getSceneManagerPointer()->setCamera(_camera);
        }

        void setupLights()
        {

            auto *light4 = (VESpotLight *)getSceneManagerPointer()->createLight("StandardAmbientLight", VELight::VE_LIGHT_TYPE_AMBIENT, _camera);
            light4->m_col_ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

            //use one light source
            auto *light1 = (VEDirectionalLight *)getSceneManagerPointer()->createLight("StandardDirLight", VELight::VE_LIGHT_TYPE_DIRECTIONAL, getRoot()); //new VEDirectionalLight("StandardDirLight");
            light1->lookAt(glm::vec3(0.0f, 20.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            light1->m_col_diffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
            light1->m_col_specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

            // VELight *light3 = (VEPointLight *)getSceneManagerPointer()->createLight("StandardPointLight", VELight::VE_LIGHT_TYPE_POINT, camera); //new VEPointLight("StandardPointLight");		//sphere is attached to this!
            // light3->m_col_diffuse = glm::vec4(0.99f, 0.99f, 0.6f, 1.0f);
            // light3->m_col_specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            // light3->m_param[0] = 200.0f;
            // light3->multiplyTransform(glm::translate(glm::vec3(0.0f, 0.0f, 15.0f)));

            auto *light2 = (VESpotLight *)getSceneManagerPointer()->createLight("StandardSpotLight", VELight::VE_LIGHT_TYPE_SPOT, _camera);
            light2->m_col_diffuse = glm::vec4(0.99f, 0.6f, 0.6f, 1.0f);
            light2->m_col_specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            light2->m_param[0] = 200.0f;
            light2->multiplyTransform(glm::translate(glm::vec3(5.0f, 0.0f, 0.0f)));
        }
    };
}
#endif