#include "Encoder.h"
#include "UDPSend.h"
#include <iostream>

#define PORT 5000

namespace game
{
    class Streamer
    {
    private:
        Encoder m_encoder;
        UDPSend m_udpSender;

    public:
        Streamer(/* args */);
        ~Streamer();
        void encodeAndSend(const uint8_t *dataImage, int width, int height, int position);
    };

    Streamer::Streamer()
    {
        m_encoder = Encoder();
        m_udpSender = UDPSend();
    }

    Streamer::~Streamer()
    {
        m_udpSender.closeSock();
    }

    void Streamer::encodeAndSend(const uint8_t *dataImage, int width, int height, int position)
    {
        m_encoder.setupContexts(width, height);
        auto frame = m_encoder.getFrameFromData(dataImage, position);
        auto yuvFrame = m_encoder.convertRgbToYuv(frame);
        auto packet = m_encoder.frameToPacket(yuvFrame);

        if (!packet)
        {
            return;
        }

        printf("packetdata: %p, packet size: %d\n", packet->data, packet->size);
        m_udpSender.init("127.0.0.1", PORT);
        m_udpSender.send((char *)packet->data, packet->size);
        // m_udpSender.send("Hello", 8);
        m_udpSender.closeSock();

        av_frame_free(&frame);
        // av_packet_unref(packet);
        av_frame_free(&yuvFrame);
        m_encoder.cleanupContexts();
    }
}