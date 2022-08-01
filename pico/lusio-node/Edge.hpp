#include "hardware/pio.h"

class Edge {
public:
  explicit Edge(uint edgeNum, PIO &pioTx, uint txPin, PIO &pioRx, uint rxPin);

  bool init();

private:
  uint edgeNum_;

  // BurstGenerator &burstGen_;

  PIO &pioTx_;
  uint txPin_;
  PIO &pioRx_;
  uint rxPin_;

  // uint carrierControlOffset_;
  uint txOffset_;
  uint rxOffset_;

  int txSM_;
  int rxSM_;
};