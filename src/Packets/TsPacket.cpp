#include <Packets/TsPacket.hpp>

TsPacket::TsPacket(Buffer* data)
  : data_(data)
{
  if (data_.size() != 188)
    throw 42;
}

unsigned int TsPacket::size()
{
  return 188;
}

bool TsPacket::getError() const
{
  return ((data_[1] & 0b1000'0000) != 0);
}

bool TsPacket::getStart() const
{
  return ((data_[1] & 0b0100'0000) != 0);
}

bool TsPacket::getPriority() const
{
  return ((data_[1] & 0b0010'0000) != 0);
}

uint16_t TsPacket::getPid() const
{
  return ((data_[1] & 0b0001'1111) << 8) + data_[2];
}

TsPacket::Tsc TsPacket::getTsc() const
{
  int tsc = data_[3] & 0b1100'0000 >> 6;
  if (tsc == 0)
    return Tsc::NotScrambled;
  else if (tsc == 1)
    return Tsc::Reserved;
  else if (tsc == 2)
    return Tsc::ScrambledEven;
  else
    return Tsc::ScrambledOdd;
}

TsPacket::Afc TsPacket::getAfc() const
{
  int tsc = (data_[3] & 0b0011'0000) >> 4;
  if (tsc == 0)
    return Afc::Reserved;
  else if (tsc == 1)
    return Afc::NoField;
  else if (tsc == 2)
    return Afc::FieldOnly;
  else
    return Afc::WithPayload;
}

uint8_t TsPacket::getCounter() const
{
  return (data_[3] & 0b0000'1111);
}

uint8_t TsPacket::afLength() const
{
  return data_[4];
}

bool TsPacket::afDiscontinuity() const
{
  return ((data_[5] & 0b1000'0000) >> 7);
}

bool TsPacket::afRandomAccess() const
{
  return ((data_[5] & 0b0100'0000) >> 6);
}

bool TsPacket::afEsPriority() const
{
  return ((data_[5] & 0b0010'0000) >> 5);
}

bool TsPacket::afPcrFlag() const
{
  return ((data_[5] & 0b0001'0000) >> 4);
}

bool TsPacket::afOpcrFlag() const
{
  return ((data_[5] & 0b0000'1000) >> 3);
}

bool TsPacket::afSplicingFlag() const
{
  return ((data_[5] & 0b0000'0100) >> 2);
}

bool TsPacket::afPrivateDataFlag() const
{
  return ((data_[5] & 0b0000'0010) >> 1);
}

bool TsPacket::afExtensionFlag() const
{
  return ((data_[5] & 0b0000'0001) >> 0);
}

uint64_t TsPacket::afPcr() const
{
  // 33 bits base, 6 bits reserved, 9 bits extension
  uint64_t base = 0;
  for (int i = 6; i < 10; i++)
  {
    base <<= 8;
    base += data_[i];
  }
  base <<= 1;
  base += ((data_[10] & 0b1000'0000) >> 7);

  uint64_t extension = 0;
  extension += (data_[10] & 0b0000'0001);
  extension <<= 8;
  extension += data_[11];

  return base * 300 + extension;
}

uint64_t TsPacket::afOpcr() const
{
  int      index = afPcrFlag() ? 12 : 6;
  uint64_t opcr = 0;

  for (int i = index; i < index + 6; i++)
  {
    opcr <<= 8;
    opcr += data_[i];
  }

  return opcr;
}

uint8_t TsPacket::afSpliceCountdown() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;

  return data_[index];
}

uint8_t TsPacket::afPrivateDataLength() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;

  return data_[index];
}

uint8_t TsPacket::privateData(uint8_t privateIndex) const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  index += 1; // We consider there is private data, so no need to call afPrivateDataFlag

  return data_[index + privateIndex];
}

uint8_t TsPacket::afExtensionLength() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();

  return data_[index];
}

bool TsPacket::afLtwFlag() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 1; // Extension Length field

  return ((data_[index] & 0b1000'0000) >> 7);
}

bool TsPacket::afPiecewiseRateFlag() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 1; // Extension Length field

  return ((data_[index] & 0b0100'0000) >> 6);
}

bool TsPacket::afSeamlessSpliceFlag() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 1; // Extension Length field

  return ((data_[index] & 0b0010'0000) >> 5);
}

bool TsPacket::afLtwValid() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 2; // Base Extension

  return ((data_[index] & 0b1000'0000) >> 7);
}

uint16_t TsPacket::afLtwOffset() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 2; // Base Extension

  uint16_t ltw = 0;
  ltw += (data_[index] & 0b0111'1111);
  ltw <<= 8;
  ltw += data_[index + 1];
  return ltw;
}

uint32_t TsPacket::afPiecewiseRate() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 2; // Base Extension
  if (afLtwFlag()) index += 2;

  int piecewise = 0;
  piecewise += (data_[index] & 0b0011'1111);
  piecewise <<= 8;
  piecewise += data_[index + 1];
  piecewise <<= 8;
  piecewise += data_[index + 2];

  return piecewise;
}

uint8_t TsPacket::afSpliceType() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 2; // Base Extension
  if (afLtwFlag()) index += 2;
  if (afPiecewiseRateFlag()) index += 3;

  return ((data_[index] & 0b1111'0000) >> 4);
}

uint64_t TsPacket::afDts() const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 2; // Base Extension
  if (afLtwFlag()) index += 2;
  if (afPiecewiseRateFlag()) index += 3;

  uint64_t dts = 0;

  dts += (data_[index] & 0b0000'1111);
  index++;
  for (uint64_t i = 0; i < 4; i++)
  {
    dts <<= 8;
    dts += data_[index + i];
  }

  return dts;
}

uint8_t TsPacket::payload(uint8_t payloadIndex) const
{
  int index = 6;
  if (afPcrFlag()) index += 6;
  if (afOpcrFlag()) index += 6;
  if (afSplicingFlag()) index += 1;
  if (afPrivateDataFlag()) index += 1 + afPrivateDataLength();
  index += 2; // Base Extension
  if (afLtwFlag()) index += 2;
  if (afPiecewiseRateFlag()) index += 3;
  if (afSeamlessSpliceFlag()) index += 5;

  return data_[index + payloadIndex];
}
