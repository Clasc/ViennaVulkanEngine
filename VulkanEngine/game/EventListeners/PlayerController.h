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
        inline static const float m_movement_speed = 20.0f;
        inline static const std::string NAME = "PlayerController";
        ///create some default constants for the actions
        inline static const glm::vec4 TOTAL_TRANSLATION = glm::vec4(0.0, 0.0, 0.0, 1.0); //total translation
        inline static const glm::vec4 TOTAL_ROTATION = glm::vec4(1.0);                   //total rotation around the axes, is 4d !

    public:
        PlayerController();
        ~PlayerController();
        virtual bool onKeyboard(veEvent event);
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

        glm::vec4 translate = TOTAL_TRANSLATION;
        glm::vec4 rot4 = TOTAL_ROTATION; //total rotation around the axes, is 4d !
        float angle = 0.0;

        if (event.idata3 == GLFW_RELEASE)
            return false;

        auto birdEntity = getSceneManagerPointer()->getSceneNode(EntityName::Player);
        switch (event.idata1)
        {
        case GLFW_KEY_A:
            translate = birdEntity->getTransform() * glm::vec4(-1.0, 0.0, 0.0, 1.0); //left
            break;
        case GLFW_KEY_D:
            translate = birdEntity->getTransform() * glm::vec4(1.0, 0.0, 0.0, 1.0); //right
            break;
        case GLFW_KEY_W:
            translate = birdEntity->getTransform() * glm::vec4(0.0, 0.0, 1.0, 1.0); //forward
            break;
        case GLFW_KEY_S:
            translate = birdEntity->getTransform() * glm::vec4(0.0, 0.0, -1.0, 1.0); //back
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
        case GLFW_KEY_UP:                                                      //pitch rotation is in cam/local space
            angle = (float)event.dt * 1.0f;                                    //pitch angle
            rot4 = birdEntity->getTransform() * glm::vec4(1.0, 0.0, 0.0, 1.0); //x axis from local to parent space!
            break;
        case GLFW_KEY_DOWN:                                                    //pitch rotation is in cam/local space
            angle = (float)event.dt * -1.0f;                                   //pitch angle
            rot4 = birdEntity->getTransform() * glm::vec4(1.0, 0.0, 0.0, 1.0); //x axis from local to parent space!
            break;

        default:
            return false;
        };

        ///add the new translation vector to the previous one
        glm::vec3 trans = m_movement_speed * glm::vec3(translate.x, translate.y, translate.z);
        birdEntity->multiplyTransform(glm::translate(glm::mat4(1.0f), (float)event.dt * trans));

        ///combination of yaw and pitch, both wrt to parent space
        glm::vec3 rot3 = glm::vec3(rot4.x, rot4.y, rot4.z);
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0), angle, rot3);
        birdEntity->multiplyTransform(rotate);

        return true;
    }
}
#endif