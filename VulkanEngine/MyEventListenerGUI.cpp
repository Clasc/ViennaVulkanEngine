#include "MyEventListenerGUI.h"
namespace ve {
	MyEventListenerGUI::MyEventListenerGUI():VEEventListener(NAME)
	{
		state = {
			"" ,
			-1
		};
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

		if (nk_begin_titled(ctx, "Scene Nodes","Scene Nodes", nk_rect(0, 0, 600, 800), NK_WINDOW_TITLE | NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_SCALABLE)) {
			nk_input_unicode(ctx, state.pressed);
			state.pressed = -1;

			std::vector<std::string> nodeList;
			auto root = getSceneManagerPointer()->getRootSceneNode();
			getSceneManagerPointer()->createSceneNodeList(root, nodeList);

			for (auto n : nodeList) {
				nk_layout_row_dynamic(ctx, 45, 1);
				sprintf(outbuffer, n.c_str());
				
				if (nk_button_label(ctx, outbuffer)) {
					if (state.openNode.compare(n) == 0) {
						state.openNode = "";
					}
					else {
						state.openNode = n;
					}
				}

				if (state.openNode.compare(n) == 0) {
					auto node = getSceneManagerPointer()->getSceneNode(n);
					if (node == nullptr) {
						continue;
					}
					sprintf(outbuffer, nodeTypeToString(node->getNodeType()));
					nk_label(ctx, outbuffer, NK_TEXT_CENTERED);

					if (isLight(node)) {
						renderLightSubMenu(ctx, (VELight*)node);
					}
					else {
						renderNodeSubMenu(ctx, node);
					}
				}
			}
		}
		nk_end(ctx);
	}

	bool  MyEventListenerGUI::onKeyboard(veEvent event) {
		if (event.idata3 == GLFW_PRESS) {
			state.pressed = event.idata1;
		}
		return true;
	}

	char* MyEventListenerGUI::nodeTypeToString(VESceneNode::veNodeType nodetype) {
		switch (nodetype) {
		case VESceneNode::veNodeType::VE_NODE_TYPE_SCENENODE: {
			return "Scene Node";
		};
		case VESceneNode::veNodeType::VE_NODE_TYPE_SCENEOBJECT: {
			return "Scene Object";
		};
		default:return "unknown";
		}
	}

	void MyEventListenerGUI::renderNodeSubMenu(nk_context* ctx, VESceneNode* node) {
		char outbuffer[200];
		nk_layout_row_dynamic(ctx, 45, 6);

		auto pos = node->getPosition();

		sprintf(outbuffer, "x:");
		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
		auto event = nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buffers.x, sizeof(buffers.x) - 1, nk_filter_ascii);
		if (event & NK_EDIT_INACTIVE) {
			strcpy(buffers.x, std::to_string(node->getPosition().x).c_str());
		}
		if (event & NK_EDIT_ACTIVE) {
			pos.x = (float)strtod(buffers.x, NULL);
			node->setPosition(pos);
		}
		

		sprintf(outbuffer, "y:");
		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
		event = nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buffers.y, sizeof(buffers.y) - 1, nk_filter_ascii);
		if (event & NK_EDIT_INACTIVE) {
			strcpy(buffers.y, std::to_string(node->getPosition().y).c_str());
		}
		if (event & NK_EDIT_ACTIVE) {
			pos.y = (float)strtod(buffers.y, NULL);
			node->setPosition(pos);
		}

		sprintf(outbuffer, "z:");
		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
		event = nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buffers.z, sizeof(buffers.z) - 1, nk_filter_ascii);
		if (event & NK_EDIT_INACTIVE) {
			strcpy(buffers.z, std::to_string(node->getPosition().z).c_str());
		}
		if (event & NK_EDIT_ACTIVE) {
			pos.z = (float)strtod(buffers.z, NULL);
			node->setPosition(pos);
		}
	}
	
	void MyEventListenerGUI::renderLightSubMenu(nk_context* ctx, VELight* light) {
		char outbuffer[200];
		nk_layout_row_dynamic(ctx, 45, 1);
		
		auto originalCol = light->m_col_ambient;
		nk_color lightColor = { originalCol.r * 255,originalCol.g * 255, originalCol.b * 255, originalCol.a * 255, };
		nk_label(ctx, outbuffer, NK_TEXT_LEFT);
		nk_label(ctx, "Ambient Light Color", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 250, 1);
		nk_color_pick(ctx, &lightColor, NK_RGBA);

		light->m_col_ambient = { float(lightColor.r) / 255, float(lightColor.g) / 255, float(lightColor.b) / 255, float(lightColor.a) / 255 };
		//light->m_col_diffuse = { float(lightColor.r) / 255, float(lightColor.g) / 255, float(lightColor.b) / 255, float(lightColor.a) / 255 };
		//light->m_col_specular = { float(lightColor.r) / 255, float(lightColor.g) / 255, float(lightColor.b) / 255, float(lightColor.a) / 255 };	
	}

	bool MyEventListenerGUI::isLight(VESceneNode* node) {
		auto lights = getSceneManagerPointer()->getLights();
		auto count = 0;
		for (auto n : lights) {
			if (n->getName().compare(node->getName()) == 0) {
				return true;
			}
		}
		return false;
	}
}