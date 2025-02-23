#include "images.h" // Include the generated header file
#include "raylib.h"
/*#include <cstddef>*/
/*#include <cstdint>*/
#include <stdio.h>

#define LED_SIZE 10                   // Size of each LED (adjust as needed)
#define SCREEN_WIDTH (64 * LED_SIZE)  // Adjust based on image size
#define SCREEN_HEIGHT (64 * LED_SIZE) // Adjust based on image size

typedef struct Timer {
  double startTime; // Start time (seconds)
  double lifeTime;  // Lifetime (seconds)
} Timer;

void StartTimer(Timer *timer, double lifetime) {
  timer->startTime = GetTime();
  timer->lifeTime = lifetime;
}

void UpdateTimer(Timer *timer) {
  if (timer != NULL) {
    timer->lifeTime -= GetFrameTime();
  }
}

bool TimerDone(Timer timer) {
  return GetTime() - timer.startTime >= timer.lifeTime;
}

double GetElapsed(Timer timer) { return GetTime() - timer.startTime; }

Timer animationT = {0};
/*float animationSpeed = 0.05f; // 50ms*/
float animationSpeed = 0.025; // 25ms
int currentPixel = 0;
bool isFirstBoot = true;
uint64_t fully_drawn_count = 0;

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LED Matrix Simulation");
  SetTargetFPS(60);

  StartTimer(&animationT, animationSpeed);
  ClearBackground(BLACK);

  while (!WindowShouldClose()) {
    BeginDrawing();

    UpdateTimer(&animationT);
    Image_t currentImage = images[fully_drawn_count];


    if (currentPixel < currentImage.size) {
      if (isFirstBoot) {
        // During first boot, draw pixels with delay
        if (TimerDone(animationT)) {
          int x = currentImage.data[currentPixel].x * LED_SIZE;
          int y = currentImage.data[currentPixel].y * LED_SIZE;
          Color color =
              (Color){currentImage.data[currentPixel].r, currentImage.data[currentPixel].g,
                      currentImage.data[currentPixel].b, 255};
          DrawRectangle(x, y, LED_SIZE, LED_SIZE, color);
          currentPixel++;
          StartTimer(&animationT, animationSpeed);
        }
      } else {
        // After first boot, draw all pixels instantly
        for (; currentPixel < currentImage.size; currentPixel++) {
          int x = currentImage.data[currentPixel].x * LED_SIZE;
          int y = currentImage.data[currentPixel].y * LED_SIZE;
          Color color =
              (Color){currentImage.data[currentPixel].r, currentImage.data[currentPixel].g,
                      currentImage.data[currentPixel].b, 255};
          DrawRectangle(x, y, LED_SIZE, LED_SIZE, color);
        }
      }
    } else {
      // All pixels drawn, start long delay
      if (TimerDone(animationT)) {
        // Reset for next iteration
        currentPixel = 0;
        isFirstBoot = false;
        ClearBackground(BLACK);
        /*StartTimer(&animationT,*/
        /*           20.0f); // 20 second delay between iterations*/
        StartTimer(&animationT,
                   2.0f); // 2 second delay between iterations
    fully_drawn_count = (fully_drawn_count + 1 ) % num_images;
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
