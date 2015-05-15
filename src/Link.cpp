#include <Link.h>
#include <cstdio>

#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace Link {

/**
  * Explicitet constructor for linklaget.
  * Åbner den serielle port og sætter baudraten, paritet og antal stopbits
  */
Link::Link(int bufsize)
{
	buffer = new unsigned char[(bufsize*2)+2];
	
    serialPort=v24OpenPort("/dev/ttyS0",V24_STANDARD);
    if ( serialPort==NULL )
    {
        fputs("error: sorry, open failed!\n",stderr);
        exit(1);
    }

    int rc=v24SetParameters(serialPort,V24_B115200,V24_8BIT,V24_NONE);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }
}

/**
  * Destructor for linklaget.
  * Lukker den serielle port
  */
Link::~Link()
{
	if(serialPort != NULL)
		v24ClosePort(serialPort);
	if(buffer != NULL)
		delete [] buffer;
}

/**
 * Bytestuffer buffer, dog maximalt det antal som angives af size
 * Herefter sendes de til modtageren via den serielle port
 *
 * @param buffer De bytes som skal sendes
 * @param size Antal bytes der skal sendes
 */
void Link::send(char buf[], short size)
{	
//	printf("[Link.send] Sending... size: %d\n",size);

//	std::cout 	<< "[Link.send] buf[]: "; 
//	for(int i=0 ; i < size ; i++) {
//		printf("%c(%X)",buf[i],buf[i]);
//	}
//	printf("\n\n");

	/* test udskrift HEADER */

//	printf("[Link.send] HEADER: ");
//	for(int i = 0 ; i < 4 ; i++) {
//		printf("(0x%X)",buf[i]);
//	}
//	printf("\n");

	int cnt = 0;
	unsigned char begin[1] = {'A'};
	
	/* send første A */
	
   	int n = v24Write(serialPort,begin,1);
	if(n != 1) {
		std::cout 	<< "ERROR(" 
					<< v24QueryErrno 
					<< "): Failed to write to serialPort, bytes written: "
					<< n 
					<< std::endl;
	}
	
	for(int i = 0; i < size; i++) {
		if (buf[i] == 'A') {
				buffer[cnt] = 'B';
				cnt++;
				buffer[cnt] = 'C';
				cnt++;
		}
		else if (buf[i] == 'B') {
				buffer[cnt] = 'B';
				cnt++;
				buffer[cnt] = 'D';
				cnt++;
		}
		else {
			buffer[cnt] = buf[i];
			cnt++;
		}
	}
	        
	/* send data */
	                                                                                                                                                                                                               
   	n = v24Write(serialPort,buffer,cnt);
	if(n != cnt) {
		std::cout 	<< "ERROR(" 
					<< v24QueryErrno 
					<< "): Failed to write to serialPort, bytes written: "
					<< n 
					<< std::endl;
	}
	
	/* send sidste A */
	
   	n = v24Write(serialPort,begin,1);
	if(n != 1) {
		std::cout 	<< "ERROR(" 
					<< v24QueryErrno 
					<< "): Failed to write to serialPort, bytes written: "
					<< n 
					<< std::endl;
	}
}

/**
 * Modtager data fra den serielle port og debytestuffer data og gemmer disse i buffer.
 * Modtager højst det antal bytes som angives af size.
 * Husk kun at læse en byte ad gangen fra den serielle port.
 *
 * @param buffer De bytes som modtages efter debytestuffing
 * @param size Antal bytes der maximalt kan modtages
 * @return Antal bytes modtaget
 */
short Link::receive(char buf[], short size) {
	
//	printf("[Link.receive] size: %d\n", size);
	
	int err, counter=0, realSize=0;
	unsigned char begin = '0';
	unsigned char tempBuf[size];
	
	/* find det første 'A' */
	while(v24Getc(serialPort) != 'A');
	
	/* læs 1 byte ad gangen */
	while(1) {
		tempBuf[counter] = v24Getc(serialPort);	
		if(tempBuf[counter] == 'A')
			break;
		else
			counter++;
	}
	
	/* test udskrift (cout er en bitch) */
	
//	printf("[Link.receive] Udpakket data: ");
//	for(int i=0 ; i < counter ; i++) {
//		printf("%c(%X)",tempBuf[i],tempBuf[i]);
//	}
//	std::cout << std::endl << "counter: " << counter << std::endl;
	
	/* tæl data størrelse uden B'er */
	for(int i=0 ; i < counter ; i++) {
	
		if(tempBuf[i] != 'B')
			realSize++;
	}
	
	/* opbyg reel data array */
	counter = 0;
	for(int i=0 ; i < realSize ; i++) {
		
		if(tempBuf[counter] == 'B' && tempBuf[counter+1] == 'C') {
			buf[i] = 'A';
			counter+=2;
		}
		else if(tempBuf[counter] == 'B' && tempBuf[counter+1] == 'D') {
			buf[i] = 'B';
			counter+=2;
		}
		else {
			buf[i] = tempBuf[counter];
			counter++;
		}
	}

	/* test udskrift buffer */
	
//	std::cout << "[Link.receive] realSize: " << realSize << std::endl;
//	printf("[Link.receive] Udpakket data (uden A'er): ");
//	for(int i=0 ; i < realSize ; i++) {
//		printf("%c",buf[i]);
//	}
//	printf("\n");
	
	/* test udskrift HEADER */
	
//	printf("[Link.receive] HEADER: ");
//	for(int i=0 ; i < 4 ; i++) {
//		printf("(0x%X)",buf[i]);
//	}
//	printf("\n");
	
	return realSize;
}
}/* namespace Link */
