#include "raylib.h"
#include "image.h"  // Include the generated header file
#include <stdio.h>

#define LED_SIZE 1  // Size of each LED (adjust as needed)
#define SCREEN_WIDTH  (64 * LED_SIZE)  // Adjust based on image size
#define SCREEN_HEIGHT (64 * LED_SIZE)  // Adjust based on image size

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LED Matrix Simulation");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw each non-black pixel as an LED
        for (int i = 0; i < sizeof(image_data) / sizeof(image_data[0]); i++) {
            int x = image_data[i].x * LED_SIZE;
            int y = image_data[i].y * LED_SIZE;
            Color color = (Color){ image_data[i].r, image_data[i].g, image_data[i].b, 255 };
            // printf("printing pixel at position %d, %d", x,y);

            // Draw as a square (or use DrawCircle for a round LED effect)
            DrawRectangle(x, y, LED_SIZE, LED_SIZE, color);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

