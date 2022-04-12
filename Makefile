HELPER = private

CC = g++

CFLAGS = -Wall -O2 -std=c++20
LFLAGS =

OBJECTS = main.o private-method.o acquire-method.o stanza.o config.o

all: $(HELPER)
	sh test.sh

include $(wildcard *.d */*.d)

$(HELPER): $(OBJECTS)
	$(CC) $^ -o $(HELPER) $(LFLAGS)

%.o: %.cpp
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(HELPER) *.o *.d */*.o */*.d
