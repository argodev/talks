// ReadPcap.cpp : Defines the entry point for the console application.
//
// NOTE: Please do not copy this code!!! This is for your own sanity.
// I am taking a number of shortcuts for the purposes of a very 
// constrained demo that will bite you if you try to use this on
// real data on a real network (think: vlan headers amongst other things).
// 

#include "stdafx.h"
#include "pcap.h"		// winpcap
#include "winsock2.h"	//need winsock for inet_ntoa and ntohs methods
#include <iostream>
#include <array>
#include <bitset>

using namespace std;

// some interesting examples
// http://www.binarytides.com/code-packet-sniffer-c-winpcap/

// why am I doing this?
#pragma comment(lib , "ws2_32.lib") //For winsock
#pragma comment(lib , "wpcap.lib") //For winpcap


//Ethernet Header
typedef struct eth_header
{
	UCHAR dest[6];
	UCHAR source[6];
	USHORT type;
}   eth_header, *PETHER_HDR, FAR * LPETHER_HDR, ETHERHeader;

/* 4 bytes IP address */
typedef struct ip_address{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header{
	u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
	u_char  tos;            // Type of service 
	u_short tlen;           // Total length 
	u_short identification; // Identification
	u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
	ip_address  saddr;      // Source address
	ip_address  daddr;      // Destination address
	u_int   op_pad;         // Option + Padding
}ip_header;


/* prototype of the packet handler */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

int total = 0;
array<bool, 24> message = { false };

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char **argv)
{
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (argc != 2)
	{
		printf("usage: %s filename", argv[0]);
		return -1;
	}

	/* Open the capture file */
	if ((fp = pcap_open_offline(argv[1],	// name of the device
		errbuf								// error buffer
		)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the file %s.\n", argv[1]);
		return -1;
	}

	// read and dispatch packets until EOF is reached
	pcap_loop(fp, 0, packet_handler, NULL);

	pcap_close(fp);

	// OK, now can we re-assemble our message?
	bitset<8> a1;
	bitset<8> a2;
	bitset<8> a3;

	// set the bits
	for (int i = 0; i < 8; i++)
	{
		a1.set(i, message[i]);
		a2.set(i, message[i + 8]);
		a3.set(i, message[i + 16]);
	}

	unsigned char a11 = static_cast<unsigned char>(a1.to_ulong());
	unsigned char a22 = static_cast<unsigned char>(a2.to_ulong());
	unsigned char a33 = static_cast<unsigned char>(a3.to_ulong());

	cout << "Message: " << a11 << a22 << a33 << endl;
	return 0;
}


/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	eth_header *eh;
	ip_header *ih;
	u_int s_b4;

	// Unused variable
	(VOID)(param);

	//Ethernet header
	eh = (eth_header *)pkt_data;

	//Ip packets
	if (ntohs(eh->type) == 0x0800)
	{
		// retrieve the position of the ip header
		ih = (ip_header *)(pkt_data + 14); // length of ethernet header
		s_b4 = (unsigned int)ih->saddr.byte4;

		// if the traffic is originating from our "sender"...
		if (s_b4 == 22)
		{
			if (total < message.size())
			{
				// get the most significant bit (MSB)
				// and update the slot in the array
				int v3 = (((unsigned int)ih->ttl) >> 7) & 0x1;
				message[total] = v3;
			}

			++total;
		}
	}
}
