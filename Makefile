HELPER = private

CC = g++

CFLAGS = -Wall -O2
LFLAGS =

OBJECTS = main.o

all: $(HELPER)
	./$(HELPER)

include $(wildcard *.d */*.d)

$(HELPER): $(OBJECTS)
	$(CC) $^ -o $(HELPER) $(LFLAGS)

%.o: %.cpp
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(HELPER) *.o *.d */*.o */*.d
