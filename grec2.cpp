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
#include <string>

// For #include "." files
#include "Packet2.h"
#include "Packet2.cpp"

// buffer SIZE
#define BUFFER_SIZE 528
// max packet SIZE

// provide hostname and port value as command line arguments.
// argv [0] is the executable name.
int main ( int argc, char *argv []) {
int sock, rval, listen;
struct addrinfo hints, *results, *ptr;
// quick check if we provide the right arguments.


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

getaddrinfo(NULL, portnum, &hints, &results);

if ((rval = getaddrinfo (argv[2], portnum, &hints, &results)) != 0) { // error
std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
return 3;
}

// loop through the results from the linked list.
for (ptr = results; ptr != NULL; ptr = ptr->ai_next) {
if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol )) != -1) {
break ; // socket successful
    }
}

 // if (reach end of list)
if (ptr == NULL) {
std::cerr << " Failed to open socket " << std :: endl ;
return 4;
}

// binding the socket.
if (bind(sock, ptr->ai_addr, ptr->ai_addrlen) == -1) {
std::cerr << "Cannot bind the socket" << std::endl;
return 5;
}

// reads in file
std::ifstream ifi;

char buf[BUFFER_SIZE];
int bytesRead = recvfrom(sock, buf, BUFFER_SIZE, 0, results->ai_addr, &results->ai_addrlen);

buf[bytesRead] = '\0';
std::ifstream inFile(file); 

while (inFile.read(buf, BUFFER_SIZE)) {
    bytesRead += inFile.gcount(); 
}
if (inFile.gcount() > 0) {
    bytesRead += inFile.gcount(); 
}
if (inFile.bad()) {
    std::cout << "Error reading from file" << std::endl; 
    ifi.close(); 
    return 2;
}
ifi.close();
// returning the paylod from recv buffer -> setting set payload
// wrong to content 
std::stringstream strStream;
strStream << inFile.rdbuf(); //read the file
std::string content = strStream.str(); //str holds the content of the file
std::cout << content << std::endl;

Packet *P; 
P = new Packet; 

P->setBuffer(buf, BUFFER_SIZE);

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

unsigned long crc1;
crc1=crc32(0L, NULL, 0);
crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buf), 8);

std::cout << "CRC1 for header = " << std::hex << crc1 << std::endl;
P->setCRC1(crc1);
std::cout << "Packet CRC1 = " << std::hex << P->getCRC1() << std::endl;

  uint8_t *startofPayload = reinterpret_cast<uint8_t*>(&buf[12]);
  unsigned long crc2;
  crc2=crc32(0L, NULL, 0);
  crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(startofPayload), 512);

	std::cout << "CRC for payload = " << std::hex << crc2 << std::endl;
	P->setCRC2(crc2);
	std::cout << "Packet CRC2 = " << std::hex << P->getCRC2() << std::endl;


    // Packet Prot
if (P->getType() == 1) {
    std::cout << "Packet contains data" << std::endl; 
    if (P->getTR() == 1) { 
        std::cout << "Confirm PTYPE NACK" << std::endl; 
    }
    int windowSize = P->getWindow();
    if (windowSize >= 0 && windowSize <= 31) {
        std::cout << "Window size is valid: " << windowSize << std::endl; 
    } 
    else { 
        std::cerr << "Invalid window size for PTYPE data" << std::endl; 
    }
    int sequenceSize = P->getSeqNum();
        if (sequenceSize >= 0 && sequenceSize <= windowSize) {
            std::cout << "Sequence size: " << sequenceSize << std::endl; 
        int nextSequenceSize = (sequenceSize +1) % 256;
        std::cout << "Next sequence number: " << nextSequenceSize << std::endl;         
        
    int length = P->getLength(); 
    if (length >= 0 && length <= 512) {
        std::cout << "Length value: " << length << std::endl; 
        int lastAckSeqNum;
        if (length == 0 && sequenceSize == lastAckSeqNum) {
            std::cout << "Data transfer is completed" << std::endl; 
        }
    }
    else { 
        std::cerr << "Invalid length feild value for PTYPE data" << std::endl; 
    }

    // fix the crc1 diffrentation from sender and recieiver.
    if (P->getCRC1() == crc1) {
        std::cout << "-- CRC1 PASSES --" << std::endl;
    }
    else { 
        std::cout << "-- CRC1 FAILS --" << std::endl; 
    }
   
    if (P->getTR() == 0) {
        const uint8_t *payloadData = P->getPayload();
        int payloadSize = P->getLength();
        std::cout << "Payload size: " << payloadSize <<std::endl; 
    }
    else {
        std::cout << "Payload Size is 0 (TR is 1)" << std::endl; 
    }
    // fix the crc2 getter and receiver from sender and receiver
    if (P->getCRC2() == crc2) {
        std::cout << "-- CRC2 PASSES --" << std::endl;
    }
    else { 
        std::cout << "-- CRC2 FAILS --" << std::endl; 
    }
}
        else {
            std::cerr << "Invalid sequence size for PTYPE data" << std::endl; 
        }
    }
if (P->getType() == 1) {
    std::cout << "Packet contains data" << std::endl; 

} else if (P-> getType() == 2) { 
    std::cout << "-- Confirm with PTYPE NACK --" << std::endl; 
    int nextExpecSequenceSize = (P->getSeqNum() + 1) % 256;
    std::cout << "Next expected sequence number: " << nextExpecSequenceSize << std::endl; 
}

 else if (P->getType() == 3) {
    // check TR field for PTYPE data
    if (P->getTR() == 1) { 
        std::cout << "Recived PTYPE NACK || data packet TR set to 1" << std::endl; 
    }
    else {
        std::cerr << "Invalid TR value for PTYPE NACK" << std::endl; 
    }
}
else { 
    std::cerr << "Invalid TYPE" << std::endl;
}

std::cout << "Length in Packet = " << (P->getLength()) << std::endl;


std::cout << std::endl;

close (sock);
}
