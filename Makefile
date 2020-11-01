LIB_SRC=src/Buffers/BasicBuffer.cpp src/Buffers/Buffer.cpp src/Buffers/VirtualBuffer.cpp
LIB_OBJ=$(patsubst src/%.cpp,obj/%.o,$(LIB_SRC))
CC=g++
LIB_INCLUDE_FLAGS=-I include

TEST_INCLUDE_FLAGS=-I /usr/local/include/gtest
TEST_LINK_FLAGS=-lgtest
TEST_SRC=test/main.cpp
TEST_OBJ=$(patsubst test/%.cpp,obj/test/%.o,$(TEST_SRC))

all: libtuyaux.so test

clean::
	rm -rf obj

libtuyaux.so:$(LIB_OBJ)
	g++ -o $@  $^ -shared

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CC) -c -fPIC $(LIB_INCLUDE_FLAGS) -o $@ $^

# TESTS

test: testLibTuyaux

testLibTuyaux: $(TEST_OBJ) $(LIB_OBJ)
	g++ $(LIB_OBJ) $(TEST_OBJ) $(TEST_INCLUDE_FLAGS) $(TEST_LINK_FLAGS) -o $@

obj/test/%.o: test/%.cpp
	mkdir -p $(@D)
	$(CC) -c $(TEST_INCLUDE_FLAGS) -o $@ $^
