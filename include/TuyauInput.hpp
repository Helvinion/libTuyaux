#ifndef INCLUDE_TUYAU_INPUT_HPP
# define INCLUDE_TUYAU_INPUT_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

// This is the Input end of a Tuyau. You can put data in it

template <typename Elt>
class TuyauInput
{
public:
  TuyauInput(std::queue<Elt>& queue, std::mutex& queueGuard, std::condition_variable& queueSizeIsValid);

  void push(Elt&& elt);
private:
  // We only have references, it's the responsability of
  // the creator of the object to keep them alive
  std::queue<Elt>&         queue_;
  std::mutex&              queueGuard_;
  std::condition_variable& queueSizeIsValid_;
};

#include <impl/TuyauInput.hxx>

#endif // INCLUDE_TUYAU_INPUT_HPP
