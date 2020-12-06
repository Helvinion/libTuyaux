LIB_SRC=src/Buffers/BasicBuffer.cpp src/Buffers/Buffer.cpp src/Buffers/VirtualBuffer.cpp src/Packets/TsPacket.cpp
LIB_OBJ=$(patsubst src/%.cpp,obj/%.o,$(LIB_SRC))
CC=g++
LIB_INCLUDE_FLAGS=-I include

TEST_INCLUDE_FLAGS=-I /usr/local/include/gtest -I include
TEST_LINK_FLAGS=-lgtest
TEST_SRC=test/main.cpp test/Buffers/BasicBufferTest.cpp test/Buffers/CommonBufferTest.cpp test/Packets/TsPacketTest.cpp
TEST_OBJ=$(patsubst test/%.cpp,obj/test/%.o,$(TEST_SRC))

all: libtuyaux.so test

clean::
	rm -rf obj

libtuyaux.so:$(LIB_OBJ)
	g++ -g -o $@  $^ -shared

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) -g -c -fPIC $(LIB_INCLUDE_FLAGS) -o $@ $^

# TESTS

test: testLibTuyaux
	./testLibTuyaux

testLibTuyaux: $(TEST_OBJ) $(LIB_OBJ) libtuyaux.so
	g++ -g $(LIB_OBJ) $(TEST_OBJ) $(TEST_INCLUDE_FLAGS) $(TEST_LINK_FLAGS) -o $@

obj/test/%.o: test/%.cpp
	mkdir -p $(@D)
	$(CC) -g -c $(TEST_INCLUDE_FLAGS) -o $@ $^
