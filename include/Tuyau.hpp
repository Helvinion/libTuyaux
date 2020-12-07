#ifndef INCLUDE_TUYAU_HPP
# define INCLUDE_TUYAU_HPP

#include "TuyauInput.hpp"
#include "TuyauOutput.hpp"

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename Elt>
class Tuyau
{
public:
  Tuyau();
  ~Tuyau();

  TuyauInput<Elt>&  input();
  TuyauOutput<Elt>& output();
  
private:
  std::queue<Elt>         queue_;
  mutable std::mutex      queueGuard_;
  std::condition_variable queueSizeIsValid_;

  TuyauInput<Elt>  input_;
  TuyauOutput<Elt> output_;
};

#include "impl/Tuyau.hxx" 

#endif // INCLUDE_TUYAU_HPP
