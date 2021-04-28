#include "../../VEInclude.h"
#include <vector>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

#ifndef MPEG_STREAM_LISTENER_H
#define MPEG_STREAM_LISTENER_H

using namespace ve;

namespace game
{

    class MpegStreamListener : public VEEventListener
    {
    private:
        inline static const std::string NAME = "MpegStreamListener";
        inline static const char *FILENAME = "media/stream/video/out.mpg";
        uint32_t m_numScreenshot = 0;
        AVCodecContext *m_avcodec_context;
        AVCodecID codecId = AV_CODEC_ID_MPEG1VIDEO;
        AVCodec *m_codec;
        std::vector<uint8_t *> m_frames;

    public:
        MpegStreamListener();
        ~MpegStreamListener();
        virtual void onFrameEnded(veEvent event);
        void encode(AVFrame *frame, AVPacket *pkt, FILE *outfile);
        bool onKeyboard(veEvent event);
    };

    MpegStreamListener::MpegStreamListener() : VEEventListener(NAME)
    {
        m_codec = avcodec_find_encoder(codecId);
        m_avcodec_context = avcodec_alloc_context3(m_codec);

        m_avcodec_context->bit_rate = 400000;

        // resolution must be a multiple of two
        auto extent = getEnginePointer()->getWindow()->getExtent();

        m_avcodec_context->width = extent.width;
        m_avcodec_context->height = extent.height;

        // frames per second
        m_avcodec_context->time_base.num = 1;
        m_avcodec_context->time_base.den = 25;
        m_avcodec_context->framerate.num = 25;
        m_avcodec_context->framerate.den = 1;

        m_avcodec_context->gop_size = 10; // emit one intra frame every ten frames
        m_avcodec_context->max_b_frames = 1;
        m_avcodec_context->pix_fmt = AV_PIX_FMT_YUV420P;
    }

    MpegStreamListener::~MpegStreamListener()
    {
    }

    void MpegStreamListener::encode(AVFrame *frame, AVPacket *pkt, FILE *outfile)
    {
        int ret;

        if (avcodec_open2(m_avcodec_context, m_codec, NULL) < 0)
        {
            fprintf(stderr, "could not open codec\n");
            exit(1);
        }

        // send the frame to the encoder */
        ret = avcodec_send_frame(m_avcodec_context, frame);
        if (ret < 0)
        {
            fprintf(stderr, "error sending a frame for encoding\n");
            exit(1);
        }

        while (ret >= 0)
        {
            int ret = avcodec_receive_packet(m_avcodec_context, pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                return;
            else if (ret < 0)
            {
                fprintf(stderr, "error during encoding\n");
                exit(1);
            }

            printf("encoded frame %lld (size=%5d)\n", pkt->pts, pkt->size);
            fwrite(pkt->data, 1, pkt->size, outfile);
            av_packet_unref(pkt);
        }
    }

    void MpegStreamListener::onFrameEnded(veEvent event)
    {
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

    bool MpegStreamListener::onKeyboard(veEvent event)
    {
        if (event.idata1 != GLFW_KEY_R)
        {
            // if R is not pressed return
            return false;
        }
        // if R is pressed save recording

        FILE *f = fopen(FILENAME, "wb");
        if (!f)
        {
            fprintf(stderr, "could not open %s\n", FILENAME);
            exit(1);
        }

        auto dataImage = m_frames[0];
        if (!dataImage)
        {
            return false;
        }

        auto frame = av_frame_alloc();
        frame->format = m_avcodec_context->pix_fmt;
        frame->width = m_avcodec_context->width;
        frame->height = m_avcodec_context->height;

        if (av_frame_get_buffer(frame, 32) < 0)
        {
            fprintf(stderr, "could not alloc the frame data\n");
            exit(1);
        }

        if (av_frame_make_writable(frame) < 0)
        {
            fprintf(stderr, "Cannot make frame writeable\n");
            exit(1);
        }

        auto pkt = av_packet_alloc();
        if (!pkt)
        {
            fprintf(stderr, "Cannot alloc packet\n");
            exit(1);
        }

        av_image_fill_arrays(frame->data, frame->linesize, dataImage, AV_PIX_FMT_YUV420P, m_avcodec_context->width, m_avcodec_context->height, 1);

        std::cout << "after fill arrays" << std::endl;

        // struct SwsContext *img_convert_ctx = sws_getContext(m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_RGBA, m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
        // sws_scale(img_convert_ctx, (const uint8_t **)frame->data, frame->linesize, 0, m_avcodec_context->height,
        //           frame->data, frame->linesize);

        encode(frame, pkt, f);

        m_frames.clear();

        return true;
    }
}

#endif