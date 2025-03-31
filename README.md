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
