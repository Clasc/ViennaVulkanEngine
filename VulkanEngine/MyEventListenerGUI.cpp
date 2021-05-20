#include "MyEventListenerGUI.h"
namespace ve {
	MyEventListenerGUI::MyEventListenerGUI():VEEventListener(NAME)
	{
	}

	MyEventListenerGUI::~MyEventListenerGUI()
	{
	}

	void MyEventListenerGUI::onDrawOverlay(veEvent event) {

		VESubrenderFW_Nuklear* pSubrender = (VESubrenderFW_Nuklear*)getRendererPointer()->getOverlay();
		if (pSubrender == nullptr) {
			return;
		}

		auto ctx = pSubrender->getContext();


		if (nk_begin_titled(ctx, "Scene Nodes","Scene Nodes", nk_rect(0, 0, 600, 800), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE)) {
			char outbuffer[200];
			nk_layout_row_dynamic(ctx, 45, 1);
			
			//TODO: use instead getSceneManagerPointer()->createSceneNodeList()

			auto root = getSceneManagerPointer()->getRootSceneNode();
			sprintf(outbuffer, "Scene Nodes:");
			nk_label(ctx, outbuffer, NK_TEXT_LEFT);
			appendChildNodesToBuffer(outbuffer, root, ctx);
		}
		
		nk_end(ctx);


		if (nk_begin_titled(ctx, "Light Color", "Light Color", nk_rect(800, 0, 300, 300), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE)) {
			nk_color lightColor = {0,0,0,100};
			float mix;
			float fov;
			nk_property_float(ctx, "mix", 0.0f, &mix, 1.0f, 0.1f, 0.01f);
			nk_property_float(ctx, "fov", 0.25f, &fov, 110.0f, 1.0f, 1.0f);
			nk_label(ctx, "Light Color", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 250, 1);
			nk_color_pick(ctx, &lightColor, NK_RGBA);

			auto lights = getSceneManagerPointer()->getLights();
			printf("red: %d \n", lightColor.r / 255);
			
			for (auto light : lights) {
				light->m_col_ambient = { float(lightColor.r) / 255, float(lightColor.g) / 255, float(lightColor.b) / 255, float(lightColor.a) / 255 };
				light->m_col_diffuse = { float(lightColor.r) / 255, float(lightColor.g) / 255, float(lightColor.b) / 255, float(lightColor.a) / 255 };
				light->m_col_specular = { float(lightColor.r) / 255, float(lightColor.g) / 255, float(lightColor.b) / 255, float(lightColor.a) / 255 };
			}
		}
		
		nk_end(ctx);

	}

	void MyEventListenerGUI::appendChildNodesToBuffer(char* outbuffer,VESceneNode* node, nk_context* ctx) {
		if (!node) {
			return;
		}

		sprintf(outbuffer, node->getName().c_str());
		nk_label(ctx, outbuffer, NK_TEXT_LEFT);

		for (const auto child : node->getChildrenList())
		{
			appendChildNodesToBuffer(outbuffer, child, ctx);
		}
	}

}