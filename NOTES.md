## Checkout
	- AF_BLUETOOTH
	- packet { man 7 packet }
	- SOCK_RAW


# Notes for project

## Check Point 1 :

	-  Parse HTTP 1.1 requests and classify them as “good” or “bad” based on the provided RFC [3]. For
all “good” requests, you will simply echo back the original request. For all “bad” requests, you will
return an HTTP response with 400 as the error code.

	- Test using our provided cp1 checker.py test script (read that script and understand it too) Try to
“break” your server and make it crash — and then patch the bugs you find that make it crash.

	-  Note that Apache Bench expects a “Content-Length” field on every reply, even error messages. Every
valid request we send you will include a “Content-Length” field.

	- If the request is validly formed but not a HEAD, GET, or POST, then you may either reply with a
400 or a 501 error code for this checkpoint (in CP2, you must reply with a 501).


## Check Point 2 :

	-  Respond to properly formatted HTTP HEAD and GET requests. You do not need to respond to
POST requests yet (continue to echo reply to POST requests if the request is correctly formatted; send
an error 400 if the request is malformed).

	-   Support five HTTP 1.1 error codes: 400, 404, 408, 501, and 505. 404 is for files not found; 408 is for
connection timeouts; 501 is for unsupported methods; 505 is for bad version numbers. Everything else
can be handled with a 400.

	- Handle pipelined requests.

	- Perform Logging like Aapche

	- Last-modified
	- Chunking support not needed
	- Conditional GETs not needed
	- Build own hashtable library if needed
	- do not assume that they always fit inside one buffer. Be prepared to parse across buffer boundaries.

	-  assume requests to your server have the “Content-Length” field, if applicable. If it is missing for
a POST request, return a 400 response. For GET and HEAD request if the field is missing you can
assume the content length is 0.

	- You are allowed to use that or a simplified version of it. No need to support all well-known
MIME-types, just the most common ones: text/html, text/css, image/png, image/jpeg, image/gif and
maybe a few others, up to your discretion.

	- for LAst Modified field use stat()

	- You may reject any header line larger than 8192 bytes. Note that this is different than a “Content-
	Length” of greater than 8192. Additionally, if you do reject a request, you must find and parse the
	next request properly for pipelining purposes.

	- No need to support /n instead of /r/n


GRADE : Responds to wrong version with 505 (2%)
• Responds to unsupported method with 501
(2%)
• Responds to other problems or invalid re-
quests with 400 (2%)
• Responds to timeouts with 408 (4%)
• Hidden tests (14%)

## Check Point 3 :
	
	- Daemonise the server
	- Implement HTTPS and CGI : CGI - 3875 and TLS - 2818

### Implementing Daemon :

	- For SIGHUP, you need to rehash the server. This is used in practice to ensure that servers experience
minimal downtime. An administrator can update the certificate for the server, then send a SIGHUP signal
that will cause the server to update and use the new certificate.

	- For SIGTERM, you need to gracefuly shut down the connections, any memory usage, and ultimately the
entire server.

### Implementing CGI :
	
	- CGI :  RFC 3875 		URI : RFC 2396
	- Check CGI variables from PDF

	-  Implement your CGI module. Any URI starting with “/cgi/” will be handled by a single command
	line–specified executable via a CGI interface coded by you. We have also provided example code of a
	CGI runner in the handout for CP3.

	(a) CGI URI’s may accept GETs, POSTs, and HEADs; your job is not to decide this, just pass along
	information to the program being called.

	(b) You need to pipe stdin, pipe stdout, fork(), setup environment variables per the CGI specification,
	and execve() the executable (it should be executable) Note: Watch the piped fd’s in the parent
	process using your select() loop. Just add them to the appropriate select() sets and treat them
	like sockets, except you have to pipe them further to specific sockets.

	(c) Pass any message body (especially for POSTs) via stdin to the CGI executable.

	(d) Receive any response over stdout until the process dies (monitor process status), or there is
	nothing more to read, or a broken pipe is encountered.

	(e) If the CGI program fails in any way, return a 500 response to the client, otherwise send all bytes
	from the stdout of the spawned process to the requesting client.

	(f) The CGI application will produce headers and message body data as it sees fit, you do not need
	to modify or inspect these bytes at all.
	
### Implementing HTTPS:
	
	SEE PDF
