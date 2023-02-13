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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is streamwrite hostname portnumber
 */

/**
 * Function accepts parameters at the command line
 * such as the server ip to conenct to and the port
 * that the client will listen to
 *
 * @return Exits with status 1 after paramters are given
 **********************************************************************/
static void usage() {
	cerr << "Usage: server [-s server-ip]" << endl;
	cerr << "    -s the server port to which the client must listen" << endl;
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
 * Implements a transactional TCP client program by reading binary
 * input and sending to the server by copying to the socket. It then
 * shutdowns the library call to let the server know the request
 * phase has ended. It reads any respose data and copy's it over
 *
 * @param argc Base function argument to be used on command line
 * @param argv Base function argument to be used on command line
 ********************************************************************/
int main(int argc, char **argv) {
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buf[2048];

	int opt;
	int bytes_read, rval = 0;
	string ip_addr = "127.0.0.1"; //sets ip address ahead of time

	while ((opt = getopt(argc, argv, "s:")) != -1) {
		switch(opt){
			case 's': ip_addr = optarg; 
			break;
			default: usage(); 	//allow for usage on the command line
			break;
		}
	}

     /* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {	//if socket is less than 0
		perror("opening stream socket");		//show opening stream socket fails and exit
		exit(1);
	}

     /* Connect socket using name specified by command line. */
	server.sin_family = AF_INET;
	hp = gethostbyname(ip_addr.c_str());
	if (hp == 0) {
	  	fprintf(stderr, "%s: unknown host", argv[1]);	//show host cannot be resolved
		exit(2);
	}

	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(argv[optind]));

	if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) { //if connection to server fails
		perror("connecting stream socket");	
		exit(1);	//show error and exit
	}

	while ((bytes_read = read(fileno(stdin), buf, sizeof(buf))) > 0) {	//while reading bytes into stdin and size of the buffer is greater than 0
		if (safe_write(sock, buf, bytes_read) < 0){	//if safe_write() is less than 0
			perror("writing on stream socket");
			close(sock);	//end connection and show error
		}
	}

	shutdown(sock, SHUT_WR);

	do {
		if (safe_write(fileno(stdout), buf, rval) < 0) { 	//if writing into stdout fails
			perror("writing on stream socket");
			close(sock);		//show error and close
		}
		if ((rval = read(sock, buf, sizeof(buf))) < 0) {  //if reading into buffer fails
			perror("reading stream socket");				
			close(sock);	//show error and close
		}
	} while (rval != 0); //do while rval is not equal to 0

	close(sock);	//end connection
	return 0;
}