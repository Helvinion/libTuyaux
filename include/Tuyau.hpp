#ifndef INCLUDE_TUYAU_HPP
# define INCLUDE_TUYAU_HPP

#include "TuyauInput.hpp"
#include "TuyauOutput.hpp"

#include <queue>
#include <mutex>
#include <condition_variable>


template <typename Elt>
class Tuyau : public TuyauInput<Elt>, public TuyauOutput<Elt>
{
public:
  Tuyau();
  
  virtual void push(Elt&& elt);
  virtual Elt  pull();
  virtual bool hasData() const;

private:
  bool hasDataImpl() const;

  mutable std::mutex      queueGuard_;
  std::condition_variable queueSizeIsValid_;

  std::queue<Elt> queue_;
};

#include "impl/Tuyau.hxx" 

#endif // INCLUDE_TUYAU_HPP
