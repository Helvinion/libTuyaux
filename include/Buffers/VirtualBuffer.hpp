#ifndef INCLUDE_BUFFERS_VIRTUALBUFFER_HPP
# define INCLUDE_BUFFERS_VIRTUALBUFFER_HPP

#include <Buffers/Buffer.hpp>

#include <deque>

class VirtualBuffer : public Buffer
{
public:
  VirtualBuffer(Buffer* buffer);
  VirtualBuffer(VirtualBuffer&& buffer);
  virtual ~VirtualBuffer();
  
  virtual unsigned char& operator[](unsigned int index);
  virtual const unsigned char& operator[](unsigned int index) const;
  virtual unsigned int size() const;
  virtual VirtualBuffer* split(unsigned int index);
  virtual void dump(unsigned char* dst) const;

  void push_back(Buffer* buffer);
  void push_front(Buffer* buffer);

private:

  VirtualBuffer();
  std::deque<Buffer*> buffers_;
  unsigned int        size_;
};

#endif // INCLUDE_BUFFERS_VIRTUALBUFFER_HPP
