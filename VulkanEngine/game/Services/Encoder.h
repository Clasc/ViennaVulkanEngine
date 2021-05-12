extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

#include "UDPSend.h"

#ifndef ENCODER
#define ENCODER

#define PORT 5000
#define ADDRESS "127.0.0.1"

namespace game
{
    class Encoder
    {
    private:
        static const size_t BIT_RATE = 4000000;
        static const AVCodecID CODEC = AV_CODEC_ID_MPEG2VIDEO;

        UDPSend m_udpSender;
        AVCodecContext *m_avcodec_context;
        SwsContext *m_img_convert_ctx;
        AVCodec *m_codec;

    public:
        Encoder();
        ~Encoder();
        void encode(AVFrame *frame, std::function<void(AVPacket *packet)> callback);
        void setupContexts(size_t width, size_t height);
        void saveImageBufferToFile(const uint8_t *dataImage, FILE *f, int position);
        AVFrame *getFrameFromData(const uint8_t *dataImage, int position);
        void encodeFrameAndSend(AVFrame *frame);
        AVFrame *convertRgbToYuv(AVFrame *frame);
        void cleanupContexts();
    };

    Encoder::Encoder()
    {
        m_udpSender = UDPSend();
    }

    Encoder::~Encoder()
    {
    }

    void Encoder::setupContexts(size_t width, size_t height)
    {
        m_codec = avcodec_find_encoder(CODEC);

        m_avcodec_context = avcodec_alloc_context3(m_codec);

        m_avcodec_context->bit_rate = BIT_RATE;

        m_avcodec_context->width = width;
        m_avcodec_context->height = height;

        // frames per second
        m_avcodec_context->time_base.num = 1;
        m_avcodec_context->time_base.den = 25;
        m_avcodec_context->framerate.num = 25;
        m_avcodec_context->framerate.den = 1;

        m_avcodec_context->gop_size = 10; // emit one intra frame every ten frames
        m_avcodec_context->max_b_frames = 1;
        m_avcodec_context->pix_fmt = AV_PIX_FMT_YUV420P;

        if (avcodec_open2(m_avcodec_context, m_codec, NULL) < 0)
        {
            fprintf(stderr, "could not open codec\n");
            exit(1);
        }

        m_img_convert_ctx = sws_getContext(m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_RGBA, m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_YUV420P, 0, NULL, NULL, NULL);
        if (!m_img_convert_ctx)
        {
            fprintf(stderr, "error creating swsContext");
            exit(1);
        }
    }

    void Encoder::encodeFrameAndSend(AVFrame *frame)
    {
        encode(frame, [&](AVPacket *pkt) {
            fprintf(stderr, "sending encoded frame...\n");
            m_udpSender.init(ADDRESS, PORT);
            m_udpSender.send((char *)pkt->data, pkt->size);
            m_udpSender.closeSock();
        });
    }

    void Encoder::encode(AVFrame *frame, std::function<void(AVPacket *packet)> callback)
    {

        auto pkt = av_packet_alloc();
        if (!pkt)
        {
            fprintf(stderr, "Cannot alloc packet\n");
            exit(1);
        }

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

            callback(pkt);
            av_packet_unref(pkt);
        }

        av_frame_free(&frame);
    }

    void Encoder::cleanupContexts()
    {
        avcodec_free_context(&m_avcodec_context);
        sws_freeContext(m_img_convert_ctx);
    }

    AVFrame *Encoder::getFrameFromData(const uint8_t *dataImage, int position)
    {

        auto frame = av_frame_alloc();
        frame->format = AV_PIX_FMT_RGBA;
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

        av_image_fill_arrays(frame->data, frame->linesize, dataImage, AV_PIX_FMT_RGBA, m_avcodec_context->width, m_avcodec_context->height, 1);
        frame->pts = position;
        return frame;
    }

    void Encoder::saveImageBufferToFile(const uint8_t *dataImage, FILE *f, int position)
    {
        fflush(stdout);

        auto rgbFrame = getFrameFromData(dataImage, position);
        auto frame = convertRgbToYuv(rgbFrame);

        encode(frame, [f](AVPacket *pkt) {
            fwrite(pkt->data, 1, pkt->size, f);
        });
    }

    AVFrame *Encoder::convertRgbToYuv(AVFrame *rgbFrame)
    {
        auto result = av_frame_alloc();
        result->format = m_avcodec_context->pix_fmt;
        result->width = m_avcodec_context->width;
        result->height = m_avcodec_context->height;

        if (av_frame_get_buffer(result, 32) < 0)
        {
            fprintf(stderr, "could not alloc the frame data\n");
            exit(1);
        }

        if (av_frame_make_writable(result) < 0)
        {
            fprintf(stderr, "Cannot make frame writeable\n");
            exit(1);
        }

        sws_scale(m_img_convert_ctx, (const uint8_t **)rgbFrame->data, rgbFrame->linesize, 0, m_avcodec_context->height,
                  result->data, result->linesize);

        result->pts = rgbFrame->pts;

        return result;
    }
}
#endif