#Server
The http proxy server is TCP server which:
    Accepts http request from multiple clients
    Uses select() to manage concurrent connections
    Checks if the entry is present in the cache. If it is sends the document from cache to client.
    If not in cache or entry is expired, forwards the request to the web server
    Receives document from the web server, stores the entry in cache and forwards it to the client
    Logs messages during the entire process
    
#Client
The http client :
    Connects to the proxy server
    Sends request for document
    Receives the document and saves it in a file
    Logs messages during the entire process

To run the code:
  make - to compile all the files
  make server - to compile only the server
  make client - to compile only the client
  make clean - to delete all output files
