#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

namespace Transport
{
	class Checksum
	{
	public:
		Checksum ()
		{
		}
		bool checkChecksum(char buf[], short size);
		void calcChecksum (char buf[], short size);
	private:
		long checksum (char buf[], short size);
	};
}

#endif /* __CHECKSUM_H__ */