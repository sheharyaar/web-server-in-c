# Symbols used

- MAX_CLIENT : Maximum number of clients allowed, (backlog)
- MAX_BUF : Maximum buffer size for http messages
- TRUE : true
- FALSE : false


# Data Structures

Linked list is used to store the data of clients connected to the server.
It contains :
	- Client fd
	- Client IP
	- Client Port
	- ?Buffer?
Need a buffer for HTTP Pipelining

# Functions

- wati_on_client: implement select() for synchronous connections.
- get_client() : implement accept().
- drop_client() : remove client on error and remove from the client list.
- send_400() : Send 400 status
- send_404() : Send 404
- send_501() : Send 501
