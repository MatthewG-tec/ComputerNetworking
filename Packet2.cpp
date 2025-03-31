#include "Packet2.h"
#include <iostream>

// max size for payload
#define DATA_SZ 1024
// size of header
#define HEADER_SZ 5

Packet::Packet() {
  data = nullptr;
	buffer_size = 0;
}

Packet::~Packet() {}

void Packet::setBuffer(void *buf, int b_size) {
  data = static_cast<uint8_t*> (buf);
	buffer_size = b_size;
}

const uint8_t Packet::getData() {return *data;}

void Packet::setType(unsigned int n) {
	if (n > 3 || n < 0)  // The value must be between 0 and 2 bits
		n = 0;
  if (data != nullptr && buffer_size > HEADER_SZ) {
		data[TYPE] &= 0x3f;  // Clear buffer (0x3f = 111111)
		data[TYPE] |= (n << 6);  // sets the lower 2 bits of the header field
	}
}

unsigned int Packet::getType() const {
  if (data != nullptr && buffer_size > HEADER_SZ)
		return (data[TYPE] >> 6);
	else
	  return 0;
}

void Packet::setTR(unsigned int n) {
  if (data != nullptr && buffer_size > HEADER_SZ) {
		data[0] &= 0xDF;
		data[0] |= (n << 5);
	}
}

unsigned int Packet::getTR() const {
  if (data != nullptr && buffer_size > HEADER_SZ)
		return (data[0] >> 5) & 1;
	else
		return 0;
}

void Packet::setWindow(unsigned int n) {
  if (data != nullptr && buffer_size > HEADER_SZ) {
		data[0] &= 0xE0;
	  data[0] |= n;
	}
}

unsigned int Packet::getWindow() const {
  if (data != nullptr && buffer_size > HEADER_SZ) {
		return (data[0] & 0x1F);
	} else {
			return 0;
	}
}

void Packet::setSeqNum(unsigned int n) {
	if (n > 255)  // Must be a 8 bit value
		n = 0;
	if (data != nullptr && buffer_size > HEADER_SZ)
		data[SEQNUM] = n;
}

unsigned int Packet::getSeqNum() const {
  if (data != nullptr && buffer_size > HEADER_SZ)
  	return data[SEQNUM];
	else
		return 0;
}

void Packet::setLength(unsigned int n) {
	if(n > 65535 || n < 0)  // Must be a 16 bit value
		n = 0;
  if (data != nullptr && buffer_size > HEADER_SZ) {
		data[LENGTH] = (n >> 8);  // Shift right to get by 8 bits to get the MSB
		data[LENGTH + 1] = (n & 255);  // sets the n-value bits to 0
	}
}

unsigned int Packet::getLength() const {
	if (data != nullptr && buffer_size > HEADER_SZ)
		return data[LENGTH + 1] | (data[LENGTH] << 8);  // undos the changes
																										// in setlength
	else
		return 0;
}


void Packet::setTimestamp(unsigned int n) {
  if (n > (0xffffffff) || n < 0)  // Must be a 4 byte value
		n = 0;
  if (data != nullptr && buffer_size > HEADER_SZ) {
	  data[TIMESTAMP] = (n >> 24);
		data[TIMESTAMP + 1] = (n & 0x00ff0000) >> 16;
		data[TIMESTAMP + 2] = (n & 0x0000ff00) >> 8;
		data[TIMESTAMP + 3] = (n & 0x000000ff);
	}
}

unsigned int Packet::getTimestamp() const {
	if (data != nullptr && buffer_size > HEADER_SZ)
		return ((data[TIMESTAMP] << 24) | (data[TIMESTAMP + 1] << 16) | (data[TIMESTAMP + 2] << 8) | data[TIMESTAMP + 3]);
	else
		return 0;
}

void Packet::setCRC1(unsigned int n) {
  if (n > (0xffffffff) || n < 0)  // Must be a 4 byte value
		n = 0;
  if (data != nullptr && buffer_size > HEADER_SZ) {
	  data[CRC1] = (n >> 24);
		data[CRC1 + 1] = (n & 0x00ff0000) >> 16;
		data[CRC1 + 2] = (n & 0x0000ff00) >> 8;
		data[CRC1 + 3] = (n & 0x000000ff);
	}
}

unsigned int Packet::getCRC1() const {
	if (data != nullptr && buffer_size > HEADER_SZ)
		return ((data[CRC1] << 24) | (data[CRC1 + 1] << 16) | (data[CRC1 + 2] << 8) | data[CRC1 + 3]);
	else
		return 0;
}

void Packet::setPayload(std::string s) {
	unsigned int value = 0;
	for(int i = 0; i <= s.length() - 2; i++) {
		value = s[i];
		data[i + PAYLOAD] = value;
	}
}

const uint8_t* Packet::getPayload() const {
	return data + PAYLOAD;
}

void Packet::displayPayload() {
	for (int i = PAYLOAD; i <= CRC2 - 1; i++)
		std::cout << static_cast<int>(data[i]) << " ";
}

void Packet::setCRC2(unsigned int n) {
  if (n > (0xffffffff) || n < 0)  // Must be a 4 byte value
		n = 0;
  if (data != nullptr && buffer_size > HEADER_SZ) {
	  data[CRC2] = (n >> 24);
		data[CRC2 + 1] = (n & 0x00ff0000) >> 16;
		data[CRC2 + 2] = (n & 0x0000ff00) >> 8;
		data[CRC2 + 3] = (n & 0x000000ff);
	}
}

unsigned int Packet::getCRC2() const {
	if (data != nullptr && buffer_size > HEADER_SZ)
		return ((data[CRC2] << 24) | (data[CRC2 + 1] << 16) | (data[CRC2 + 2] << 8) | data[CRC2 + 3]);
	else
		return 0;
}
