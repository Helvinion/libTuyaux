#ifndef INCLUDE_BOXES_DUPLICATE_HPP
# define INCLUDE_BOXES_DUPLICATE_HPP

template <typename Type>
class Duplicate : public Box<std::tuple<Type>, std::tuple<Type, Type>>
{
public:

  Duplicate(TuyauOutput<Type>& in, TuyauInput<Type>& out1, TuyauInput<Type>& out2)
    : Box<std::tuple<Type>, std::tuple<Type, Type>>(std::forward_as_tuple(in), std::forward_as_tuple(out1, out2))
  {
  }
  
  virtual void process()
  {
    Type elt = this->template getInput<0>().pull().value();
    Type cpy(elt);
    this->template getOutput<0>().push(std::move(elt));
    this->template getOutput<1>().push(std::move(cpy));
  }
};

#endif // INCLUDE_BOXES_DUPLICATE_HPP
