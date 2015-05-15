#ifndef __FILE_SERVER_H__
#define __FILE_SERVER_H__

#include <iostream>

class file_server
{
public:
	file_server ();

private:
	char *buffer;
	
	void sendFile(std::string fileName, long fileSize, Transport::Transport *transport);
};

#endif /* __SLIP_SERVER_H__ */