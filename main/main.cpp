#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "images.h"
#include "states.h"
#include <cstdint>

MatrixPanel_I2S_DMA *dma_display = nullptr;
const char *TAG = "Matrix";
const int IDLE_TIME_MS = 200;
const int SLIDESHOW_INTERVAL_MS = 2000;

uint8_t image_idx = 0; // ideally we load different imagedata based on index

states_idx_t state_machine_idx = STATE_SLIDESHOW;  // perform different visualization based on state machine

bool isBooting = true;

int poll_button(uint8_t gpio_num){
    int buttonState = digitalRead(gpio_num);
    ESP_LOGI(TAG, "button state is: %d", buttonState);
    return buttonState;
}
// potrei usare reference invece che un puntatore dato che siamo in C++ ma per consistenza uso un puntatore
void handle_button_press(int buttonState, states_idx_t *state){
      if (*state == STATE_IMAGE_MAX) {
        *state = STATE_SLIDESHOW;  // Wrap to beginning
        image_idx = 0; //reset the global
    } else {
        // Clear the screen before lighting up with the current image
        dma_display->fillScreenRGB888(0, 0, 0);
        *state = (states_idx_t)((int)*state + 1);
        ESP_LOGI("handle button press", "new state is: %d", *state);
    }
}

void printImage(Image_t image){
// Print non-black pixels
      for (int i = 0; i < image.size; i++) {
        uint16_t x = image.data[i].x;
        uint16_t y = image.data[i].y;
        if (x >= dma_display->width() || y >= dma_display->height()) {
          ESP_LOGE(TAG, "pixel number %d  x=%d y=%d out of bound", i,
                   image.data[i].x, image.data[i].y);
          break; // unrecoverable error
        }
        dma_display->drawPixel(x, y,
                               dma_display->color565(image.data[i].r,
                                                     image.data[i].g,
                                                     image.data[i].b));

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
}


void displaySlideshow(){
  static int64_t  last_change_time = 0;
  int64_t now = esp_timer_get_time() / 1000; // Convert microseconds to ms
  if(now - last_change_time >= SLIDESHOW_INTERVAL_MS){
    // Clear the screen before lighting up with the current image
    dma_display->fillScreenRGB888(0, 0, 0);
    Image_t currentImage = images[image_idx];
    printImage(currentImage);
    image_idx = (image_idx + 1) % num_images;
    last_change_time = now;
  }
  else return; // no-op if not enough time has passed
}

void displayImage(bool stateChanged){
    if (state_machine_idx >= STATE_IMAGE_0 && state_machine_idx <= STATE_IMAGE_MAX) {
      if (!stateChanged){
        return; // no-op if button was not pressed
      }
        Image_t currentImage = images[state_machine_idx];
        printImage(currentImage);  // state is 0-8, perfect for array
    } else if (state_machine_idx == STATE_SLIDESHOW) {
        ESP_LOGE("displayImage", "state machine in unexpected state: STATE_SLIDESHOW");
    }
    else
    {
      ESP_LOGE("displayImage", "state machine in impossible state: %d", state_machine_idx);
    }
    
}

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

  const uint8_t GPIO_BUTTON = GPIO_NUM_19;
  pinMode(GPIO_BUTTON, INPUT_PULLUP);  // use internal resistor
  bool stateChanged; // wether the state machine index has changed or not (reacts to button press)


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
    stateChanged = false;
    int buttonstate = poll_button(GPIO_BUTTON);
    if (buttonstate == BUTTON_PRESSED)
    {
      ESP_LOGI("button pressed", "state was: %d", state_machine_idx);
      handle_button_press(buttonstate, &state_machine_idx);
      stateChanged = true;
    }
    
    ESP_LOGI(TAG, "Iterating through non black pixels...");
    switch (state_machine_idx)
    {
    case STATE_SLIDESHOW:
      displaySlideshow();
      break;
    default:
      displayImage(stateChanged);
      break;
    }
    vTaskDelay(IDLE_TIME_MS / portTICK_PERIOD_MS); // set a small period of idle
  }
}
