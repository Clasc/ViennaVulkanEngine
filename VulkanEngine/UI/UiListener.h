#ifndef UI_LISTENER_H
#define UI_LISTENER_H
#include "../VEInclude.h"
#include "EngineUI.h"
using namespace ui;

namespace ve
{
    class UiListener : public VEEventListener
    {
    private:
        inline static const std::string NAME = "UiListener";
        EngineUI ui;

    public:
        UiListener();
        ~UiListener();
        void onFrameEnded(veEvent event);
    };

    UiListener::UiListener() : VEEventListener(NAME)
    {
        ui = EngineUI();
    }

    UiListener::~UiListener()
    {
    }

    void UiListener::onFrameEnded(veEvent event)
    {
        ui.render();
    }
} // namespace ve
#endif