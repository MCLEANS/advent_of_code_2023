CC = g++

CFLAGS += -c -Wall --std=c++17
CFLAGS += -I base_state_machine
CFLAGS += -I Motor

LFLAGS = -Wall

OBJS += main.o
OBJS += SM.o
OBJS += Motor.o


all : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main 

main.o : main.cpp 
	$(CC) $(CFLAGS) main.cpp 

SM.o : base_state_machine/SM.h base_state_machine/SM.cpp
	$(CC) $(CFLAGS) base_state_machine/SM.cpp

Motor.o : Motor/Motor.h Motor/Motor.cpp
	$(CC) $(CFLAGS) Motor/Motor.cpp

run :
	./main 

clean :
	rm -r *.o main