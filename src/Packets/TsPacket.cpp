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

#define TAKE_INTO_ACCOUNT(fieldName, fieldSize, applicability) \
if (applicability)                                             \
{                                                              \
  if (field > (fieldName))                                     \
    index += (fieldSize);                                      \
  else                                                         \
    return index;                                              \
}

int TsPacket::getAdaptationFieldIndex(TsPacket::Field field) const
{
  int index = 0;

  TAKE_INTO_ACCOUNT(Field::AfExtensionFlag, 1, true);
  TAKE_INTO_ACCOUNT(Field::AfPcr, 6, afPcrFlag());
  TAKE_INTO_ACCOUNT(Field::AfOpcr, 6, afOpcrFlag());
  TAKE_INTO_ACCOUNT(Field::AfSpliceCountdown, 1, afSplicingFlag());
  TAKE_INTO_ACCOUNT(Field::AfPrivateDataLength, 1, afPrivateDataFlag());
  TAKE_INTO_ACCOUNT(Field::AfPrivateData, afPrivateDataLength(), afPrivateDataFlag());

  if (afExtensionFlag())
  {
    TAKE_INTO_ACCOUNT(Field::AfExtensionLength, 1, true);
    TAKE_INTO_ACCOUNT(Field::AfSeamlessSpliceFlag, 1, true);
    TAKE_INTO_ACCOUNT(Field::AfLtwOffset, 2, afLtwFlag());
    TAKE_INTO_ACCOUNT(Field::AfPiecewiseRate, 3, afPiecewiseRateFlag());
    TAKE_INTO_ACCOUNT(Field::AfDtsNext, 5, afSeamlessSpliceFlag());
  }

  return index;
}

int TsPacket::getFieldIndex(TsPacket::Field field) const
{
  // The first byte of a Ts Packet is always 'G'
  int index = 1; 

  TAKE_INTO_ACCOUNT(Field::Pid, 2, true);
  TAKE_INTO_ACCOUNT(Field::Counter, 1, true);

  if (getAfc() == Afc::FieldOnly || getAfc() == Afc::WithPayload)
  {
    TAKE_INTO_ACCOUNT(Field::AfLength, 1, true);

    // If we are asking for the payload index,
    // we already have all the necessary information : the AF length.
    if (field == Field::Payload)
      index += afLength();
    else
      index += getAdaptationFieldIndex(field);
  }

  return index;
}

#undef TAKE_INTO_ACCOUNT

bool TsPacket::getError() const
{
  return ((data_[getFieldIndex(Field::ErrorBit)] & 0b1000'0000) >> 7);
}

bool TsPacket::getStart() const
{
  return ((data_[getFieldIndex(Field::StartBit)] & 0b0100'0000) >> 6);
}

bool TsPacket::getPriority() const
{
  return ((data_[getFieldIndex(Field::PriorityBit)] & 0b0010'0000) >> 5);
}

uint16_t TsPacket::getPid() const
{
  uint16_t pid = 0;

  pid += (data_[getFieldIndex(Field::Pid)] & 0b0001'1111);
  pid <<= 8;
  pid += (data_[getFieldIndex(Field::Pid) + 1]);

  return pid;
}

TsPacket::Tsc TsPacket::getTsc() const
{
  int tsc = (data_[getFieldIndex(Field::Tsc)] & 0b1100'0000) >> 6;

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
  int tsc = (data_[getFieldIndex(Field::Afc)] & 0b0011'0000) >> 4;

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
  return (data_[getFieldIndex(Field::Counter)] & 0b0000'1111);
}

uint8_t TsPacket::afLength() const
{
  return data_[getFieldIndex(Field::AfLength)];
}

bool TsPacket::afDiscontinuity() const
{
  return ((data_[getFieldIndex(Field::AfDiscontinuity)] & 0b1000'0000) >> 7);
}

bool TsPacket::afRandomAccess() const
{
  return ((data_[getFieldIndex(Field::AfRandomAccess)] & 0b0100'0000) >> 6);
}

bool TsPacket::afEsPriority() const
{
  return ((data_[getFieldIndex(Field::AfEsPriority)] & 0b0010'0000) >> 5);
}

bool TsPacket::afPcrFlag() const
{
  return ((data_[getFieldIndex(Field::AfPcrFlag)] & 0b0001'0000) >> 4);
}

bool TsPacket::afOpcrFlag() const
{
  return ((data_[getFieldIndex(Field::AfOpcrFlag)] & 0b0000'1000) >> 3);
}

bool TsPacket::afSplicingFlag() const
{
  return ((data_[getFieldIndex(Field::AfSpicingFlag)] & 0b0000'0100) >> 2);
}

bool TsPacket::afPrivateDataFlag() const
{
  return ((data_[getFieldIndex(Field::AfPrivateDataFlag)] & 0b0000'0010) >> 1);
}

bool TsPacket::afExtensionFlag() const
{
  return ((data_[getFieldIndex(Field::AfExtensionFlag)] & 0b0000'0001) >> 0);
}

uint64_t TsPacket::afPcr() const
{
  // 33 bits base, 6 bits reserved, 9 bits extension
  int index = getFieldIndex(Field::AfPcr);
  uint64_t base = 0;

  for (int i = 0; i < 4; i++)
  {
    base <<= 8;
    base += data_[index + i];
  }
  base <<= 1;
  base += ((data_[index + 4] & 0b1000'0000) >> 7);

  uint64_t extension = 0;
  extension += (data_[index + 4] & 0b0000'0001);
  extension <<= 8;
  extension += data_[index + 5];

  return base * 300 + extension;
}

uint64_t TsPacket::afOpcr() const
{
  int      index = getFieldIndex(Field::AfOpcr);
  uint64_t opcr = 0;

  for (int i = 0; i < 6; i++)
  {
    opcr <<= 8;
    opcr += data_[index + i];
  }

  return opcr;
}

uint8_t TsPacket::afSpliceCountdown() const
{
  return data_[getFieldIndex(Field::AfSpliceCountdown)];
}

uint8_t TsPacket::afPrivateDataLength() const
{
  return data_[getFieldIndex(Field::AfPrivateDataLength)];
}

uint8_t TsPacket::privateData(uint8_t privateIndex) const
{
  return data_[getFieldIndex(Field::AfPrivateData) + privateIndex];
}

uint8_t TsPacket::afExtensionLength() const
{
  return data_[getFieldIndex(Field::AfExtensionLength)];
}

bool TsPacket::afLtwFlag() const
{
  return ((data_[getFieldIndex(Field::AfLtwFlag)] & 0b1000'0000) >> 7);
}

bool TsPacket::afPiecewiseRateFlag() const
{
  return ((data_[getFieldIndex(Field::AfPiecewiseRateFlag)] & 0b0100'0000) >> 6);
}

bool TsPacket::afSeamlessSpliceFlag() const
{
  return ((data_[getFieldIndex(Field::AfSeamlessSpliceFlag)] & 0b0010'0000) >> 5);
}

bool TsPacket::afLtwValid() const
{
  return ((data_[getFieldIndex(Field::AfLtwValid)] & 0b1000'0000) >> 7);
}

uint16_t TsPacket::afLtwOffset() const
{
  int index = getFieldIndex(Field::AfLtwOffset);

  uint16_t ltw = 0;
  ltw += (data_[index] & 0b0111'1111);
  ltw <<= 8;
  ltw += data_[index + 1];
  return ltw;
}

uint32_t TsPacket::afPiecewiseRate() const
{
  int index = getFieldIndex(Field::AfPiecewiseRate);

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
  return ((data_[getFieldIndex(Field::AfSpliceType)] & 0b1111'0000) >> 4);
}

uint64_t TsPacket::afDts() const
{
  uint64_t dts = 0;
  int      index = getFieldIndex(Field::AfDtsNext);

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
  return data_[getFieldIndex(Field::Payload) + payloadIndex];
}

VirtualBuffer TsPacket::extractPayload()
{
  return data_.split(getFieldIndex(Field::Payload));
}
