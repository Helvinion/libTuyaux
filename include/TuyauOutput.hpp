#ifndef INCLUDE_TUYAU_OUTPUT_HPP
# define INCLUDE_TUYAU_OUTPUT_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

// This is the Output end of a Tuyau. You can get data from it

template <typename Elt>
class TuyauOutput
{
public:
  TuyauOutput(std::queue<Elt>& queue, std::mutex& queueGuard, std::condition_variable& queueSizeIsValid);
  
  Elt  pull();
  bool hasData() const;

private:
  bool hasDataImpl() const;

  // We only have references, it's the responsability of
  // the creator of the object to keep them alive
  std::queue<Elt>&         queue_;
  std::mutex&              queueGuard_;
  std::condition_variable& queueSizeIsValid_;
};

#include <impl/TuyauOutput.hxx>

#endif // INCLUDE_TUYAU_OUTPUT_HPP
