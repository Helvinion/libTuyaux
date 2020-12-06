#include "gtest/gtest.h"

#include <Buffers/BasicBuffer.hpp>
#include <Packets/TsPacket.hpp>

class TsPacketTest : public ::testing::Test {
protected:
  TsPacketTest()
  {
  }

  ~TsPacketTest() override
  {
  }
};

TEST_F (TsPacketTest, SimplePacketDecode)
{
    // We create a simple TS packet with no adaptation field
    // and no fancy parameter
    BasicBuffer buffer(188);
    buffer[0] = 'G';
    buffer[1] = 0;
    buffer[2] = '*'; // PID = 42
    buffer[3] = 0b0001'0111; // No Adaptation field, CNT = 7;
    for (int i = 4; i < 188; i++)
        buffer[i] = 0;
    
    TsPacket packet(new BasicBuffer(std::move(buffer)));

    EXPECT_EQ(packet.getPid(), 42);
    EXPECT_EQ(packet.getAfc(), TsPacket::Afc::NoField);
    EXPECT_EQ(packet.getCounter(), 7);
}