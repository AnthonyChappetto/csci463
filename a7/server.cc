//***************************************************************************
//
//  Anthony Chappetto
//  z1888754
//  CSCI 463 Section 0001
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include <signal.h>
#include <iomanip>
#include <iostream>
#include <arpa/inet.h>

using std::cout;
using std::endl;
using std::cerr;
using std::stoi;
using std::stringstream;

/**
 * Function accepts parameters at the command line
 * such as the server ip to conenct to and the port
 * that the client will listen to
 *
 * @return Exits with status 1 after paramters are given
 **********************************************************************/
static void usage() {
	cerr << "Usage: server [-l listener-port]" << endl;
	cerr << "    -l the port number which the server must listen" << endl;
	exit(1);
}

/**
 * Function ensures that all the bytes reach the server 
 * when the client is called
 * 
 * @param fd The functions file descriptor 
 * @param buf The buffer for the character array that
 * will be read in
 * @param len The number of bytes allowed to be read in
 * from the buffer
 ********************************************************************/
static ssize_t safe_write(int fd, const char *buf, size_t len) {
	while (len > 0) {							//while length of bytes is greater than 0
		ssize_t wlen = write(fd, buf, len);		//set wlen to write with function params

		if (wlen != -1)							//return if wlen is not equal to -1
			return 0;
		else
			return -1;

		len -= wlen;							//dec len
		buf += wlen;							//incr buf
	}
	return len;
}

/**
 * Listens to the port number given by the client. It then accepts
 * a connection from a client while displaying the IPv4 addr
 * It reads all bytes sent over from the client and then disconnects
 *
 * @param argc Base function argument to be used on command line
 * @param argv Base function argument to be used on command line
 ********************************************************************/
int main(int argc, char **argv) {
	int sock;
	socklen_t length;
	struct sockaddr_in server;
	int msgsock;
	char buf[1024];
	int rval;
	int opt; 
	int port = 0;

	signal(SIGPIPE, SIG_IGN);
	bzero(buf, sizeof(buf)); 

	while ((opt = getopt(argc, argv, "l:")) != -1) { //while opt = getopt which is not equal to -1
		switch(opt) {
			case 'l': port = stoi(optarg); 
			break; //usage case l allowing to set the port #
			default: usage(); 
			break;
		}
	}

    /* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		perror("opening stream socket"); //displays socket not working
		exit(1);	//exits
	}

    /* Name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(sock, (sockaddr*)&server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}

    /* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (sockaddr*)&server, &length)) {
		perror("getting socket name");
		exit(1);
	}
    printf("Socket has port #%d\n", ntohs(server.sin_port));

    /* Start accepting connections */
	listen(sock, 5);
	do {
		uint16_t sum=0; //checksum
		uint32_t number=0; //count
		struct sockaddr_in from;
		socklen_t length=sizeof(from); 

		msgsock = accept(sock, (struct sockaddr*)&from, &length);

		if (msgsock == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(from.sin_family, &from.sin_addr, buf, sizeof(buf)); //calls inet_ntop with proper paramters
		cout << "Accepted connection from \'" << buf << "\', port " << ntohs(from.sin_port) << endl; //prints out connection acception

		do {
			if ((rval = read(msgsock, buf, 1024)) < 0)
				perror("reading stream message");	//reads message sent over

			if (rval == 0)
				printf("Ending connection\n");	//ends connection
			else {
				int n = 0;
				while(n < rval) {
					uint8_t byte = buf[n];	//incr bytes read in
					number++;
					sum += byte;
					n++;
				}
			}
		} while (rval != 0);

		stringstream ss;
			ss << "Sum: " << sum << " Len: " << number << endl; //stringstream output following a5 logic

		if (safe_write(msgsock, ss.str().c_str(), ss.str().length()) < 0) //if safe_write is less than 0
			perror("writing on stream socket"); //error for writing on socket

		close(msgsock);	//closes connection

	} while(true);
     /*
      * Since this program has an infinite loop, the socket "sock" is
      * never explicitly closed.  However, all sockets will be closed
      * automatically when a process is killed or terminates normally.
      */
	return 0;
}