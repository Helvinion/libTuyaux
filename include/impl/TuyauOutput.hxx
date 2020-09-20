#include <functional>

template <typename Elt>
TuyauOutput<Elt>::TuyauOutput(std::queue<Elt>& queue, std::mutex& queueGuard, std::condition_variable& queueSizeIsValid)
  : queue_(queue)
  , queueGuard_(queueGuard)
  , queueSizeIsValid_(queueSizeIsValid)
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
Elt TuyauOutput<Elt>::pull()
{
  std::unique_lock<std::mutex> lock(queueGuard_);

  // Wait until there is some data in the queue
  queueSizeIsValid_.wait(lock, std::bind(&TuyauOutput<Elt>::hasDataImpl, this));

  Elt ret = std::move(queue_.front());
  queue_.pop();

  return ret;
}
