#ifndef INCLUDE_BUFFERS_BASICBUFFER_HPP
# define INCLUDE_BUFFERS_BASICBUFFER_HPP

#include <Buffers/Buffer.hpp>
#include <memory>

class BasicBuffer : public Buffer
{
public:
  BasicBuffer(unsigned int size);
  BasicBuffer(unsigned char* data, unsigned int size);
  BasicBuffer(const Buffer& buffer);
  BasicBuffer(BasicBuffer&& buffer);
  BasicBuffer(BasicBuffer& buffer, unsigned int index);
  
  virtual unsigned char& operator[](unsigned int index);
  virtual const unsigned char& operator[](unsigned int index) const;
  virtual unsigned int size() const;
  virtual BasicBuffer* split(unsigned int index);
  virtual void dump(unsigned char* dst) const;

private:
  std::shared_ptr<unsigned char[]> data_;
  unsigned int                     index_;
  unsigned int                     size_;
};

#endif // INCLUDE_BUFFERS_BASICBUFFER_HPP
