template <typename Elt>
Tuyau<Elt>::Tuyau()
  : input_(queue_, queueGuard_, queueSizeIsValid_)
  , output_(queue_, queueGuard_, queueSizeIsValid_)
{
}

template <typename Elt>
Tuyau<Elt>::~Tuyau()
{
  std::unique_lock<std::mutex> lock(queueGuard_);
}

template <typename Elt>
TuyauInput<Elt>& Tuyau<Elt>::input()
{
  return input_;
}

template <typename Elt>
TuyauOutput<Elt>& Tuyau<Elt>::output()
{
  return output_;
}
  
