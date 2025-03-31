// What needs to be done:
// CRC1 is supposed to be the crc of the header (including timestamp)
// Payload is the data in the file you want to send
// crc2 is the crc calculation of the payload
// If all that works, figure out the acknowledgment


#include "Packet2.h"
#include "Packet2.cpp"

#include <iostream>
#include <zlib.h>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <iomanip>

#define BUFFER_SIZE 528

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
int sock, rval;
struct addrinfo hints, *results, *ptr;

// quick check if we provide the right arguments
if (argc != 5) {
    std::cout << "Usage " << argv [0] << std::endl;
    std::cout << "File: " << "File Name" << std::endl;
    std::cout << "Destination_host: " << "::1 or 127.0.0.1" << std::endl;
    std::cout << "Destination_port: " << "2012" << std::endl;
    std::cout << "IPv4 or IPv6: " << "4 or 6" << std::endl;
return 1;
}

const char *file = argv[1];
const char *hostname = argv[2];
const char *portnum = argv[3];
const char *ipVersion = argv[4];

// if we obtain the data from getaddrinfo , we might as well use it to open the socket.
// first , we prepare hints by clearing all of its fields.
memset (& hints, 0, sizeof(hints));
// hints.ai_family = AF_UNSPEC; // for both IPv4 and IPv6.
if (std::string(ipVersion) == "4") {
    hints.ai_family = AF_INET; // use IPv4
} 
else if (std::string(ipVersion) == "6") {
    hints.ai_family = AF_INET6; // use IPv6
}
else {
    std::cerr << "Invalid IP version use '4' for IPv4 or '6' for IPv6" << std::endl;
return 2;
}

hints.ai_socktype = SOCK_DGRAM;
hints.ai_flags = AI_PASSIVE; // listen in on ip for 

getaddrinfo(NULL, "2012", &hints, &results);

if ((rval = getaddrinfo (argv[2], portnum, &hints, &results)) != 0) { // error
std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
return 3;
}

// loop through the results from the linked list
for (ptr = results; ptr != NULL; ptr = ptr->ai_next) {
if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) != -1) {
break;  // socket successful
    }
}

// if (reach end of list)
if (ptr == NULL) {
std::cerr << "Failed to open socket" << std::endl;
return 3;
}

// Identify the sender
// std::string message = "";

// std::cout << "The SENDER terminal window: " << std::endl;
// std::cout << "sender receiver_address: " << argv[4] << std::endl;
//std::cout << "Enter a message: ";
//std::cin >> message;

/*
const char *msg = message.c_str();
int len, bytes_sent;

len = strlen(msg);
*/

// std::cout << "File: " << argv[1] << std::endl;


  std::ifstream ifi;
  // Gets the total bytes of the file
  // open file in binary mode
  ifi.open(argv[1], std::ios::binary);  // argv[2] is the file name
  if (ifi.fail()) {
    std::cout << "Cannot open file" << std::endl;
    return 1;
  }

  // read from file until the end is reached
  char buf[BUFFER_SIZE];
  int totalBytesRead = 0;

  while (ifi.read(buf, BUFFER_SIZE)) {
    totalBytesRead += ifi.gcount();
  }

  // Handle the remaining bytes if any
  if (ifi.gcount() > 0) {
    totalBytesRead += ifi.gcount();
  }

  if (ifi.bad()) {
    std::cout << "Error reading from file" << std::endl;
    ifi.close();
    return 2;
  }

  ifi.close();


	// Wanting to get the content of the file as a string
	  std::ifstream inFile;
    inFile.open(argv[1]); //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string content = strStream.str(); //str holds the content of the file

    std::cout << content << std::endl;



Packet *P;
P = new Packet;

uint8_t *buffer;
buffer = new uint8_t[BUFFER_SIZE];

P->setBuffer(buffer, BUFFER_SIZE);
P->setType(1);
P->setTR(0);
P->setWindow(0);
P->setSeqNum(0);
P->setLength(512);
P->setTimestamp(0);

std::cout << "Packet header: " << P->getType() << " " << P->getTR() << " "
	<< P->getWindow() << " " << P->getSeqNum() << " " << P->getLength() <<
	" " << P->getTimestamp() << std::endl;

std::string headerString = "";

headerString += std::to_string(P->getType());
headerString += std::to_string(P->getTR());
headerString += std::to_string(P->getWindow());
headerString += std::to_string(P->getSeqNum());
headerString += std::to_string(P->getLength());
headerString += std::to_string(P->getTimestamp());

std::cout << "Header string: " << headerString << std::endl;
std::cout << "Length: " << headerString.length() << std::endl;

// setting and calculating the crc1
  unsigned long crc1;
  crc1=crc32(0L, NULL, 0);
  crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), 8);

	std::cout << "CRC1 for header = " << std::hex << crc1 << std::endl;
	P->setCRC1(crc1);
	std::cout << "Packet CRC1 = " << std::hex << P->getCRC1() << std::endl;


	P->setPayload(content);
  //std::cout << "Payload: ";
 	//P->displayPayload();
	//std::cout << std::endl;


// setting and calculating the crc2
  uint8_t * startofPayload = &buffer[12];
  unsigned long crc;
  crc=crc32(0L, NULL, 0);
  crc = crc32(crc, reinterpret_cast<const Bytef*>(startofPayload), 512);

	std::cout << "CRC for payload = " << std::hex << crc << std::endl;
	P->setCRC2(crc);
	std::cout << "Packet CRC2 = " << std::hex << P->getCRC2() << std::endl;

std::cout << "Length in Packet = " << (P->getLength()) << std::endl;

// std::cout << "Buffer: " << std::endl;
// for(int i = 0; i < 528; i++) {
// 	std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]) << " ";
// }
//std::cout << std::endl;

int packetLength = sizeof(buffer);

std::cout << "Size of packet: " << packetLength << std::endl;

sendto(sock, buffer, BUFFER_SIZE, 0, results->ai_addr, results->ai_addrlen);

// int byte_count;
// socklen_t fromlen;
// struct sockaddr_storage fromaddr;
//char buf [1024];

//LINES BELOW DO NOT WORK!!!

// initialize fromlen. It is an input/output parameter.
// fromlen = sizeof(fromaddr);
// byte_count = recvfrom(sock, buf, sizeof(buf)-1, 0, (sockaddr*)(&fromaddr), &fromlen);

// // we assume we receive a string, not a binary packet, make it null string.

// buf [byte_count]= '\0';
// std::cout << "Recieved: " << buf << std::endl;

close(sock);

delete P;

}

