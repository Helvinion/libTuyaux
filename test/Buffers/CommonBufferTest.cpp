#include "gtest/gtest.h"

#include <chrono>
#include <random>
#include <cstring>

#include <Buffers/BasicBuffer.hpp>
#include <Buffers/VirtualBuffer.hpp>

template <typename BufferTypeTemplate>
class CommonBufferTest : public ::testing::Test
{
 protected:
  CommonBufferTest()
  {
    dump_ = new unsigned char[BufferSize];
    data_ = new unsigned char[BufferSize]; 

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);

    for (unsigned int i = 0; i < BufferSize; i++)
    {
      data_[i] = generator() % 256;
      dump_[i] = 0;
    }

    buffer_ = makeBuffer(data_, BufferSize);
  }

  ~CommonBufferTest() override
  {
    delete[] dump_;
    delete buffer_;
  }

  using BufferType = BufferTypeTemplate;
  unsigned int   getSize()   {return BufferSize;}
  unsigned char* getData()   {return data_;}
  unsigned char* getDump()   {return dump_;}
  BufferType*    getBuffer() {return buffer_;}
private:

  BufferType* makeBuffer(unsigned char* data, unsigned int size)
  {
    return new BufferType(data_, BufferSize);
  }
  
  const unsigned int BufferSize = 4096;
  unsigned char*     data_;
  unsigned char*     dump_;
  BufferType*        buffer_;
};

template <>
VirtualBuffer* CommonBufferTest<VirtualBuffer>::makeBuffer(unsigned char* data, unsigned int size)
{
  return new VirtualBuffer(new BasicBuffer(data, size));
}

using BufferTypesToTest = ::testing::Types<BasicBuffer, VirtualBuffer>;
TYPED_TEST_CASE(CommonBufferTest, BufferTypesToTest);

TYPED_TEST (CommonBufferTest, Dumping)
{
  using BufferType = typename TestFixture::BufferType;
  BufferType*    buffer = this->getBuffer();
  unsigned char* data = this->getData();
  unsigned char* dump = this->getDump();
  unsigned int   size = this->getSize();

  buffer->dump(dump);
  EXPECT_EQ(std::memcmp(data, dump, size), 0);
  EXPECT_NE(data, dump);
  data[1000]++;
  EXPECT_NE(std::memcmp(data, dump, size), 0);
}

TYPED_TEST (CommonBufferTest, Spliting)
{
  using BufferType = typename TestFixture::BufferType;
  unsigned char* data = this->getData();
  unsigned char* dump = this->getDump();
  unsigned int   size = this->getSize();

  BufferType* b1 = this->getBuffer();
  EXPECT_EQ(b1->size(), size);

  BufferType* b2 = b1->split(1000);
  EXPECT_EQ(b1->size(), 1000);
  EXPECT_EQ(b2->size(), size - 1000);

  BufferType* b3 = b2->split(1000);
  EXPECT_EQ(b2->size(), 1000);
  EXPECT_EQ(b3->size(), size - 2000);
  
  BufferType* b4 = b3->split(1000);
  EXPECT_EQ(b3->size(), 1000);
  EXPECT_EQ(b4->size(), size - 3000);

  BufferType* b5 = b4->split(1000);  
  EXPECT_EQ(b4->size(), 1000);
  EXPECT_EQ(b5->size(), size - 4000);

  b1->dump(dump);
  b2->dump(dump + 1000);
  b3->dump(dump + 2000);
  b4->dump(dump + 3000);
  b5->dump(dump + 4000);

  EXPECT_EQ(std::memcmp(data, dump, size), 0);

  delete b2;
  delete b3;
  delete b4;
  delete b5;
}
