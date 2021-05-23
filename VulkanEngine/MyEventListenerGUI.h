#include "VEInclude.h"

#pragma once

#ifndef MYEVENTLISTENERGUI_H
#define	MYEVENTLISTENERGUI_H

namespace ve{

	class MyEventListenerGUI : public VEEventListener {
		public:
			void onDrawOverlay(veEvent event);
			MyEventListenerGUI();
			~MyEventListenerGUI();
			char* nodeTypeToString(VESceneNode::veNodeType nodetype);
			void renderLightSubMenu(nk_context* ctx, VELight* light);
			void renderNodeSubMenu(nk_context* ctx, VESceneNode* node);
			bool isLight(VESceneNode* node);

		private:
			struct uiState { std::string openNode; };
			uiState state;
			inline const static std::string NAME = "MyEventListenerGUI";
	};
}
#endif // !MYEVENTLISTENERGUI_H
