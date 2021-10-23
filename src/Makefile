flags = -Wall -Werror
objects = server.o init.o log.o http.o

server : $(objects)
	gcc -o server $(objects) $(flags)

server.o : server.c server.h
	gcc -c server.c $(flags)

init.o : init.c server.h
	gcc -c init.c $(flags)

log.o : log.c server.h log.h
	gcc -c log.c $(flags)

http.o : server.h log.h http.c
	gcc -c http.c $(flags)

clean : 
	rm server $(objects) $(flags)
