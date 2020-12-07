#include "gtest/gtest.h"

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

    generator.start();
    generator.stop();
}