#include "gtest/gtest.h"

#include <Buffers/BasicBuffer.hpp>

class BasicBufferTest : public ::testing::Test {
 protected:
  BasicBufferTest()
  {
  }

  ~BasicBufferTest() override
  {
  }
};

TEST_F (BasicBufferTest, Constructions)
{
  BasicBuffer b1(999);
  EXPECT_EQ(b1.size(), 999);

  unsigned char* data = new unsigned char[4096];
  data[1000] = '0';

  BasicBuffer b2(data, 4096);
  EXPECT_EQ(b2.size(), 4096);
  EXPECT_EQ(b2[1000], '0');
  // N.B. It is normally forbidden to use this buffer after the construction
  data[1000] = '1';
  EXPECT_EQ(b2[1000], '1');

  BasicBuffer b3(b2);
  EXPECT_EQ(b2[1000], b3[1000]);
  b3[1000] = '2';
  EXPECT_NE(b2[1000], b3[1000]);

  BasicBuffer b4(std::move(b3));
  EXPECT_EQ(b4[1000], '2');

  BasicBuffer b5(b4, 1000);
  EXPECT_EQ(b4.size(), 1000);
  EXPECT_EQ(b5.size(), 3096);
  EXPECT_EQ(b5[0], '2');
}

