LIB_SRC=src/Buffers/BasicBuffer.cpp src/Buffers/Buffer.cpp src/Buffers/VirtualBuffer.cpp src/Packets/TsPacket.cpp src/Boxes/NoiseGenerator.cpp
LIB_OBJ=$(patsubst src/%.cpp,obj/%.o,$(LIB_SRC))
CC=g++
CC_FLAGS=-g -std=c++17
LIB_INCLUDE_FLAGS=-I include
LD_LIBS_FLAGS=-lpthread

TEST_INCLUDE_FLAGS=-I /usr/local/include/gtest -I include
TEST_LINK_FLAGS=-lgtest
TEST_SRC=test/main.cpp test/Buffers/BasicBufferTest.cpp test/Buffers/CommonBufferTest.cpp test/Packets/TsPacketTest.cpp test/Boxes/DiscardBoxTest.cpp
TEST_OBJ=$(patsubst test/%.cpp,obj/test/%.o,$(TEST_SRC))

all: libtuyaux.so test

clean::
	rm -rf obj

libtuyaux.so:$(LIB_OBJ)
	g++ $(CC_FLAGS) -o $@  $^ -shared $(LD_LIBS_FLAGS)

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c -fPIC $(LIB_INCLUDE_FLAGS) -o $@ $^

# TESTS

test: testLibTuyaux
	./testLibTuyaux

testLibTuyaux: $(TEST_OBJ) $(LIB_OBJ) libtuyaux.so
	g++ $(CC_FLAGS) $(LIB_OBJ) $(TEST_OBJ) $(TEST_INCLUDE_FLAGS) $(TEST_LINK_FLAGS) -o $@ $(LD_LIBS_FLAGS)

obj/test/%.o: test/%.cpp
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $(TEST_INCLUDE_FLAGS) -o $@ $^
