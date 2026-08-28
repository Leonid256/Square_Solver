#include "/opt/homebrew/opt/raylib/include/raylib.h"
#include "function.hpp"
#include <stdio.h>

//---------------------------------------------------------------------------------------------

int square_function_graphic(coefficients coeffs)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float step_camera = 5.0;

    float x_up_arrow_start = screenWidth / 2;
    float x_up_arrow_end_left = screenWidth / 2 - 10;
    float x_up_arrow_end_right = screenWidth / 2 + 10;
    float y_up_arrow_start = 0;
    float y_up_arrow_end = 10;

    InitWindow(screenWidth, screenHeight, "Square_function_graphic");

    SetTargetFPS(60);
    
    Camera2D cameraPosition = {};
    cameraPosition.target = (Vector2){0, 0};
    cameraPosition.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    cameraPosition.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        float x_max = screenWidth * 3;
        float y_max = screenHeight * 3;
    
        int scale_x = 10;
        float scale_y = 0.5;

        DrawLine(x_up_arrow_start, y_up_arrow_start, x_up_arrow_end_left, y_up_arrow_end, LIGHTGRAY);  //oY arrow
        DrawLine(x_up_arrow_start, y_up_arrow_start, x_up_arrow_end_right, y_up_arrow_end, LIGHTGRAY);

        if (cameraPosition.target.x < (screenWidth * 3 - 15.0))
        {
            if (IsKeyDown(KEY_RIGHT)) 
            {
                cameraPosition.target.x += step_camera;

                x_up_arrow_start -= step_camera;
                x_up_arrow_end_right -= step_camera;
                x_up_arrow_end_left -= step_camera;
            }
        }
        if (cameraPosition.target.x > (-screenWidth * 3 + 15.0))
        {
            if (IsKeyDown(KEY_LEFT)) 
            {
                cameraPosition.target.x -= step_camera;

                x_up_arrow_start += step_camera;
                x_up_arrow_end_right += step_camera;
                x_up_arrow_end_left += step_camera;
            }
        }

        if (cameraPosition.target.y < (screenHeight * 3 - 15.0))
        {
            if (IsKeyDown(KEY_DOWN)) cameraPosition.target.y += step_camera;
        }
        if (cameraPosition.target.y > (-screenHeight * 3 + 15.0))
        {
            if (IsKeyDown(KEY_UP)) cameraPosition.target.y -= step_camera;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(cameraPosition);

        DrawLine(-screenWidth * 4, 0, screenWidth * 4, 0, LIGHTGRAY);
        DrawLine(0, -screenHeight * 4, 0, screenHeight * 4, LIGHTGRAY);

        for (double x = - screenWidth * 3; x <= screenWidth * 3; x += 0.005)
        {
            double y = (x * scale_x) * (x * scale_x) * coeffs.a + (x * scale_x) * coeffs.b + coeffs.c;

            DrawPixel(x * scale_x, -y * scale_y, BLACK);
        }

        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}