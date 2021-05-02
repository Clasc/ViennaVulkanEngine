#include "../../VEInclude.h"
#include "../Services/Encoder.h"
#include <vector>

#ifndef RECORDER_LISTENER_H
#define RECORDER_LISTENER_H

using namespace ve;

namespace game
{

    class RecorderListener : public VEEventListener
    {
    private:
        inline static const std::string NAME = "RecorderListener";
        inline static const char *FILEPATH = "media/stream/video/out";
        std::vector<uint8_t *> m_frames;
        bool m_is_recording = false;
        int m_num_recordings = 0;
        Encoder m_encoder;

    public:
        RecorderListener();
        ~RecorderListener();
        virtual void onFrameEnded(veEvent event);
        bool onKeyboard(veEvent event);
    };

    RecorderListener::RecorderListener() : VEEventListener(NAME)
    {
        m_encoder = Encoder();
    }

    RecorderListener::~RecorderListener()
    {
    }

    void RecorderListener::onFrameEnded(veEvent event)
    {
        if (!m_is_recording)
        {
            return;
        }

        VkExtent2D extent = getWindowPointer()->getExtent();
        uint32_t imageSize = extent.width * extent.height * 4;
        VkImage image = getRendererPointer()->getSwapChainImage();

        uint8_t *dataImage = new uint8_t[imageSize];

        vh::vhBufCopySwapChainImageToHost(getRendererPointer()->getDevice(),
                                          getRendererPointer()->getVmaAllocator(),
                                          getRendererPointer()->getGraphicsQueue(),
                                          getRendererPointer()->getCommandPool(),
                                          image, VK_FORMAT_R8G8B8A8_UNORM,
                                          VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                          dataImage, extent.width, extent.height, imageSize);

        m_frames.push_back(dataImage);
    }

    bool RecorderListener::onKeyboard(veEvent event)
    {
        bool is_R_pressed = event.idata1 == GLFW_KEY_R && event.idata3 == GLFW_PRESS;

        if (event.idata3 != GLFW_PRESS)
        {
            return false;
        }

        if (is_R_pressed && !m_is_recording)
        {
            m_is_recording = true;
            m_num_recordings++;
            std::cout << "recording started" << std::endl;
            return true;
        }

        if (is_R_pressed && m_is_recording)
        {
            m_is_recording = false;
            auto extent = getEnginePointer()->getWindow()->getExtent();
            m_encoder.setupContexts(extent.width, extent.height);

            auto filename = std::string(FILEPATH) + std::to_string(m_num_recordings) + ".mpg";
            FILE *file = fopen(filename.c_str(), "wb");
            if (!file)
            {
                fprintf(stderr, "could not open %s\n", filename.c_str());
                return false;
            }

            auto counter = 0;

            for (const auto dataImage : m_frames)
            {
                if (!dataImage)
                {
                    continue;
                }
                m_encoder.saveImageBufferToFile(dataImage, file, counter++);
            }

            std::cout << "done saving video!" << std::endl;

            uint8_t endcode[] = {0, 0, 1, 0xb7};
            fwrite(endcode, 1, sizeof(endcode), file);
            m_frames.clear();

            m_encoder.cleanupContexts();
            return true;
        }
    }
}

#endif