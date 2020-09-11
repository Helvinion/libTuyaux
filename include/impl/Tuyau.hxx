#include <functional>

template <typename Elt>
Tuyau<Elt>::Tuyau()
{
}

template <typename Elt>
void Tuyau<Elt>::push(Elt&& elt)
{
  std::unique_lock<std::mutex> lock(queueGuard_);

  queue_.push(elt);

  queueSizeIsValid_.notify_one();
}

template <typename Elt>
bool Tuyau<Elt>::hasData() const
{
  std::unique_lock<std::mutex> lock(queueGuard_);
  return hasDataImpl();
}

template <typename Elt>
bool Tuyau<Elt>::hasDataImpl() const
{
  return !queue_.empty();
}

template <typename Elt>
Elt Tuyau<Elt>::pull()
{
  std::unique_lock<std::mutex> lock(queueGuard_);

  // Wait until there is some data in the queue
  queueSizeIsValid_.wait(lock, std::bind(&Tuyau<Elt>::hasDataImpl, this));

  Elt ret = std::move(queue_.front());
  queue_.pop();

  return ret;
}
