#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "image.h"

MatrixPanel_I2S_DMA *dma_display = nullptr;
const char *TAG = "Matrix";

extern "C" void app_main()
{
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
    if (!dma_display)
    {
        ESP_LOGE(TAG, "Failed to initialize display!");
        return;
    }

    dma_display->begin();
    dma_display->setBrightness8(255); // 0-255
                                      //
    // Debug the configuration
    ESP_LOGI(TAG, "Matrix configuration:");
    ESP_LOGI(TAG, "Width: %d, Height: %d", dma_display->width(), dma_display->height());

    // Clear the screen
    dma_display->fillScreenRGB888(0, 0, 0);

    ESP_LOGI(TAG, "Iterating through non black pixels...");

    // Clear the screen before lighting up with the current image
    dma_display->fillScreenRGB888(0, 0, 0);

    // Iterate over all non black pixels in the image
    for (int y = 0; y < dma_display->height(); y++)
    {

        // Print non-black pixels
        for (int i = 0; i < sizeof(image_data) / sizeof(image_data[0]); i++)
        {
         uint16_t x = image_data[i].x;
         uint16_t  y = image_data[i].y;
            if (x >= dma_display->width() || y >= dma_display->height())
            {
                ESP_LOGI(TAG, "pixel number %d  x=%d y=%d out of bound", i, image_data[i].x, image_data[i].y);
                break; // unrecoverable error
            }
            dma_display->drawPixel(x, y, dma_display->color565(image_data[i].r, image_data[i].g, image_data[i].b));

            // ESP_LOGI(TAG, "Lighting up line %d", y);

            // Wait for a short duration to create a visible effect
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        ESP_LOGI(TAG, "Finished iterating through all lines.");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
