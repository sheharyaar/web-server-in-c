flags = -Wall -Werror
objects = server.o log.o 

server : $(objects)
	gcc -o server $(objects) $(flags)

server.o : server.c server.h log.h
	gcc -c server.c $(flags)

log.o : log.c log.h
	gcc -c log.c $(flags)

#http.o : server.h log.h http.c
#	gcc -c http.c $(flags)

clean : 
	rm server $(objects) 
