#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>
#include <cstddef>

/*
 01 2 3 7 8     15 16     23 24     31
+--+-+---+--------+---------+--------+
|Ty|T|Win|   Seq  |       Length     |
|pe|R|dow|   num  |                  |
+--+-+---+--------+---------+--------+
|       Timestamp (4 bytes)          |
|                                    |
+------------------------------------+
|       CRC1 (4 bytes)               |
|                                    |
+------------------------------------+
|                                    |
|                                    |
+                                    +
.       Payload (max 512 bytes)      .
.                                    .
.                                    .
+------------------------------------+
|       CRC2 (4 bytes, optional)     |
|                                    |
+------------------------------------+
*/

class Packet {
private:
	int buffer_size;  // Size of buffer
	uint8_t *data;  // Get this from the buffer

	// Starting index of the follwing:
  	const int TYPE = 0;
	const int TR = 0;
	const int WINDOW = 0;
	const int SEQNUM = 1;
	const int LENGTH = 2;
	const int TIMESTAMP = 4;
	const int CRC1 = 8;
	const int PAYLOAD = 12;
	const int CRC2 = 524;

public:
	// Initializes data to nullptr and buff_size to 0
  Packet();

	// Default destructor
	~Packet();

	// Sets the private data member to the buffer
	// and stores the updates buffer_size to b_size
	void setBuffer(void *buffer, int b_size);

	// Returns the value of data
	const uint8_t getData();

	// Can set the type to an int between 0 and 3, otherwise it is set to 0
	void setType(unsigned int);

	// Returns the value in the type index
	unsigned int getType() const;

	void setTR(unsigned int);

	unsigned int getTR() const;

	void setWindow(unsigned int);

	unsigned int getWindow() const;

	void setSeqNum(unsigned int);

	unsigned int getSeqNum() const;

	void setLength(unsigned int);

	unsigned int getLength() const;

  	void setTimestamp(unsigned int);

	unsigned int getTimestamp() const;

	void setCRC1(unsigned int);

	unsigned int getCRC1() const;

	void setPayload(std::string);

	const uint8_t *getPayload() const;

	void displayPayload();

	void setCRC2(unsigned int);

	unsigned int getCRC2() const;
};

#endif


