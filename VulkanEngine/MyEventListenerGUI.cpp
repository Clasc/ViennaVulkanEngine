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

		struct nk_context* ctx = pSubrender->getContext();
		if (nk_begin(ctx, "Scene Nodes", nk_rect(0, 0, 600, 800), NK_WINDOW_BORDER)) {
			char outbuffer[200];
			nk_layout_row_dynamic(ctx, 45, 1);
			
			auto root = getSceneManagerPointer()->getRootSceneNode();
			sprintf(outbuffer, "Scene Nodes:");
			nk_label(ctx, outbuffer, NK_TEXT_LEFT);
			appendChildNodesToBuffer(outbuffer, root, ctx);
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