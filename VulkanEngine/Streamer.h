#include "Encoder.h"
#include <iostream>

#define PORT 5000
#define ADDRESS "127.0.0.1"

namespace game
{
	class Streamer
	{
	private:
		Encoder m_encoder;
		bool isSetup = false;
		UDPSend m_udpSender;

	public:
		Streamer(/* args */);
		~Streamer();
		void setupOnce(int width, int height);
		void cleanup();
		void encodeAndSend(const uint8_t* dataImage, int width, int height, int position);
	};

	Streamer::Streamer()
	{
		m_encoder = Encoder();
		m_udpSender = UDPSend();
	}

	Streamer::~Streamer()
	{
	}

	void Streamer::setupOnce(int width, int height)
	{
		if (!isSetup)
		{
			m_udpSender.init(ADDRESS, PORT);
			m_encoder.setupContexts(width, height);
			isSetup = true;
		}
	}

	void Streamer::cleanup()
	{
		m_encoder.cleanupContexts();
		isSetup = false;
		m_udpSender.closeSock();
	}

	void Streamer::encodeAndSend(const uint8_t* dataImage, int width, int height, int position)
	{
		auto frame = m_encoder.getFrameFromData(dataImage, position);
		auto yuvFrame = m_encoder.convertRgbToYuv(frame);

		m_encoder.encode(yuvFrame, [&](AVPacket * pkt) {
				printf("sending encoded frame...\n\n");
				printf("packet size: %d \n", pkt->size);
				m_udpSender.send((char*)pkt->data, pkt->size);
			});

		av_frame_free(&frame);
		av_frame_free(&yuvFrame);
	}
}