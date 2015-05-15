#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <Transport.h>
#include <lib.h>
#include <file_client.h>
#include <string.h>

using namespace std;

bool debug = false;

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 1000

/// <summary>
/// Initializes a new instance of the <see cref="file_client"/> class.
/// 
/// file_client metoden opretter en peer-to-peer forbindelse
/// Sender en forspørgsel for en bestemt fil om denne findes på serveren
/// Modtager filen hvis denne findes eller en besked om at den ikke findes (jvf. protokol beskrivelse)
/// Lukker alle streams og den modtagede fil
/// Udskriver en fejl-meddelelse hvis ikke antal argumenter er rigtige
/// </summary>
/// <param name='args'>
/// Filnavn med evtuelle sti.
/// </param>
file_client::file_client(int argc, char **argv)
{
	Transport::Transport transport(BUFSIZE);
    
    if(debug)cout << "[App.client] Transfering: " << argv[1] << endl;
    
    // test
    if(debug)cout << "[App.client] argv[1]: " << argv[1] << endl;
    if(debug)cout << "[App.client] strlen(argv[1]): " << strlen(argv[1]) << endl;
    
    /* Write fileName to Server */
    transport.send(argv[1],strlen(argv[1]));
    
	// Get the damn file
	receiveFile(argv[1], &transport);

	if(debug)cout << "[App.client] Connection terminated." << endl;
}

/// <summary>
/// Receives the file.
/// </summary>
/// <param name='fileName'>
/// File name.
/// </param>
/// <param name='transport'>
/// Transportlaget
/// </param>
void file_client::receiveFile (std::string fileName, Transport::Transport *transport)
{
	char buffer[BUFSIZE];	// The buffer :D
	bzero(buffer,BUFSIZE);
	long fileSize;

	// Get filesize
	int n = transport->receive(buffer,BUFSIZE);	// Get size of file to transfer
	fileSize = atol(buffer);
	
	if(debug) {
		printf("[App.receiveFile] buffer: ");
		for(int i=0 ; i<n ; i++) {
			printf("%c",buffer[i]);
		}
		printf("\n");
	}
	cout << "[App.client] FileSize: " << fileSize << endl;
	
	if(fileSize <= 0)
		error("[App.client] No such file on Server\n");
	else {
		cout << "[App.client] Filesize: " << fileSize << " bytes" << endl;
			
		fileName = extractFileName(fileName); // Extract filename from path
		cout << "[App.client] Receiving <" << fileName << "> from Server" << endl;
		const char* fileReceiveName = fileName.c_str(); // Parse fileName to const char *ptr
		
		FILE * fp = fopen(fileReceiveName,"wb"); // Write new file or overwrite file
		
		if(fp == NULL)
			error("[App.client] File cannot be opened.\n");
		else {
			bzero(buffer, BUFSIZE); // Clear buffer
		
			// Get the file
			int n;
			long n_size = 0;

			do{
				n = transport->receive(buffer,BUFSIZE); // Read block of data from stream
				fwrite(buffer,1,n,fp);	// Write blocks from buffer 1 byte n times to fp
				bzero(buffer,BUFSIZE);	// Clear buffer
				cout << "[App.client] transfer size: " << n << " Byte(s)" << endl;
				n_size += n;
				if(debug)cout << "[App.client] fileSize: " << fileSize << endl;
				if(debug)cout << "[App.client] transfersize(total): " << n_size << endl;
			}while(n_size != fileSize);
		
			cout << endl << "[App.client] Ok received from server!" << endl;
			fclose(fp);	// Close file
		}
	}
}		

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// First argument: Filname
/// </param>
int main(int argc, char** argv)
{
    if (argc < 2) {	// Syntax check
		error("[App.client] The server needs 1 argument <filename>\n");
    }

	new file_client(argc, argv);
	
	return 0;
}
