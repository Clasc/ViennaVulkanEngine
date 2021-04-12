#include "../../VEInclude.h"
#include "../Const/EntityName.h"

#ifndef COLLISIONLISTENER_H
#define COLLISIONLISTENER_H

using namespace ve;

namespace game
{

    class CollisionListener : public VEEventListener
    {
    private:
        inline static const std::string NAME = "CollisionListener";
        bool _gameWon = false;

    protected:
        virtual void onFrameStarted(veEvent event)
        {
            auto fruitPosition = getSceneManagerPointer()->getSceneNode(EntityName::Fruit1)->getPosition();
            auto playerPosition = getSceneManagerPointer()->getSceneNode(EntityName::PlayerParent)->getPosition();

            float distance = glm::length(fruitPosition - playerPosition);

            if (distance < 4)
            {
                _gameWon = true;
                getEnginePointer()->end();
            }
        };

    public:
        ///Constructor of class EventListenerCollision
        CollisionListener() : VEEventListener(NAME){};

        ///Destructor of class EventListenerCollision
        virtual ~CollisionListener(){};
    };
}
#endif