#ifndef INCLUDE_TUYAU_OUTPUT_HPP
# define INCLUDE_TUYAU_OUTPUT_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <optional>
#include <functional>

// This is the Output end of a Tuyau. You can get data from it

template <typename Elt>
class TuyauOutput
{
public:
  TuyauOutput(std::queue<Elt>& queue, std::mutex& queueGuard, std::condition_variable& queueSizeIsValid);
  
  std::optional<Elt> pull();
  bool               hasData() const;
  void               stopPull();

  void setCallback(const std::function<void(Elt&&)>& callback);
  void resetCallback();

private:
  bool hasDataImpl() const;
  bool shouldStopWaiting() const;
  
  // Optionnal function to call each time an element is pushed
  std::function<void(Elt&&)> callback_;
  std::thread callbackThread_;
  bool        endThread_;
  void callbackLoop();

  // We only have references, it's the responsability of
  // the creator of the object to keep them alive
  std::queue<Elt>&         queue_;
  std::mutex&              queueGuard_;
  std::condition_variable& queueSizeIsValid_;
  bool                     stopWaiting_;
};

#include <impl/TuyauOutput.hxx>

#endif // INCLUDE_TUYAU_OUTPUT_HPP
