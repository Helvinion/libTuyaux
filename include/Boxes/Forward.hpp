#ifndef INCLUDE_BOXES_FORWARD_HPP
#define INCLUDE_BOXES_FORWARD_HPP

#include <Box.hpp>
#include <Tuyau.hpp>

template <typename Type>
class Forward : public Box<std::tuple<Type>, std::tuple<Type>>
{
public:

  Forward(TuyauOutput<Type>& in, TuyauInput<Type>& out)
    : Box<std::tuple<Type>, std::tuple<Type>>(std::forward_as_tuple(in), std::forward_as_tuple(out))
  {
  }
  
  virtual void process()
  {
    Type elt = this->template getInput<0>().pull().value();
    this->template getOutput<0>().push(std::move(elt));
  }
};

#endif // INCLUDE_BOXES_FORWARD_HPP
