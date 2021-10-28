flags = -Wall -Werror 
objects = server.o log.o http.o parse.o error_codes.o

server : $(objects)
	gcc -o server $(objects) $(flags)

server.o : server.c server.h
	gcc -c server.c $(flags)

log.o : log.c server.h
	gcc -c log.c $(flags)

http.o : server.h http.c
	gcc -c http.c $(flags)

parse.o : parse.c server.h
	gcc -c parse.c $(flags)

error_codes.o : error_codes.c server.h
	gcc -c error_codes.c $(flags)

clean : 
	rm server $(objects) 
