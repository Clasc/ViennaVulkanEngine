#ifndef MY_VULKAN_ENGINE
#define MY_VULKAN_ENGINE
#include "CustomListeners.h"
#include "VEInclude.h"

namespace ve
{
    ///user defined manager class, derived from VEEngine
    class MyVulkanEngine : public VEEngine
    {
    protected:
    public:
        /**
		* \brief Constructor of my engine
		* \param[in] debug Switch debuggin on or off
		*/
        MyVulkanEngine(bool debug = false) : VEEngine(debug){};
        ~MyVulkanEngine(){};

        ///Register an event listener to interact with the user
        virtual void registerEventListeners()
        {
            VEEngine::registerEventListeners();

            registerEventListener(new LevelListener("LevelListener"), {veEvent::VE_EVENT_KEYBOARD});
            registerEventListener(new LightListener("LightListener"), {veEvent::VE_EVENT_KEYBOARD});
        };

        ///create many cubes
        void createCubes(uint32_t n, VESceneNode *parent)
        {

            float stride = 300.0f;
            static std::default_random_engine e{12345};
            static std::uniform_real_distribution<> d{1.0f, stride};
            static std::uniform_real_distribution<> r{-1.0f, 1.0f};
            static std::uniform_real_distribution<> v{0.1f, 5.0f};
            static std::uniform_real_distribution<> dur{900.3f, 1000.0f};

            VEMesh *pMesh;
            VECHECKPOINTER(pMesh = getSceneManagerPointer()->getMesh("media/models/test/crate0/cube.obj/cube"));

            VEMaterial *pMat;
            VECHECKPOINTER(pMat = getSceneManagerPointer()->getMaterial("media/models/test/crate0/cube.obj/cube"));

            for (uint32_t i = 0; i < n; i++)
            {
                VESceneNode *pNode;
                VECHECKPOINTER(pNode = getSceneManagerPointer()->createSceneNode("The Node" + std::to_string(i), parent));
                pNode->setTransform(glm::translate(glm::mat4(1.0f), glm::vec3(d(e) - stride / 2.0f, d(e) / 2.0f, d(e) - stride / 2.0f)));
                registerEventListener(new RotatorListener("RotatorListener" + std::to_string(i), pNode, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f)), {veEvent::VE_EVENT_FRAME_STARTED, veEvent::VE_EVENT_DELETE_NODE});

                VEEntity *e2;
                VECHECKPOINTER(e2 = getSceneManagerPointer()->createEntity("The Cube" + std::to_string(i), pMesh, pMat, pNode));
                //registerEventListener(new RotatorListener("RotatorListener2" + std::to_string(i), e2, v(e), glm::vec3(r(e), r(e), r(e))), {veEvent::VE_EVENT_FRAME_STARTED, veEvent::VE_EVENT_DELETE_NODE});
                //registerEventListener( new BlinkListener("BlinkListener" + std::to_string(i), e2, dur(e) ), { veEvent::VE_EVENT_FRAME_STARTED, veEvent::VE_EVENT_DELETE_NODE });
            }
        }

        ///create many lights
        void createLights(uint32_t n, VESceneNode *parent)
        {
            float stride = 200.0f;
            static std::default_random_engine e{12345};
            static std::uniform_real_distribution<> d{1.0f, stride};

            for (uint32_t i = 0; i < n; i++)
            {
                VELight *pLight;
                VECHECKPOINTER(pLight = getSceneManagerPointer()->createLight("Light" + std::to_string(i), VELight::VE_LIGHT_TYPE_SPOT, parent));
                pLight->m_col_diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
                pLight->m_col_specular = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
                pLight->m_param[0] = stride / 3.0f;
                //pLight->multiplyTransform(glm::translate(glm::vec3(d(e) - stride / 2.0f, d(e)/10.0f, d(e) - stride / 2.0f)));

                pLight->lookAt(glm::vec3(d(e) - stride / 2.0f, d(e) / 30.0f, d(e) - stride / 2.0f),
                               glm::vec3(d(e) - stride / 2.0f, 0.0f, d(e) - stride / 2.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }

        ///Load the first level into the game engine
        ///The engine uses Y-UP, Left-handed
        virtual void loadLevel(uint32_t numLevel = 1)
        {

            VEEngine::loadLevel(numLevel); //create standard cameras and lights

            VESceneNode *pScene;
            VECHECKPOINTER(pScene = getSceneManagerPointer()->createSceneNode("Level 1", getRoot()));

            //scene models

            /*
            VESceneNode *sp1;
			VECHECKPOINTER( sp1 = getSceneManagerPointer()->createSkybox("The Sky", "media/models/test/sky/cloudy",
										{	"bluecloud_ft.jpg", "bluecloud_bk.jpg", "bluecloud_up.jpg",
											"bluecloud_dn.jpg", "bluecloud_rt.jpg", "bluecloud_lf.jpg" }, pScene)  );

			RotatorListener *pRot;
			VECHECKPOINTER( pRot = new RotatorListener("CubemapRotator", sp1, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f)) );
			getEnginePointer()->registerEventListener(pRot, { veEvent::VE_EVENT_DELETE_NODE, veEvent::VE_EVENT_FRAME_STARTED });
            */

            VESceneNode *e4;
            VECHECKPOINTER(e4 = getSceneManagerPointer()->loadModel("The Plane", "media/models/test", "plane_t_n_s.obj", 0, pScene));
            e4->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

            VEEntity *pE4;
            VECHECKPOINTER(pE4 = (VEEntity *)getSceneManagerPointer()->getSceneNode("The Plane/plane_t_n_s.obj/plane/Entity_0"));
            pE4->setParam(glm::vec4(1000.0f, 1000.0f, 0.0f, 0.0f));

            VESceneNode *pointLight = getSceneManagerPointer()->getSceneNode("StandardPointLight");
            if (pointLight != nullptr)
            {
                VESceneNode *eL;
                VECHECKPOINTER(eL = getSceneManagerPointer()->loadModel("The Light", "media/models/test/sphere", "sphere.obj", 0, pointLight));
                eL->multiplyTransform(glm::scale(glm::vec3(0.02f, 0.02f, 0.02f)));

                VEEntity *pE;
                VECHECKPOINTER(pE = (VEEntity *)getSceneManagerPointer()->getSceneNode("The Light/sphere.obj/default/Entity_0"));
                pE->m_castsShadow = false;
            }

            VESceneNode *e1;
            VECHECKPOINTER(e1 = getSceneManagerPointer()->loadModel("The Cube", "media/models/test/crate0", "cube.obj"));
            e1->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)));
            e1->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 5.0f, 10.0f)));
            pScene->addChild(e1);

            createCubes(10, pScene);
            //createLights(10, pScene );
            //VESceneNode *pSponza = m_pSceneManager->loadModel("Sponza", "models/sponza", "sponza.dae", aiProcess_FlipWindingOrder);
            //pSponza->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
        };
    };
}
#endif