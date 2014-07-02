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
array<bool, 24> message = { false };

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char **argv)
{
	// hard-coded, bad-way to build up our message
	// DO NOT DO IT THIS WAY
	bitset<8> r('R');
	bitset<8> o('o');
	bitset<8> b('b');

	// set the bits
	for (int i = 0; i < 8; i++)
	{ 
		message[i] = r[i];		// R
		message[i + 8] = o[i];	// o
		message[i + 16] = b[i];	// b
	}

	//cout << "Rob: " << message.size() << endl;

	//// pretty-print bool array
	//for (int i = 0; i < message.size(); i++)
	//{
	//	if (message[i])
	//	{
	//		cout << 1;
	//	}
	//	else
	//	{
	//		cout << 0;
	//	}
	//}

	//cout << endl;


	//return 0;


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

	cout << "Total of " << total << " packets." << endl;
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
		if (s_b4 == 22)
		{
			if (total < message.size())
			{
				cout << "I should set the MSB as follows: " << message[total] << endl;
				printf("TTL : %d\n", (unsigned int)ih->ttl);

				// we are going to cheat here. Our client was setting outbound packets
				// with a TTL of 128 on all of them. We will simply set the TTL to either
				// 255 or 127 based on our MSB value (not using bit-math here)
				ih->ttl = (message[total]) ? 255 : 127;
				printf("TTL : %d\n", (unsigned int)ih->ttl);
			}

			++total;
		}
	}

	/* save the packet on the dump file */
	pcap_dump(dumpfile, header, pkt_data);
}
