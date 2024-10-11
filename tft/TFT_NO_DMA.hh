
#include <TFT_SCREEN.h>

bool TFT_SCREEN::dmaBusy() {
  return false;
}

void TFT_SCREEN::dmaWait() {
}

void TFT_SCREEN::dma_sendMDTBuffer16(const uint8_t* buff, const int32_t len)
{
}

bool TFT_SCREEN::initDMA(bool ctrl_cs)
{
  return false;
}

void TFT_SCREEN::deInitDMA()
{
}

