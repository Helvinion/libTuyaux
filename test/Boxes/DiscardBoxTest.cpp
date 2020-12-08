#include "gtest/gtest.h"

#include <chrono>
#include <thread>

#include <Buffers/BasicBuffer.hpp>
#include <Boxes/Discard.hpp>
#include <Boxes/NoiseGenerator.hpp>

class DiscardBoxTest : public ::testing::Test {
 protected:
  DiscardBoxTest()
  {
  }

  ~DiscardBoxTest() override
  {
  }
};

TEST_F (DiscardBoxTest, Constructions)
{
    Tuyau<VirtualBuffer> tuyau;
    Discard<VirtualBuffer> box(tuyau.output());
    NoiseGenerator generator(tuyau.input(), 10000, 1000);

    using namespace std::chrono_literals;
    generator.start();
    std::this_thread::sleep_for(1s);
    generator.stop();
}
