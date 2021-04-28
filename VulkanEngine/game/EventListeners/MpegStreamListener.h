#include "../../VEInclude.h"
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
        uint32_t m_numScreenshot = 0;
        AVCodecContext *m_avcodec_context;
        AVCodecID codecId = AV_CODEC_ID_MPEG4;

    public:
        MpegStreamListener();
        ~MpegStreamListener();
        virtual void onFrameEnded(veEvent event);
        void encode(AVFrame *frame, AVPacket *pkt, FILE *outfile);
    };

    MpegStreamListener::MpegStreamListener() : VEEventListener(NAME)
    {
        const AVCodec *codec = avcodec_find_encoder(codecId);
        m_avcodec_context = avcodec_alloc_context3(codec);

        m_avcodec_context->bit_rate = 400000;

        // resolution must be a multiple of two
        m_avcodec_context->width = 352;
        m_avcodec_context->height = 288;

        // frames per second
        m_avcodec_context->time_base.num = 1;
        m_avcodec_context->time_base.den = 25;
        m_avcodec_context->framerate.num = 25;
        m_avcodec_context->framerate.den = 1;

        m_avcodec_context->gop_size = 10; // emit one intra frame every ten frames
        m_avcodec_context->max_b_frames = 1;
        m_avcodec_context->pix_fmt = AV_PIX_FMT_YUV420P;

        if (avcodec_open2(m_avcodec_context, codec, NULL) < 0)
        {
            fprintf(stderr, "could not open codec\n");
            exit(1);
        }
    }

    MpegStreamListener::~MpegStreamListener()
    {
    }

    void MpegStreamListener::encode(AVFrame *frame, AVPacket *pkt, FILE *outfile)
    {
        int ret;

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

        auto frame = av_frame_alloc();
        struct SwsContext *img_convert_ctx = sws_getContext(m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_YUV420P, m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
        avpicture_fill((AVPicture *)frame, dataImage, AV_PIX_FMT_RGB24, m_avcodec_context->width, m_avcodec_context->height);
        sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, m_avcodec_context->height, frame->data, frame->linesize);

        // encode the image
        encode(frame, pkt, f);

        delete[] dataImage;
    }

}
#endif