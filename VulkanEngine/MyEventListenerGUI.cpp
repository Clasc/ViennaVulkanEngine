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
		if (nk_begin(ctx, "Scene Nodes", nk_rect(0, 0, 300, 170), NK_WINDOW_BORDER)) {
			char outbuffer[10000];
			nk_layout_row_dynamic(ctx, 45, 1);
			sprintf(outbuffer, "Hello World!");
			nk_label(ctx, outbuffer, NK_TEXT_LEFT);
			
			auto root = getSceneManagerPointer()->getRootSceneNode();
			nk_label(ctx, "Scene Nodes:", NK_TEXT_LEFT);
			appendChildNodesToBuffer(outbuffer, root, ctx);
		}

		nk_end(ctx);
	}

	void MyEventListenerGUI::appendChildNodesToBuffer(char* outbuffer,VESceneNode* node, nk_context* ctx) {
		if (!node) {
			sprintf(outbuffer, "EndOfTree");
			nk_label(ctx, outbuffer, NK_TEXT_LEFT);
			return;
		}

		for (const auto child : node->getChildrenList())
		{
			sprintf(outbuffer, child->getName().c_str());
			nk_label(ctx, outbuffer, NK_TEXT_LEFT);
			appendChildNodesToBuffer(outbuffer, child, ctx);
		}
	}

}