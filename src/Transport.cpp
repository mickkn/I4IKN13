#include <iostream>
#include <cstring>
#include <cstdio>
#include <Transport.h>
#include <TransConst.h>

#include <unistd.h>

#define DEFAULT_SEQNO 2

namespace Transport
{
	/// <summary>
	/// Initializes a new instance of the <see cref="Transport"/> class.
	/// </summary>
	Transport::Transport(short BUFSIZE)
	{
		link = new Link::Link(BUFSIZE+ACKSIZE);
		checksum = new Checksum();
		buffer = new char[BUFSIZE+ACKSIZE];
		seqNo = 0;
		old_seqNo = DEFAULT_SEQNO;
		errorCount = 0;
	}

	/// <summary>
	/// Delete dynamics attribute before delete this object.
	/// </summary>
	Transport::~Transport()
	{
		if(link) delete link;
		if(checksum) delete checksum;
		if(buffer) delete [] buffer;
	}
	
	/// <summary>
	/// Receives the ack.
	/// </summary>
	/// <returns>
	/// The ack.
	/// </returns>
	bool Transport::receiveAck()
	{
		char buf[ACKSIZE];
		short size = link->receive(buf, ACKSIZE);
		if (size != ACKSIZE) return false;
		if(!checksum->checkChecksum(buf, ACKSIZE) ||
				buf[SEQNO] != seqNo ||
				buf[TYPE] != ACK)
			return false;
			
		seqNo = (buffer[SEQNO] + 1) % 2;
			
		return true;
	}

	/// <summary>
	/// Sends the ack.
	/// </summary>
	/// <param name='ackType'>
	/// Ack type.
	/// </param>
	void Transport::sendAck (bool ackType)
	{
		char ackBuf[ACKSIZE];
		ackBuf [SEQNO] = (ackType ? buffer[SEQNO] : (buffer[SEQNO] + 1) % 2);
		ackBuf [TYPE] = ACK;
		checksum->calcChecksum (ackBuf, ACKSIZE);

		link->send(ackBuf, ACKSIZE);
	}

	/// <summary>
	/// Send the specified buffer and size.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	/// <param name='size'>
	/// Size.
	/// </param>
	void Transport::send(const char buf[], short size)
	{
		
		//printf("[Transport.send] Sending...\n");
		//std::cout << "[Transport.send] buf[]: " << buf << std::endl;
		//std::cout << "[Transport.send] size: " << size << std::endl;
		
		/* opbyg frame med header og data <CS_HI><CS_LO><SEQ><TYPE><DATA>*/
		
		do {
			for(int i = 0 ; i < size ; i++) {	// Add data from buffer[4]
				buffer[i+4] = buf[i];
			}	
			
			buffer[SEQNO] = seqNo; 	// Tilføj sekvensnummer
			buffer[TYPE] = DATA;	// Tilføj DATA byte <0>
			
			checksum->calcChecksum(buffer,size+4); // Calculate checksum

			link->send(buffer,size+4);
		}while(!receiveAck());
		//printf("[Transport.send] Ack received\n");
	}

	/// <summary>
	/// Receive the specified buffer.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	short Transport::receive(char buf[], short size)
	{
		while(1) {
			//printf("[Transport.receive] Receiving...\n");
			short n = 0;
			
			n = link->receive(buffer,size);

			while(!checksum->checkChecksum(buffer,n)) {
				sendAck(false);
				n = link->receive(buffer,size);
				printf("[Transport.receive] Checksum error\n");
				errorCount++;
			}
			
			sendAck(true);
			printf("Error(s): %d\n", errorCount);
			
			if(seqNo == buffer[SEQNO]) {
				seqNo = (buffer[SEQNO] + 1) % 2;
				for(int i=0 ; i < n-4; i++) {
					buf[i] = buffer[i+4];
				}
				return n-4; // data byte size minus header
			}
		}
	}
}


