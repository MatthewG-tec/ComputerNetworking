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
