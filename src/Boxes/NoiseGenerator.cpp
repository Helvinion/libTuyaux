#include <Boxes/NoiseGenerator.hpp>
#include <Buffers/BasicBuffer.hpp>
#include <Buffers/VirtualBuffer.hpp>

#include <iostream>
#include <chrono>

NoiseGenerator::NoiseGenerator(TuyauInput<VirtualBuffer>& out, uint64_t bitrate, uint32_t granularity)
    : AsyncBox<std::tuple<>, std::tuple<VirtualBuffer>>(std::forward_as_tuple(), std::forward_as_tuple(out))
    , bitrate_(bitrate)
    , granularity_(granularity)
{
}

void NoiseGenerator::run()
{
    std::chrono::duration<int, std::milli> periodMs(1000 * granularity_ * 8 / bitrate_); // b / (b/s) => s
    while(!stopped())
    {
        std::this_thread::sleep_for(periodMs);
        VirtualBuffer buffer(new BasicBuffer(granularity_));
        this->template getOutput<0>().push(std::move(buffer));
    }
}
