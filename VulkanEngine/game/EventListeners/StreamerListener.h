#include "../../VEInclude.h"
#include "../Services/Encoder.h"
#include "../Services/Streamer.h"

#ifndef STREAMER_LISTEMER_H
#define STREAMER_LISTEMER_H

using namespace ve;

namespace game
{
    class StreamerListener : public VEEventListener
    {
    private:
        inline static const std::string NAME = "StreamerListener";
        Streamer m_streamer;
        int m_frames_catpured = 0;

    public:
        StreamerListener();
        ~StreamerListener();
        void onFrameEnded(veEvent event);
    };

    StreamerListener::StreamerListener() : VEEventListener(NAME)
    {
        m_streamer = Streamer();
    }

    StreamerListener::~StreamerListener()
    {
        m_streamer.cleanup();
    }

    void StreamerListener::onFrameEnded(veEvent event)
    {
        VkExtent2D extent = getWindowPointer()->getExtent();
        uint32_t imageSize = extent.width * extent.height * 4;
        VkImage image = getRendererPointer()->getSwapChainImage();
        m_streamer.setupOnce(extent.width, extent.height);

        uint8_t *dataImage = new uint8_t[imageSize];

        vh::vhBufCopySwapChainImageToHost(getRendererPointer()->getDevice(),
                                          getRendererPointer()->getVmaAllocator(),
                                          getRendererPointer()->getGraphicsQueue(),
                                          getRendererPointer()->getCommandPool(),
                                          image, VK_FORMAT_R8G8B8A8_UNORM,
                                          VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                          dataImage, extent.width, extent.height, imageSize);

        m_streamer.encodeAndSend(dataImage, extent.width, extent.height, m_frames_catpured++);
    }
}
#endif