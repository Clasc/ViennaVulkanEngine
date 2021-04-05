#include "VEInclude.h"

#ifndef CUSTOM_LISTENERS_H
#define CUSTOM_LISTENERS_H

namespace ve
{
    ///simple event listener for rotating objects
    class RotatorListener : public VEEventListener
    {
        VESceneNode *m_pObject = nullptr;
        float m_speed;
        glm::vec3 m_axis;

    public:
        ///Constructor
        RotatorListener(std::string name, VESceneNode *pObject, float speed, glm::vec3 axis) : VEEventListener(name), m_pObject(pObject), m_speed(speed), m_axis(glm::normalize(axis)){};

        void onFrameStarted(veEvent event)
        {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), m_speed * (float)event.dt, m_axis);
            m_pObject->multiplyTransform(rot);
        }

        bool onSceneNodeDeleted(veEvent event)
        {
            if (m_pObject == event.ptr)
                return true;
            return false;
        };
    };

    ///simple event listener for loading levels
    class LevelListener : public VEEventListener
    {
    public:
        ///Constructor
        LevelListener(std::string name) : VEEventListener(name){};

        ///load a new level when pressing numbers 1-3
        virtual bool onKeyboard(veEvent event)
        {
            if (event.idata3 == GLFW_RELEASE)
                return false;

            if (event.idata1 == GLFW_KEY_1 && event.idata3 == GLFW_PRESS)
            {
                getSceneManagerPointer()->deleteScene();
                getEnginePointer()->loadLevel(1);
                return true;
            }

            if (event.idata1 == GLFW_KEY_2 && event.idata3 == GLFW_PRESS)
            {
                getSceneManagerPointer()->deleteScene();
                getEnginePointer()->loadLevel(2);
                return true;
            }

            if (event.idata1 == GLFW_KEY_3 && event.idata3 == GLFW_PRESS)
            {
                getSceneManagerPointer()->deleteScene();
                getEnginePointer()->loadLevel(3);
                return true;
            }
            return false;
        }
    };

    ///simple event listener for switching on/off light
    class LightListener : public VEEventListener
    {
    public:
        ///Constructor
        LightListener(std::string name) : VEEventListener(name){};

        ///\brief switch on or off a given light
        void toggleLight(std::string name)
        {
            VELight *pLight = (VELight *)getSceneManagerPointer()->getSceneNode(name);
            if (pLight == nullptr)
                return;

            pLight->m_switchedOn = pLight->m_switchedOn ? false : true;
        }

        ///load a new level when pressing numbers 1-3
        virtual bool onKeyboard(veEvent event)
        {
            if (event.idata3 == GLFW_RELEASE)
                return false;

            if (event.idata1 == GLFW_KEY_8 && event.idata3 == GLFW_PRESS)
            {
                toggleLight("StandardDirLight");
                return true;
            }

            if (event.idata1 == GLFW_KEY_9 && event.idata3 == GLFW_PRESS)
            {
                toggleLight("StandardPointLight");
                return true;
            }

            if (event.idata1 == GLFW_KEY_0 && event.idata3 == GLFW_PRESS)
            {
                toggleLight("StandardSpotLight");
                return true;
            }

            return false;
        }
    };

    ///simple event listener for rotating objects
    class BlinkListener : public VEEventListener
    {
        VEEntity *m_pEntity;
        double t_now = 0.0;
        double t_last = 0.0;
        double m_blinkDuration;

    public:
        ///Constructor
        BlinkListener(std::string name, VEEntity *pEntity, double duration) : VEEventListener(name), m_pEntity(pEntity), m_blinkDuration(duration){};

        ///\brief let cubes blink
        void onFrameStarted(veEvent event)
        {
            t_now += event.dt;
            double duration = t_now - t_last;

            if (duration > m_blinkDuration)
            {
                m_pEntity->m_visible = m_pEntity->m_visible ? false : true; //toggle visibility
                t_last = t_now;
            }
        }

        ///\returns true if this event listener instance should be deleted also
        bool onSceneNodeDeleted(veEvent event)
        {
            if (m_pEntity == event.ptr)
                return true;
            return false;
        };
    };
}

#endif