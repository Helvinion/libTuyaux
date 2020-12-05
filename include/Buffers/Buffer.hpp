#ifndef INCLUDE_BUFFERS_BUFFER_HPP
# define INCLUDE_BUFFERS_BUFFER_HPP

class Buffer
{
public:
  virtual ~Buffer() {};
  virtual unsigned char& operator[](unsigned int index) = 0;
  virtual const unsigned char& operator[](unsigned int index) const = 0;
  virtual unsigned int size() const = 0;
  virtual Buffer* split(unsigned int index) = 0;
  virtual void dump(unsigned char* dst) const = 0;
};

#endif // INCLUDE_BUFFERS_BUFFER_HPP
