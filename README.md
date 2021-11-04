# Supported capabilities

### Server capabilities

- HTTP Request Pipelining
- Concurrent connections upto a limit defined by MAX\_CLIENT
- Client timeout
- Logging for both Server and Client events
- Error handler for both server and client errors

### HTTP Capabilities

- GET, HEAD and POST Methods
- Supported HTTP Codes : 200, 400, 404, 411, 413, 414, 500, 501, 505

# In Development

- Daemonising the server
- Supporting HTTPs
- Supporting CGI

# Codebase Notes

### Symbols used

- MAX\_CLIENT : Maximum number of clients allowed, (backlog)
- MAX\_BUF : Maximum buffer size for http messages
- TRUE : true
- FALSE : false


### Data Structures

Linked list is used to store the data of clients connected to the server.
It contains :
- Client fd
- Client IP
- Client Port
- ?Buffer?
Need a buffer for HTTP Pipelining

### Functions

- wait\_on\_client: implement select() for synchronous connections.
- get\_client() : implement accept().
- drop\_client() : remove client on error and remove from the client list.
- send\_400() : Send 400 status
- send\_404() : Send 404
- send\_501() : Send 501
