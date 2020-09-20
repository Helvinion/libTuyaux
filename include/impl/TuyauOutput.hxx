#include <functional>

template <typename Elt>
TuyauOutput<Elt>::TuyauOutput(std::queue<Elt>& queue, std::mutex& queueGuard, std::condition_variable& queueSizeIsValid)
  : queue_(queue)
  , queueGuard_(queueGuard)
  , queueSizeIsValid_(queueSizeIsValid)
  , endThread_(true)
  , stopWaiting_(false)
{
}

template <typename Elt>
bool TuyauOutput<Elt>::hasData() const
{
  std::unique_lock<std::mutex> lock(queueGuard_);
  return hasDataImpl();
}

template <typename Elt>
bool TuyauOutput<Elt>::hasDataImpl() const
{
  return !queue_.empty();
}

template <typename Elt>
std::optional<Elt> TuyauOutput<Elt>::pull()
{
  std::unique_lock<std::mutex> lock(queueGuard_);

  // Wait until there is some data in the queue
  queueSizeIsValid_.wait(lock, std::bind(&TuyauOutput<Elt>::shouldStopWaiting, this));
  stopWaiting_ = false;

  std::optional<Elt> ret;
  if (hasDataImpl())
  {
    ret = std::move(queue_.front());
    queue_.pop();
  }
    
  return ret;
}

template <typename Elt>
void TuyauOutput<Elt>::resetCallback()
{
  // Stop the thread if any
  if (callbackThread_.joinable())
  {
    endThread_ = true;
    stopPull();
    callbackThread_.join();
  }

  callback_ = nullptr;
}

template <typename Elt>
void TuyauOutput<Elt>::setCallback(const std::function<void(Elt&&)>& callback)
{
  resetCallback();

  // Start the thread
  callback_ = callback;
  endThread_ = false;
  callbackThread_ = std::thread(std::bind(&TuyauOutput<Elt>::callbackLoop, this));
}

template <typename Elt>
void TuyauOutput<Elt>::callbackLoop()
{
  while (!endThread_)
  {
    std::optional<Elt> elt = pull();
    if (elt)
      callback_(std::move(elt.value()));
  }
}

template <typename Elt>
void TuyauOutput<Elt>::stopPull()
{
  stopWaiting_ = true;
}

template <typename Elt>
bool TuyauOutput<Elt>::shouldStopWaiting() const
{
  return hasDataImpl() || stopWaiting_;
}
