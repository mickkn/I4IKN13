#ifndef __FILE_CLIENT_H__
#define __FILE_CLIENT_H__

#include <iostream>

class file_client
{
public:
    file_client(int argc, char **argv);

private:
	void receiveFile (std::string fileName, Transport::Transport *transport);
};

#endif /* __FILE_CLIENT_H__ */