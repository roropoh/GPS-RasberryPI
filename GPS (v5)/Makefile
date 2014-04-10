CC=gcc -Wall
LIB = -lgps -lncurses

dcgps: dcgps.o gpsprint.o gps-utils.o
	$(CC) -o dcgps dcgps.o gpsprint.o gps-utils.o $(LIB)

clean:
	rm -f *.o core.* dcgps

dcgps.o: dcgps.c
	$(CC) -c dcgps.c

gpsprint.o: gpsprint.c
	$(CC) -c gpsprint.c	

gps-utils.o: gps-utils.c
	$(CC) -c gps-utils.c
