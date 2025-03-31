PA7:
By: Ethan Kostiuk, Anthony Nguyen, and: Matthew Gillett
-------------------------------------------------------------------------------------------------------------------------
Using Our Sender and Receiver: 
Sender Command: 
g++ -o gsend gsend2.cpp -lz

Running the Sender:
./gsend sendText.txt 127.0.0.1 2012 4 //IPv4
./gsend sendText.txt ::1 2012 6 // IPv6

Receiver Command: 
g++ -o grec grec2.cpp -lz

Running the Receiver:
./grec testrec.txt 127.0.0.1 2012 4 // IPv4
./grec testrec.txt ::1 2012 6 // IPv6

-------------------------------------------------------------------------------------------------------------------------
## Packet Structure

The data packet is structured as follows:

| Byte Range       | Description                      |
|------------------|----------------------------------|
| 01-02            | Type (Ty)                        |
| 03               | Reserved (T)                     |
| 04-06            | Window (Win)                     |
| 07-09            | Sequence Number (Seq num)        |
| 10-15            | Length                           |
| 16-23            | Timestamp (4 bytes)              |
| 24-27            | CRC1 (4 bytes)                   |
| 28-539           | Payload (max 512 bytes)          |
| 540-543          | CRC2 (4 bytes, optional)         |

### Description of Fields:

1. **Type (Ty):**
   - The type of the packet.
   
2. **Reserved (T):**
   - Reserved field for future use or alignment.

3. **Window (Win):**
   - Window size or identifier.

4. **Sequence Number (Seq num):**
   - A numerical sequence value to track the order of packets.

5. **Length:**
   - Length of the packet's payload.

6. **Timestamp:**
   - A 4-byte timestamp indicating when the packet was generated.

7. **CRC1:**
   - A 4-byte cyclic redundancy check (CRC) value for integrity verification.

8. **Payload:**
   - The main data portion of the packet, with a maximum size of 512 bytes.

9. **CRC2 (optional):**
   - An optional second CRC value for additional integrity checking.
