#ifndef INCLUDE_BOXES_DISCARD_HPP
#define INCLUDE_BOXES_DISCARD_HPP

#include <Box.hpp>
#include <Tuyau.hpp>

template <typename Type>
class Discard : public Box<std::tuple<Type>, std::tuple<>>
{
public:

  Discard(TuyauOutput<Type>& in)
    : Box<std::tuple<Type>, std::tuple<>>(std::forward_as_tuple(in), std::forward_as_tuple())
  {
  }
  
  virtual void process()
  {
    Type elt = this->template getInput<0>().pull().value();
  }
};

#endif // INCLUDE_BOXES_DISCARD_HPP
