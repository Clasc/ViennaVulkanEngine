/*
 *  UDPSend.h
 *  MySender
 *
 *  Created by Helmut Hlavacs on 10.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


 // Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define min(x, y) ((x) <= (y) ? (x) : (y))
#define max(x, y) ((x) >= (y) ? (x) : (y))

#if !defined(_WIN32)
#define SOCKET int
#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#endif

#ifndef UDPSEND_H
#define UDPSEND_H

class UDPSend
{

public:
    SOCKET sock;
    struct sockaddr_in addr;
    unsigned long packetnum;

	UDPSend();
    ~UDPSend()
    {
		if (sock) 
		{
            closesocket(sock);
		}
		WSACleanup();
    };
    void init(char *address, int port);
    int send(char *buffer, int len);
    void closeSock();
	char sendbuffer[65000];
};

typedef struct RTHeader
{
    double time;
    unsigned long packetnum;
    unsigned char fragments;
    unsigned char fragnum;
} RTHeader_t;

UDPSend::UDPSend()
{
    packetnum = 0;
    sock = 0;
	WSADATA wsaData;
	auto wVersionRequested = MAKEWORD(2, 2);
	auto err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);
		return;
	}
}

void UDPSend::init(char *address, int port)
{
	
    if (sock)
    {
		closesocket(sock);
    }

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons(port);
	
	if (!ISVALIDSOCKET(sock)) {
		fprintf(stderr, "socket() failed!\n");
	}
}

int UDPSend::send(char *buffer, int len)
{
    packetnum++;

    if (len > 65000)
    {
        return 0;
    }
    //if( packetnum%100==50) return -1;		//test for packet loss

    int maxpacketsize = 1400;

    RTHeader_t header;
    header.time = clock() / (double)CLOCKS_PER_SEC;
    header.packetnum = packetnum;
    header.fragments = len / maxpacketsize;
    header.fragnum = 1;

    int left = len - header.fragments * maxpacketsize;
	
	if (left > 0) 
	{
        header.fragments++;
	}

    int ret, bytes = 0;
    for (int i = 0; i < header.fragments; i++)
    {
        memcpy(sendbuffer, &header, sizeof(header));
        memcpy(sendbuffer + sizeof(header), buffer + bytes, min(maxpacketsize, len - bytes));

        ret = sendto(sock, sendbuffer, min(maxpacketsize, len - bytes) + sizeof(header), 0, (const struct sockaddr *)&addr, sizeof(addr));

        if (ret == -1)
        {
            std::cout << strerror(errno) << " socket: " << sock << std::endl;
            return ret;
        }

        else
        {
            ret = ret - sizeof(header);
        }
        bytes += ret;
        header.fragnum++;
    }

    return bytes;
}

void UDPSend::closeSock()
{
	closesocket(sock);
    sock = 0;
}
#endif