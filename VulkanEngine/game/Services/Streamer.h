#include "Encoder.h"
#include <iostream>

namespace game
{
    class Streamer
    {
    private:
        Encoder m_encoder;
        bool isSetup = false;

    public:
        Streamer(/* args */);
        ~Streamer();
        void setupOnce(int width, int height);
        void cleanup();
        void encodeAndSend(const uint8_t *dataImage, int width, int height, int position);
    };

    Streamer::Streamer()
    {
        m_encoder = Encoder();
    }

    Streamer::~Streamer()
    {
    }

    void Streamer::setupOnce(int width, int height)
    {
        if (!isSetup)
        {
            m_encoder.setupContexts(width, height);
            isSetup = true;
        }
    }

    void Streamer::cleanup()
    {
        m_encoder.cleanupContexts();
        isSetup = false;
    }

    void Streamer::encodeAndSend(const uint8_t *dataImage, int width, int height, int position)
    {
        auto frame = m_encoder.getFrameFromData(dataImage, position);
        auto yuvFrame = m_encoder.convertRgbToYuv(frame);
        m_encoder.encodeFrameAndSend(yuvFrame);

        av_frame_free(&frame);
        av_frame_free(&yuvFrame);
    }
}