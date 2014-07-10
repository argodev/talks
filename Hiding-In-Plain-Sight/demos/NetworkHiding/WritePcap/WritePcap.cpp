// WritePcap.cpp : Defines the entry point for the console application.
//
// NOTE: Please do not copy this code!!! This is for your own sanity.
// I am taking a number of shortcuts for the purposes of a very 
// constrained demo that will bite you if you try to use this on
// real data on a real network (think: vlan headers amongst other things).
// 
// purpose: read in a pcap, modify the MSB (most significant bit) of the
// IP packets from a given host (10.10.10.22) in order to simulate 
// "hiding" data
//

#include "stdafx.h"
#include "pcap.h"		// winpcap
#include "winsock2.h"	//need winsock for inet_ntoa and ntohs methods
#include <iostream>
#include <bitset>
#include <array>

using namespace std;

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
const int MESSAGE_LENGTH = 3;
array<bool, MESSAGE_LENGTH * 8> message = { false };

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char **argv)
{
	// hard-coded, bad-way to build up our message
	// DO NOT DO IT THIS WAY
	string raw_message = "Rob";
	cout << "Message to send: " << raw_message << endl;
	cout << "Message Bits:    ";


	for (int i = 0; i < MESSAGE_LENGTH; i++)
	{
		// create a bitset, generated from the bits in the byte
		// of the ith character of raw_message
		bitset<8> x(raw_message[i]);

		// append to single/flat array of bits 
		// representing entire message
		for (int j = 0; j < 8; j++)
		{
			message[j + (8 * i)] = x[j];
		}

		cout << x << " ";
	}
	
	cout << endl << endl;

	pcap_t *fp;
	pcap_dumper_t *dumpfile;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (argc != 3)
	{
		printf("usage: %s infilename outfilename", argv[0]);
		return -1;
	}

	/* Open the capture file */
	if ((fp = pcap_open_offline(argv[1],	// name of the device
		errbuf								// error buffer
		)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the input file %s.\n", argv[1]);
		return -1;
	}

	/* Open the dump file */
	dumpfile = pcap_dump_open(fp, argv[2]);

	if (dumpfile == NULL)
	{
		fprintf(stderr, "\nError opening output file\n");
		return -1;
	}

	// read and dispatch packets until EOF is reached
	pcap_loop(fp, 0, packet_handler, (unsigned char *)dumpfile);

	pcap_close(fp);

	return 0;
}


/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	eth_header *eh;
	ip_header *ih;
	u_int s_b4;
	
	//Ethernet header
	eh = (eth_header *)pkt_data;

	//Ip packets
	if (ntohs(eh->type) == 0x0800)
	{
		/* retireve the position of the ip header */
		ih = (ip_header *)(pkt_data + 14); //length of ethernet header
		s_b4 = (unsigned int)ih->saddr.byte4;

		// if the traffic is originating from our "sender"...
		// NOTE: this is the wront way to do this! In our example, traffic is 
		// b/t 10.10.10.23 and 10.10.22. This is a lame way to focus on just what
		// is coming from 10.10.10.22
		if (s_b4 == 22)
		{
			// until we reach the end of our message...
			if (total < message.size())
			{
				// we are going to cheat here. Our client was setting outbound packets
				// with a TTL of 128 on all of them. We will simply set the TTL to either
				// 255 or 127 based on our MSB value (not using bit-math here)
				ih->ttl = (message[total]) ? 255 : 127;
			}

			++total;
		}
	}

	/* save the packet on the dump file */
	pcap_dump(dumpfile, header, pkt_data);
}
