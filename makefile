all:serial_port_test

serial_port_test:main.o serial_port.o
	g++ main.o serial_port.o -o serial_port_test

main.o:src/main.cpp
	g++ -I inc -c src/main.cpp -o main.o

#poll_controler.o:src/poll_controler.cpp
#	g++ -I inc -c src/poll_controler.cpp -o poll_controler.o

serial_port.o:src/serial_port.cpp
	g++ -I inc -c src/serial_port.cpp -o serial_port.o

claen:
	rm *.o

