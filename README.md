# Supported capabilities

### Server capabilities

- HTTP Request Pipelining
- Concurrent connections upto a limit defined by MAX\_CLIENT
- Client timeout
- Logging for both Server and Client events
- Error handler for both server and client errors
- Exit handler to free the client list and data

### HTTP Capabilities

- GET, HEAD and POST Methods
- Supported HTTP Codes : 200, 400, 404, 411, 413, 414, 500, 501, 505

# Codebase Notes

### Symbols used

```c
// Universal Defines
#define TRUE 1
#define FALSE 0

// Server Defines
#define MAX_CLIENT 1024
#define REQ_PIPELINE 10

// Logger Defines
#define MAX_LOG_BUF 512

// Request Defines
#define REQ_LEN 8096
#define URI_LEN 1024
#define METHOD_LEN 10
#define VERSION_LEN 10

// Response Defines
#define RESP_LEN 8096
#define RESP_CODE_LEN 56
#define RESP_LINE_LEN (RESP_CODE_LEN + 16)
#define DATE_LEN 30

// Error defines
#define INFO 0
#define WARN 1
#define ERR 2

// Define Methods here
#define GET 0
#define HEAD 1
#define POST 2
```

### Data Structures

Linked List to store connected Client information. Linked List seamed feasible as the max concurrent clients are by default 1024, so it seems not much effect on performance. <br>

```c
typedef struct client{
	int cfd;
	ssize_t readData;
	struct client *client_next;
	char host[NI_MAXHOST];
	char respBuf[RESP_LEN];
	char reqBuf[REQ_PIPELINE*REQ_LEN];
} CLIENT;
```
cfd : File descriptor for the client <br>
readDate : Number of bytes of data read from the descriptor<br>
host : String containing the hostname
respBuf : Buffer to contain the response message
reqBuf : Buffer to contain the request messagea
client\_next : Pointer to the next client node<br>


Structure to store request messages :<br>

```c
typedef struct request {
	char method[METHOD_LEN];
	char url[URI_LEN];
	char version[VERSION_LEN];
	char **headers;
} REQUEST;

```

method : Contains Request method string - GET, HEAD, POST, etc.<br>
url : URI for the source<br>
version : String to contain HTTP Version<br>
headers : a pointer to an array of strings containing the remaining headers<br>

Structure to store response messages :<br>

```c
typedef struct response{
	int version;
	int code;
	char code_str[RESP_CODE_LEN];
	char **headers;
} RESPONSE;
```

version : HTTP Version of the response used to construct response message line<br>
code : Response code<br>
code\_str : Description for the code<br>
headers : a pointer to an array of strings containing the remaining headers<br>

### Functions

- wait\_on\_client: implements select() for synchronous connections.
- get\_client() : implements accept() and adds the client to the linked list.
- drop\_client() : remove client on error and remove from the client list.
- send\_code() : generic function to send error codes as response.
