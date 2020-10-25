class Buffer
{
public:
  virtual unsigned char& operator[](unsigned int index) = 0;
  virtual const unsigned char& operator[](unsigned int index) const = 0;

  virtual unsigned int size() const = 0;
};
