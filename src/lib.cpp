
/*
 * lib.c
 *
 *  Created on: Sep 8, 2012
 *      Author: Lars Mortensen
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <lib.h>

/**
 * Udskriver fejlmeddelelse og stoppe programmet
 *
 * @param msg Fejlmeddelelse
 */
void error(const std::string msg)
{
    perror(msg.c_str());
    exit(0);
}

/**
 * Udtrækker et filnavn fra en tekststreng
 *
 * @param fileName Filnavn incl. evt. stinavn
 * @return Kun filnavn
 */
const std::string extractFileName(const std::string fileName)
{
	return fileName.substr(fileName.find_last_of("/")+1);
}

/**
 * Se om filen fileName eksisterer
 *
 * @param fileName Filnavn
 * @return Størrelsen på filen, 0 hvis den ikke findes
 */
const long check_File_Exists(std::string fileName)
{
	struct stat sts;
	if ((stat (fileName.c_str(), &sts)) == -1)
		return 0;

	return sts.st_size;
}