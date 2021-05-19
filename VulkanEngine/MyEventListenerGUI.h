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

		private:
			inline const static std::string NAME = "MyEventListenerGUI";
			void appendChildNodesToBuffer(char* outbuffer, VESceneNode* node,  nk_context* ctx);
	};
}
#endif // !MYEVENTLISTENERGUI_H
