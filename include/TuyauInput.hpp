#ifndef INCLUDE_TUYAU_INPUT_HPP
# define INCLUDE_TUYAU_INPUT_HPP

// This is the Input end of a Tuyau. You can put data in it

template <typename Elt>
class TuyauInput
{
public:
  virtual void push(Elt&& elt) = 0;
};

#endif // INCLUDE_TUYAU_INPUT_HPP
