#ifndef INCLUDE_TUYAU_OUTPUT_HPP
# define INCLUDE_TUYAU_OUTPUT_HPP

// This is the Output end of a Tuyau. You can get data from it

template <typename Elt>
class TuyauOutput
{
public:
  virtual Elt  pull() = 0;
  virtual bool hasData() const = 0;
};

#endif // INCLUDE_TUYAU_OUTPUT_HPP
