#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "images.h"
#include <cstdint>

MatrixPanel_I2S_DMA *dma_display = nullptr;
const char *TAG = "Matrix";

uint8_t image_idx = 0; // ideally we load different imagedata based on index

bool isBooting = true;
extern "C" void app_main() {
  // Initialize the configuration for a 64x64 matrix
  HUB75_I2S_CFG mxconfig(64, 64, 1);

  // Pin configuration (adjust these to match your wiring)
  mxconfig.gpio.lat = GPIO_NUM_4;  // LATCH
  mxconfig.gpio.oe = GPIO_NUM_15;  // OE (Output Enable)
  mxconfig.gpio.a = GPIO_NUM_23;   // A
  mxconfig.gpio.b = GPIO_NUM_22;   // B
  mxconfig.gpio.c = GPIO_NUM_5;    // C
  mxconfig.gpio.d = GPIO_NUM_17;   // D
  mxconfig.gpio.clk = GPIO_NUM_16; // CLK
  mxconfig.gpio.r1 = GPIO_NUM_25;  // R1
  mxconfig.gpio.g1 = GPIO_NUM_26;  // G1
  mxconfig.gpio.b1 = GPIO_NUM_27;  // B1
  mxconfig.gpio.e = GPIO_NUM_32;   // E

  // Create the matrix display object
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  if (!dma_display) {
    ESP_LOGE(TAG, "Failed to initialize display!");
    return;
  }

  dma_display->begin();
  dma_display->setBrightness8(255); // 0-255
                                    //
  // Debug the configuration
  ESP_LOGI(TAG, "Matrix configuration:");
  ESP_LOGI(TAG, "Width: %d, Height: %d", dma_display->width(),
           dma_display->height());

  /*========== MAIN LOOP =========*/
  while (1) {
    
    // Clear the screen before lighting up with the current image
    dma_display->fillScreenRGB888(0, 0, 0);
    
    ESP_LOGI(TAG, "Iterating through non black pixels...");

      Image_t currentImage = images[image_idx];

      // Print non-black pixels
      for (int i = 0; i < currentImage.size; i++) {
        uint16_t x = currentImage.data[i].x;
        uint16_t y = currentImage.data[i].y;
        if (x >= dma_display->width() || y >= dma_display->height()) {
          ESP_LOGE(TAG, "pixel number %d  x=%d y=%d out of bound", i,
                   currentImage.data[i].x, currentImage.data[i].y);
          break; // unrecoverable error
        }
        dma_display->drawPixel(x, y,
                               dma_display->color565(currentImage.data[i].r,
                                                     currentImage.data[i].g,
                                                     currentImage.data[i].b));

        if (isBooting) { // only display the boot animation for the first time
          vTaskDelay(25 / portTICK_PERIOD_MS);
        }

        // NEED TO EXPLORE THIS API
        /*  drawIcon draws a C style bitmap.
        //  Example 10x5px bitmap of a yellow sun
        //
          int half_sun [50] = {
              0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000,
        0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0,
        0xffe0, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0,
        0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0xffe0,
        0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
          };

          MatrixPanel_I2S_DMA matrix;

          matrix.drawIcon (half_sun, 0,0,10,5);
        */
        // ESP_LOGI(TAG, "Lighting up line %d", y);

        // Wait for a short duration to create a visible effect
        /*vTaskDelay(100 / portTICK_PERIOD_MS);*/
      }

      ESP_LOGI(TAG, "Finished iterating through all lines.");
      if (isBooting) { // only display the boot animation for the first time
      isBooting = false;
      }
      image_idx = (image_idx + 1) % num_images;

      /*vTaskDelay(2000 / portTICK_PERIOD_MS);*/
      vTaskDelay(2000 / portTICK_PERIOD_MS); // set a long period of idle, after
                                            // which we can skip to image2?
  }
}
