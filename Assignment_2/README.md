#Team 3 Assignment 2 TFTP Server
The server establishes a UDP server socket. It then waits for an RRQ. It uses select() to maintain concurrent connections.
For every received message it checks if the client sending message is a new client or an existing client.
Once it receives the RRQ, if it is error free, it accepts the request, adds the client details to the list of clients and starts sending the blocks of requested file. But, if the number of clients accessing server exceeds maximum (10 in our case), it makes the client wait till a slot is available for the client in the client list.
Messages are encoded using encoding algorithm.
After every block sent, the server waits for an acknowledgement from the client before sending the next block to it. Once it receives the acknowledgement for the current block sent, it sends the next block.
Once the transmission of the entire file is complete, it assumes closure of the connection and clears the allotted client details from the client list.
In case of an error, it sends an error message to the client.
