#ifndef INCLUDE_BOXES_NOISEGENERATOR_HPP
#define INCLUDE_BOXES_NOISEGENERATOR_HPP

#include <Box.hpp>
#include <Tuyau.hpp>
#include <Buffers/VirtualBuffer.hpp>

class NoiseGenerator : public AsyncBox<std::tuple<>, std::tuple<VirtualBuffer>>
{
public:
  NoiseGenerator(TuyauInput<VirtualBuffer>& out, uint64_t bitrate, uint32_t granularity);

private:
  virtual void run();

  uint64_t bitrate_;
  uint64_t granularity_;
};

#endif // INCLUDE_BOXES_NOISEGENERATOR_HPP
