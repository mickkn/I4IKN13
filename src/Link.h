
#ifndef __LINKLAGET_H__
#define __LINKLAGET_H__

#include <ezV24/ezV24.h>

#define DELIMITER 'A'

namespace Link {

class Link
{
public:
	Link(int bufsize );
	~Link();
	void send(char [], short size);
	short receive(char buf[], short size);
	char rcvChar(void);
private:
	Link( const Link &r){};
	Link& operator=(const Link &) {};
	unsigned char *buffer;
	v24_port_t *serialPort;
};


} /* namespace Linklaget */
#endif /* LINKLAGET_H_ */

