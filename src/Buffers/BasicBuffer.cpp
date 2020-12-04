#include <Buffers/BasicBuffer.hpp>

#include <cstring>

BasicBuffer::BasicBuffer(unsigned int size)
  : data_(new unsigned char[size])
  , index_(0)
  , size_(size)
{
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
  : data_(std::move(buffer.data_))
  , index_(buffer.index_)
  , size_(buffer.size_)
{
  buffer.index_ = 0;
  buffer.size_ = 0;
}

BasicBuffer::BasicBuffer(BasicBuffer& buffer, unsigned int index)
  : data_(buffer.data_)
  , index_(buffer.index_ + index)
  , size_(buffer.size_ - index)
{
  buffer.size_ = index;
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

BasicBuffer* BasicBuffer::split(unsigned int index)
{
  BasicBuffer* ret = new BasicBuffer(*this, index);
  return ret;
}

void BasicBuffer::dump(unsigned char* dst) const
{
  std::memcpy(dst, data_.get(), size_);
}
