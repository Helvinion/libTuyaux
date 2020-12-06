#ifndef INCLUDE_PACKETS_TSPACKETS_HPP
# define INCLUDE_PACKETS_TSPACKETS_HPP

#include <Buffers/VirtualBuffer.hpp>

#include <cstdint>

class TsPacket
{
public:
  TsPacket(Buffer* data);

  unsigned int size();

  enum class Tsc
  {
    NotScrambled  = 0,
    Reserved      = 1,
    ScrambledEven = 2,
    ScrambledOdd  = 3
  };

  enum class Afc
  {
    Reserved    = 0,
    NoField     = 1,
    FieldOnly   = 2,
    WithPayload = 3
  };

  // Header
  bool     getError() const;
  bool     getStart() const;
  bool     getPriority() const;
  uint16_t getPid() const;
  Tsc      getTsc() const;
  Afc      getAfc() const;
  uint8_t  getCounter() const;

  // Adaptation Field
  uint8_t  afLength() const;
  bool     afDiscontinuity() const;
  bool     afRandomAccess() const;
  bool     afEsPriority() const;
  bool     afPcrFlag() const;
  bool     afOpcrFlag() const;
  bool     afSplicingFlag() const;
  bool     afPrivateDataFlag() const;
  bool     afExtensionFlag() const;
  uint64_t afPcr() const;
  uint64_t afOpcr() const;
  uint8_t  afSpliceCountdown() const;
  uint8_t  afPrivateDataLength() const;
  uint8_t  privateData(uint8_t index) const;
  uint8_t  afExtensionLength() const;
  bool     afLtwFlag() const;
  bool     afPiecewiseRateFlag() const;
  bool     afSeamlessSpliceFlag() const;
  bool     afLtwValid() const;
  uint16_t afLtwOffset() const;
  uint32_t afPiecewiseRate() const;
  uint8_t  afSpliceType() const;
  uint64_t afDts() const;

  uint8_t  payload(uint8_t index) const;

private:
  VirtualBuffer data_;
};

#endif // INCLUDE_PACKETS_TSPACKETS_HPP
