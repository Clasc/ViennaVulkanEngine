#include "../../VEInclude.h"
#include "../Const/EntityName.h"

#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

using namespace ve;

namespace game
{

    class PlayerController : public VEEventListener
    {
    private:
        inline static const float m_movement_speed = 40.0f;
        inline static const std::string NAME = "PlayerController";
        ///create some default constants for the actions

    public:
        PlayerController();
        ~PlayerController();
        virtual bool onKeyboard(veEvent event);
        virtual void onFrameStarted(veEvent event);
    };

    PlayerController::PlayerController() : VEEventListener(NAME)
    {
    }

    PlayerController::~PlayerController()
    {
    }

    bool PlayerController::onKeyboard(veEvent event)
    {
        if (event.idata1 == GLFW_KEY_ESCAPE)
        { //ESC pressed - end the engine
            getEnginePointer()->end();
            return true;
        }

        glm::vec4 translate = glm::vec4(0.0, 0.0, 0.0, 1.0); //total translation;
        glm::vec4 rot4 = glm::vec4(1.0);

        float angle = 0.0;

        if (event.idata3 == GLFW_RELEASE)
        {
            return false;
        }

        auto cameraPlayer = getSceneManagerPointer()->getSceneNode(EntityName::CameraPlayer);
        auto player = getSceneManagerPointer()->getSceneNode(EntityName::Player);
        auto playerParent = getSceneManagerPointer()->getSceneNode(EntityName::PlayerParent);

        switch (event.idata1)
        {
        case GLFW_KEY_W:
            translate = cameraPlayer->getTransform() * glm::vec4(0.0, 0.0, 0.5, 1.0); //left
            break;
        case GLFW_KEY_A:
            translate = cameraPlayer->getTransform() * glm::vec4(-1.0, 0.0, 0.0, 1.0); //left
            break;
        case GLFW_KEY_D:
            translate = cameraPlayer->getTransform() * glm::vec4(1.0, 0.0, 0.0, 1.0); //right
            break;
        case GLFW_KEY_S:
            translate = cameraPlayer->getTransform() * glm::vec4(0.0, 0.0, -0.5, 1.0); //back
            break;
        case GLFW_KEY_Q:
            translate = glm::vec4(0.0, -1.0, 0.0, 1.0); //down
            break;
        case GLFW_KEY_E:
            translate = glm::vec4(0.0, 1.0, 0.0, 1.0); //up
            break;
        case GLFW_KEY_LEFT: //yaw rotation is already in parent space
            angle = (float)event.dt * -1.0f;
            rot4 = glm::vec4(0.0, 1.0, 0.0, 1.0);
            break;
        case GLFW_KEY_RIGHT: //yaw rotation is already in parent space
            angle = (float)event.dt * 1.0f;
            rot4 = glm::vec4(0.0, 1.0, 0.0, 1.0);
            break;
        case GLFW_KEY_UP:                                                        //pitch rotation is in cam/local space
            angle = (float)event.dt * 1.0f;                                      //pitch angle
            rot4 = cameraPlayer->getTransform() * glm::vec4(0.0, 0.0, 1.0, 1.0); //x axis from local to parent space!
            break;
        case GLFW_KEY_DOWN:                                                      //pitch rotation is in cam/local space
            angle = (float)event.dt * -1.0f;                                     //pitch angle
            rot4 = cameraPlayer->getTransform() * glm::vec4(0.0, 0.0, 1.0, 1.0); //x axis from local to parent space!
            break;

        default:
            return false;
        };

        ///add the new translation vector to the previous one
        glm::vec3 trans = m_movement_speed * glm::vec3(translate.x, translate.y, translate.z);
        playerParent->multiplyTransform(glm::translate(glm::mat4(1.0f), (float)event.dt * trans));

        ///combination of yaw and pitch, both wrt to parent space
        glm::vec3 rot3 = glm::vec3(rot4.x, rot4.y, rot4.z);
        glm::mat4 rotate = glm::rotate(player->getTransform(), angle, rot3);
        cameraPlayer->multiplyTransform(rotate);

        return true;
    }

    void PlayerController::onFrameStarted(veEvent event)
    {
        // glm::vec4 translate = glm::vec4(0.0, 0.0, 0.0, 1.0); //total translation;
        // auto playerEntity = getSceneManagerPointer()->getSceneNode(EntityName::CameraPlayer);
        // auto player = getSceneManagerPointer()->getSceneNode(EntityName::Player);
        // translate = player->getTransform() * glm::vec4(0.0, 0.0, 1.0, 1.0);
        // glm::vec3 trans = m_movement_speed * glm::vec3(translate.x, translate.y, translate.z);
        // playerEntity->multiplyTransform(glm::translate(glm::mat4(1.0f), (float)event.dt * trans));
    }
}
#endif