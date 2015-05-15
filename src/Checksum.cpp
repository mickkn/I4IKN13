#include <cstring>
#include "Checksum.h"
#include "TransConst.h"

namespace Transport
{
	long Checksum::checksum(char buf[], short size)
	{
		int i = 0, length = size;
		long sum = 0;
		while (length > 0) 
		{
	   		sum	+= (buf[i++]&0xff) << 8;
   			if ((--length)==0) break;
   			sum += (buf[i++]&0xff);
	   		--length;
		}

		return (~((sum & 0xFFFF)+(sum >> 16)))&0xFFFF;
	}

	bool Checksum::checkChecksum(char buf[], short size)
	{
		char buffer[size-CHKSUMSIZE];

		memcpy(buffer, buf+CHKSUMSIZE, size-CHKSUMSIZE); 
		return( checksum(buffer, size-CHKSUMSIZE) == (long)((unsigned char)buf[CHKSUMHIGH] << 8 | (unsigned char)buf[CHKSUMLOW]));
	}

	void Checksum::calcChecksum (char buf[], short size)
	{
		char buffer[size-CHKSUMSIZE];
		long sum = 0;

		memcpy(buffer, buf+CHKSUMSIZE, size-CHKSUMSIZE); 
		sum = checksum(buffer, size-CHKSUMSIZE);
		buf[CHKSUMHIGH] = ((sum >> 8) & 255);
		buf[CHKSUMLOW] = (sum & 255);
	}
}
