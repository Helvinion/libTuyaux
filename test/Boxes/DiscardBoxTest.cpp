#include "gtest/gtest.h"

#include <chrono>
#include <thread>

#include <Buffers/BasicBuffer.hpp>
#include <Boxes/Discard.hpp>
#include <Boxes/Duplicate.hpp>
#include <Boxes/Forward.hpp>
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

TEST_F (DiscardBoxTest, OneBox)
{
  using namespace std::chrono_literals;

  Tuyau<VirtualBuffer> tuyau;
  Discard<VirtualBuffer> box(tuyau.output());
  NoiseGenerator generator(tuyau.input(), 10000, 1000);

  generator.start();
  std::this_thread::sleep_for(1s);
  generator.stop();
}

TEST_F (DiscardBoxTest, SeveralBoxes)
{
  using namespace std::chrono_literals;

  Tuyau<VirtualBuffer> tuyau_1; // Generator -> Duplicate
  Tuyau<VirtualBuffer> tuyau_2; // Duplicate -> Forward
  Tuyau<VirtualBuffer> tuyau_3; // Duplicate -> Discard_1
  Tuyau<VirtualBuffer> tuyau_4; // Forward   -> Discard_2

  NoiseGenerator generator(tuyau_1.input(), 1'000'000, 1000);
  Duplicate<VirtualBuffer> duplicate(tuyau_1.output(), tuyau_2.input(), tuyau_3.input());
  Forward<VirtualBuffer> forward(tuyau_2.output(), tuyau_4.input());
  Discard<VirtualBuffer> discard_1(tuyau_3.output());
  Discard<VirtualBuffer> discard_2(tuyau_4.output());

  generator.start();
  std::this_thread::sleep_for(1s);
  generator.stop();
}
