#include "VEInclude.h"

#pragma once

#ifndef MYEVENTLISTENERGUI_H
#define	MYEVENTLISTENERGUI_H

namespace ve{

	class MyEventListenerGUI : public VEEventListener {
		public:
			void onDrawOverlay(veEvent event);
			bool onKeyboard(veEvent event);
			MyEventListenerGUI();
			~MyEventListenerGUI();

		private:
			struct uiState { std::string openNode; int pressed = 0; };
			uiState state;
			struct buffers {
				char x[20];
				char y[20];
				char z[20];
			};
			buffers buffers;
			char outbuffer[200];

			inline const static std::string NAME = "MyEventListenerGUI";
			char* nodeTypeToString(VESceneNode::veNodeType nodetype);
			void renderLightSubMenu(nk_context* ctx, VELight* light);
			void renderNodeSubMenu(nk_context* ctx, VESceneNode* node);
			bool isLight(VESceneNode* node);
	};
}
#endif // !MYEVENTLISTENERGUI_H
