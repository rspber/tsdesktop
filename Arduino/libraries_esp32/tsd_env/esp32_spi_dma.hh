/*
  esp32_spi_dma

  Copyright (c) 2024, rspber (https://github.com/rspber)

  source:

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.c

  Bodmer's license.txt in file /licenses/Bodmer_license.txt
*/

#include <driver/spi_master.h>
#include <hal/gpio_ll.h>
#include <TFT_SCREEN.h>

  // DMA SPI handle
  spi_device_handle_t dmaHAL;
  #ifdef CONFIG_IDF_TARGET_ESP32
    #define DMA_CHANNEL 1
    #ifdef USE_HSPI_PORT
      spi_host_device_t spi_host = HSPI_HOST;
    #elif defined(USE_FSPI_PORT)
      spi_host_device_t spi_host = SPI_HOST;
    #else // use VSPI port
      spi_host_device_t spi_host = VSPI_HOST;
    #endif
  #else
    #ifdef USE_HSPI_PORT
      #define DMA_CHANNEL SPI_DMA_CH_AUTO
      spi_host_device_t spi_host = (spi_host_device_t) SPI3_HOST; // Draws once then freezes
    #else // use FSPI port
      #define DMA_CHANNEL SPI_DMA_CH_AUTO
      spi_host_device_t spi_host = (spi_host_device_t) SPI2_HOST; // Draws once then freezes
    #endif
  #endif

  uint8_t  spiBusyCheck = 0;      // Number of ESP32 transfer buffers to check

bool TFT_SCREEN::dmaBusy()
{
  if (!dma_enabled) { return false; }
  if (!spiBusyCheck) { return false; }

  for (int i = spiBusyCheck; --i >= 0; ) {
    spi_transaction_t *rtrans;
    if (spi_device_get_trans_result(dmaHAL, &rtrans, 0) == ESP_OK) {
      spiBusyCheck--;
    }
  }

  return spiBusyCheck != 0;
}

void TFT_SCREEN::dmaWait()
{
  if (!dma_enabled) { return; }
  if (!spiBusyCheck) { return; }

  for (int i = spiBusyCheck; --i >= 0; ) {
    spi_transaction_t *rtrans;
    assert( spi_device_get_trans_result(dmaHAL, &rtrans, portMAX_DELAY) == ESP_OK);
  }
  spiBusyCheck = 0;
}

void TFT_SCREEN::dma_sendMDTBuffer16(const uint8_t* buff, const int32_t len)
{
 if (!dma_enabled) { return; }
 if (len == 0) { return; }

  dmaWait();
/*
  if(_swapBytes) {
    for (uint32_t i = 0; i < len; i++) (image[i] = image[i] << 8 | image[i] >> 8);
  }
*/
  static spi_transaction_t trans;
  memset(&trans, 0, sizeof(spi_transaction_t));
  trans.user = (void *)1;
  trans.tx_buffer = buff;  //finally send the line data
  trans.length = len * 16;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  assert(spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY) == ESP_OK);

  spiBusyCheck++;
}

extern "C" void dc_callback();

void IRAM_ATTR dc_callback(spi_transaction_t *spi_tx)
{
  if ((bool)spi_tx->user) {SPI_DC_D;}
  else {SPI_DC_C;}
}

extern "C" void dma_end_callback();

void IRAM_ATTR dma_end_callback(spi_transaction_t *spi_tx)
{
  WRITE_PERI_REG(SPI_DMA_CONF_REG(spi_host), 0);
}

bool TFT_SCREEN::initDMA(bool ctrl_cs)
{
  if (dma_enabled) return false;

  spi_bus_config_t buscfg = {
    .mosi_io_num = TFT_SPI_MOSI,
    .miso_io_num = TFT_SPI_MISO,
    .sclk_io_num = TFT_SPI_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    #ifdef xCONFIG_IDF_TARGET_ESP32S2
      .data4_io_num = -1,
      .data5_io_num = -1,
      .data6_io_num = -1,
      .data7_io_num = -1,
    #endif
    .max_transfer_sz = TFT_WIDTH * TFT_HEIGHT * 2 + 8, // TFT screen size
    .flags = 0,
    .intr_flags = 0
  };

  int8_t pin = -1;
  if (ctrl_cs) pin = TFT_SPI_CS;

  spi_device_interface_config_t devcfg = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = TFT_SPI_MODE,
    .duty_cycle_pos = 0,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = TFT_SPI_WRITE_SPEED,
    .input_delay_ns = 0,
    .spics_io_num = pin,
    .flags = SPI_DEVICE_NO_DUMMY, //0,
    .queue_size = 1,
    .pre_cb = 0, //dc_callback, //Callback to handle D/C line
    #ifdef CONFIG_IDF_TARGET_ESP32
      .post_cb = 0
    #else
      .post_cb = dma_end_callback
    #endif
  };
  esp_err_t ret = spi_bus_initialize(spi_host, &buscfg, DMA_CHANNEL);
  ESP_ERROR_CHECK(ret);
  ret = spi_bus_add_device(spi_host, &devcfg, &dmaHAL);
  ESP_ERROR_CHECK(ret);

  dma_enabled = true;
  spiBusyCheck = 0;
  return true;
}

void TFT_SCREEN::deInitDMA()
{
  if (!dma_enabled) return;
  spi_bus_remove_device(dmaHAL);
  spi_bus_free(spi_host);
  dma_enabled = false;
}
