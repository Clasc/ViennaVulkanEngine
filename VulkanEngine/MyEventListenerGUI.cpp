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
			if (nk_begin(ctx, "Scene Nodes", nk_rect(0, 0, 200, 170), NK_WINDOW_BORDER)) {
				char outbuffer[100];
				nk_layout_row_dynamic(ctx, 45, 1);
				sprintf(outbuffer, "Hello World!");
				nk_label(ctx, outbuffer, NK_TEXT_LEFT);
			}
			
			nk_end(ctx);
	}
}