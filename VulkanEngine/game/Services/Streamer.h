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
        void encodeAndSend(const uint8_t *dataImage, int width, int height);
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

    void Streamer::encodeAndSend(const uint8_t *dataImage, int width, int height)
    {
        char *message = "Hello test";
        m_encoder.setupContexts(width, height);
        //auto frame = m_encoder.encodeImageToFrame(dataImage);
        // auto packet = m_encoder.frameToPacket(frame);
        m_udpSender.init("127.0.0.1", PORT);
        m_udpSender.send(message, sizeof(message));
        m_udpSender.closeSock();
        // m_udpSender.send((char *)packet->data, packet->size);
        // av_packet_unref(packet);
        // av_frame_free(&frame);
        m_encoder.cleanupContexts();
    }
}