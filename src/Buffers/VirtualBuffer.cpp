#include <Buffers/VirtualBuffer.hpp>
#include <Buffers/BasicBuffer.hpp>

VirtualBuffer::VirtualBuffer(Buffer* buffer)
  : size_(buffer->size())
{
  buffers_.push_back(buffer);
}

VirtualBuffer::VirtualBuffer(VirtualBuffer&& buffer)
  : buffers_(std::move(buffer.buffers_))
  , size_(buffer.size_)
{
}

VirtualBuffer::VirtualBuffer()
  : size_(0)
{
}

VirtualBuffer::VirtualBuffer(const VirtualBuffer& buffer)
  : size_(buffer.size_)
{
  uint8_t* data = new uint8_t[size_];
  buffer.dump(data);
  buffers_.push_back(new BasicBuffer(data, size_));
}

VirtualBuffer::~VirtualBuffer()
{
  for (auto elt: buffers_)
  {
    delete elt;
  }
}

VirtualBuffer& VirtualBuffer::operator=(VirtualBuffer&& buffer)
{
  buffers_ = std::move(buffer.buffers_);
  size_ = buffer.size_;
  buffer.size_ = 0;
  return *this;
}

unsigned char& VirtualBuffer::operator[](unsigned int index)
{
  const VirtualBuffer& constThis = static_cast<const VirtualBuffer&>(*this);
  return const_cast<unsigned char&>(constThis[index]);
}

const unsigned char& VirtualBuffer::operator[](unsigned int index) const
{
  unsigned int currentIndex = 0;

  for (const Buffer* buffer : buffers_)
  {
    if (currentIndex + buffer->size() > index)
    {
      return (*buffer)[index - currentIndex];
    }

    currentIndex += buffer->size();
  }

  throw 42;
}

unsigned int VirtualBuffer::size() const
{
  return size_;
}

VirtualBuffer* VirtualBuffer::split(unsigned int index)
{
  if (index >= size_)
    throw 42;

  unsigned int futureSize = size_ - index;
  unsigned int currentSize = 0;

  // This split is more efficient if we start by its end.
  VirtualBuffer* ret = new VirtualBuffer();

  while (currentSize + buffers_.back()->size() <= futureSize)
  {
    ret->buffers_.push_front(buffers_.back());
    currentSize += buffers_.back()->size();
    buffers_.pop_back();
  }

  if (currentSize < futureSize)
  {
    // The split index has to be computed from the amount of bytes to take from the buffer
    unsigned int missing = futureSize - currentSize;
    ret->buffers_.push_front(buffers_.back()->split(buffers_.back()->size() - missing));
  }

  size_ = index;
  ret->size_ = futureSize;

  return ret;
}

void VirtualBuffer::push_back(Buffer* buffer)
{
  buffers_.push_back(buffer);
  size_ += buffer->size();
}

void VirtualBuffer::push_front(Buffer* buffer)
{
  buffers_.push_front(buffer);
  size_ += buffer->size();
}

void VirtualBuffer::dump(unsigned char* dst) const
{
  unsigned int index = 0;

  for (const auto& elt : buffers_)
  {
    elt->dump(dst + index);
    index += elt->size();
  }
}
