HELPER = private
TESTPROG = tests/test

CC = g++

CFLAGS = $(shell pkg-config --cflags openssl) -Wall -O2 -std=c++20
LFLAGS = $(shell pkg-config --libs openssl)

TESTCFLAGS = $(CFLAGS) $(shell pkg-config --cflags check)
TESTLFLAGS = $(LFLAGS) $(shell pkg-config --libs check)

OBJECTS = private-method.o acquire-method.o stanza.o config.o uri.o hash.o hex.o

TESTSOURCES = $(wildcard tests/*.cpp)
TESTOBJECTS = $(OBJECTS) $(TESTSOURCES:%.cpp=%.o)


all: $(HELPER)
	sh test.sh

include $(wildcard *.d */*.d)

$(HELPER): main.o $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.cpp
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

tests/%.o: tests/%.cpp
	$(CC) -MMD -MP -c $< -o $@ $(TESTCFLAGS)

$(TESTPROG): $(TESTOBJECTS)
	$(CC) $^ -o $@ $(TESTLFLAGS)

test: $(TESTPROG)
	$(TESTPROG)

clean:
	$(RM) $(HELPER) $(TESTPROG) *.o *.d */*.o */*.d
