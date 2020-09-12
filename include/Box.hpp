#ifndef INCLUDE_BOX_HPP
# define INCLUDE_BOX_HPP

#include <tuple>

#include "Tuyau.hpp"

template <typename Input, typename Output> class Box;

template <typename... InputTypes, typename... OutputTypes>
class Box<std::tuple<InputTypes...>, std::tuple<OutputTypes...>>
{
public:
  Box(std::tuple<TuyauOutput<InputTypes>&...>&& inputs, std::tuple<TuyauInput<OutputTypes>&...>&& outputs)
    : inputs_(inputs)
    , outputs_(outputs)
  {
  }

  virtual void process() = 0;

protected:  
  template <int N>
  typename std::tuple_element<N, std::tuple<TuyauOutput<InputTypes>&...>>::type getInput()
  {
    return std::get<N>(inputs_);
  }

  template <int N>
  typename std::tuple_element<N, std::tuple<TuyauInput<OutputTypes>&...>>::type getOutput()
  {
    return std::get<N>(outputs_);
  }

private:
  // This is a tuple of references,
  // each element referencing a TuyauOutput from which we'll pull data,
  // each TuyauOutput giving objects,
  // each object being listed in InputTypes
  std::tuple<TuyauOutput<InputTypes>&...>   inputs_;
  std::tuple<TuyauInput<OutputTypes>&...> outputs_;
};

#endif // INCLUDE_BOX_HPP
