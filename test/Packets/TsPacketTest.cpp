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

TEST_F (TsPacketTest, AdaptationFieldPacketDecode_1)
{
    // We create a simple TS packet with no adaptation field
    // and no fancy parameter
    BasicBuffer buffer(188);
    buffer[0] = 'G';
    buffer[1] = 0;
    buffer[2] = 200; // PID = 200
    buffer[3] = 0b0011'0011; // Adaptation field and payload, CNT = 3;
    buffer[4] = 10; // Ten bytes of AF after this one.
    buffer[5] = 0; // All flags diabled
    for (int i = 6; i < 15; i++)
        buffer[i] = 0xff;

    for (int i = 15; i < 188; i++)
        buffer[i] = 0;
    
    TsPacket packet(new BasicBuffer(std::move(buffer)));

    EXPECT_EQ(packet.getPid(), 200);
    EXPECT_EQ(packet.getAfc(), TsPacket::Afc::WithPayload);
    EXPECT_EQ(packet.getCounter(), 3);
    EXPECT_EQ(packet.payload(0), 0);
}
