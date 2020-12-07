
template <typename Elt>
TuyauInput<Elt>::TuyauInput(std::queue<Elt>& queue, std::mutex& queueGuard, std::condition_variable& queueSizeIsValid)
  : queue_(queue)
  , queueGuard_(queueGuard)
  , queueSizeIsValid_(queueSizeIsValid)
{
}

template <typename Elt>
void TuyauInput<Elt>::push(Elt&& elt)
{
  std::unique_lock<std::mutex> lock(queueGuard_);

  queue_.push(std::move(elt));

  queueSizeIsValid_.notify_one();
}
