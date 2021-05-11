#include "Encoder.h"
#include "UDPSender.h"
#include <iostream>

#define PORT 5000

namespace game
{
    class Streamer
    {
    private:
        Encoder m_encoder;
        UDPSender m_udpSender;

    public:
        Streamer(/* args */);
        ~Streamer();
        void encodeAndSend(const uint8_t *dataImage, int width, int height);
    };

    Streamer::Streamer()
    {
        m_encoder = Encoder();
        m_udpSender = UDPSender();
        m_udpSender.init("127.0.0.1", PORT);
    }

    Streamer::~Streamer()
    {
    }

    void Streamer::encodeAndSend(const uint8_t *dataImage, int width, int height)
    {
        char *message = "Hello World!";
        m_encoder.setupContexts(width, height);
        auto frame = m_encoder.encodeImageToFrame(dataImage);
        m_udpSender.send(message, sizeof(message));
        m_udpSender.closeSock();
        m_encoder.cleanupContexts();
    }
}