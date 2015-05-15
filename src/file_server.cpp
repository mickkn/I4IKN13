#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <Transport.h>
#include <lib.h>
#include <file_server.h>

using namespace std;

bool debug = false;

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 1000

/// <summary>
/// Initializes a new instance of the <see cref="file_server"/> class.
/// </summary>
file_server::file_server ()
{
	Transport::Transport transport(BUFSIZE);
	char buffer[BUFSIZE];
	
	// Read filename
	bzero(buffer,BUFSIZE);
	cout << "[App.server] waiting for filename" << endl;
	int n = transport.receive(buffer,BUFSIZE);
	
	if(debug)cout << "[App.server] n from fileName: " << n << endl;
	
	string fileName = string(buffer);
	fileName = extractFileName(fileName);
	
	// Check file exist
	long fileSize = check_File_Exists(fileName);
	if(fileSize > 0) {
		cout << "[App.server] File exist" << endl;
		cout << "[App.server] Filesize: " << fileSize << " bytes" << endl;
	}

	// Send filesize
	bzero(buffer,BUFSIZE);
	sprintf(buffer, "%ld", fileSize);
	if(debug)cout << "[App.server] Send fileSize[buffer]: " << buffer << endl;
	if(debug)cout << "[App.server] Send fileSize[strlen(buffer)]: " << strlen(buffer) << endl;	
	transport.send(buffer,strlen(buffer));
	
	// Send file
	if(debug)cout << "[App.server] Send file: " << fileName << endl;
	sendFile(fileName,fileSize,&transport);
	
	cout << "[App.server] Connection terminated." << endl; 
}

/// <summary>
/// Sends the file.
/// </summary>
/// <param name='fileName'>
/// File name.
/// </param>
/// <param name='fileSize'>
/// File size.
/// </param>
/// <param name='transport'>
/// Transport lag.
/// </param>
void file_server::sendFile(std::string fileName, long fileSize, Transport::Transport *transport)
{
	const char* fileSendName = fileName.c_str(); // Parse to const char *ptr
	char buffer[BUFSIZE]; 						 // Send buffer
	FILE *fp = fopen(fileSendName, "rb"); 		 // Make filepointer rb = read binary
	if(fp == NULL)
		error("[App.server] ERROR: No such file on Server\n");

	cout << "[App.server] Sending <" << fileName << "> to the Client..." << endl;
	bzero(buffer, BUFSIZE); // Clear buffer
	
	int n; // Counter
	while((n = fread(buffer, 1, BUFSIZE, fp))>0)
	{
		transport->send(buffer, n);	// Send file in 1000 byte blocks
			
		cout << "[App.server] transfer size: " << n << " Bytes" << endl;
		bzero(buffer, BUFSIZE); // Clear buffer
	
	}
	
	fclose(fp);
	cout << "[App.server] Ok sent to client!" << endl;
}

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// The command-line arguments.
/// </param>
int main(int argc, char **argv)
{
	new file_server();
	
	return 0;
}
