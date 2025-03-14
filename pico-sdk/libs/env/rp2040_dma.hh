/*
  rp2040_dma

  Copyright (c) 2024, rspber (https://github.com/rspber)

  source:

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_RP2040.c

  Bodmer's license.txt in file /licenses/Bodmer_license.txt
*/

// ---- the DMA --------------------------------------------------------------

#include "hardware/dma.h"
#include <TFT_SCREEN.h>

  bool                dma_enabled;
  int32_t             dma_tx_channel;
  dma_channel_config  dma_tx_config;

bool TFT_SCREEN::dmaBusy() {
  if (!dma_enabled) return false;
  if (dma_channel_is_busy(dma_tx_channel)) return true;
  DMA_END_WRITTING();
  return false;
}

void TFT_SCREEN::dmaWait() {
  while (dma_channel_is_busy(dma_tx_channel));
  DMA_END_WRITTING();
}

void TFT_SCREEN::dma_sendMDTBuffer16(const uint8_t* buff, const int32_t len)
{
  if (!dma_enabled) return;
  if (len <= 0) return;
  dmaWait(); // In case we did not wait earlier
  SET_BUS_WRITE_16();
  channel_config_set_bswap(&dma_tx_config, true); // !_swapBytes
  dma_channel_configure(dma_tx_channel, &dma_tx_config, DMA_WRITE_ADDR(), (uint16_t*)buff, len, true);
}

bool TFT_SCREEN::initDMA(bool ctrl_cs)
{
  ctrl_cs = ctrl_cs;  // compiler warning value not used

  if (dma_enabled) return false;

  dma_tx_channel = dma_claim_unused_channel(false);

  if (dma_tx_channel < 0) return false;

  dma_tx_config = dma_channel_get_default_config(dma_tx_channel);

  channel_config_set_transfer_data_size(&dma_tx_config, DMA_SIZE_16);
  channel_config_set_dreq(&dma_tx_config, DMA_DREQ());

  dma_enabled = true;
  return true;
}

void TFT_SCREEN::deInitDMA()
{
  if (!dma_enabled) return;
  dma_channel_unclaim(dma_tx_channel);
  dma_enabled = false;
}

