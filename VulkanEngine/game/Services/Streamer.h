#include "Encoder.h"
#include <iostream>

namespace game
{
    class Streamer
    {
    private:
        Encoder m_encoder;

    public:
        Streamer(/* args */);
        ~Streamer();

        void encodeAndSend(const uint8_t *dataImage, int width, int height, int position);
    };

    Streamer::Streamer()
    {
        m_encoder = Encoder();
    }

    Streamer::~Streamer()
    {
    }

    void Streamer::encodeAndSend(const uint8_t *dataImage, int width, int height, int position)
    {
        m_encoder.setupContexts(width, height);
        auto frame = m_encoder.getFrameFromData(dataImage, position);
        auto yuvFrame = m_encoder.convertRgbToYuv(frame);
        m_encoder.encodeFrameAndSend(yuvFrame);

        // m_udpSender.send("Hello", 8);

        av_frame_free(&frame);
        // av_packet_unref(packet);
        av_frame_free(&yuvFrame);
        m_encoder.cleanupContexts();
    }
}