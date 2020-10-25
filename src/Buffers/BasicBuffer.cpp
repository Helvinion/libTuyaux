#include <Buffers/BasicBuffer.hpp>

BasicBuffer::BasicBuffer(unsigned int size)
  : data_(nullptr)
  , size_(size)
{
  data_ = new unsigned char[size_];
}

BasicBuffer::BasicBuffer(unsigned char* data, unsigned int size)
  : data_(data)
  , size_(size)
{
}

BasicBuffer::BasicBuffer(const Buffer& buffer)
  : BasicBuffer(buffer.size())
{
  for (unsigned i = 0; i < size_; i++)
    data_[i] = buffer[i];
}

BasicBuffer::BasicBuffer(BasicBuffer&& buffer)
  : BasicBuffer(buffer.data_, buffer.size_)
{
  buffer.data_ = nullptr;
  buffer.size_ = 0;
}


unsigned char& BasicBuffer::operator[](unsigned int index)
{
  if (index > size_ || data_ == nullptr)
      throw 42;

    return data_[index];
}

const unsigned char& BasicBuffer::operator[](unsigned int index) const
{
  if (index > size_ || data_ == nullptr)
      throw 42;

    return data_[index];
}

unsigned int BasicBuffer::size() const
{
  return size_;
}