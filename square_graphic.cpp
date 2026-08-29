#include "/opt/homebrew/opt/raylib/include/raylib.h"
#include "function.hpp"
#include <stdio.h>
#include "rlgl.h"
//---------------------------------------------------------------------------------------------

int square_function_graphic(coefficients coeffs, double x1, double x2, enum_decisions quantity)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float step_camera = 5.0;

    float x_up_arrow_start = screenWidth / 2;           //up arrow (oY)
    float x_up_arrow_end_left = screenWidth / 2 - 10;
    float x_up_arrow_end_right = screenWidth / 2 + 10;
    float y_up_arrow_start = 0;
    float y_up_arrow_end = 10;

    float x_right_arrow_start = screenWidth;            //right arrow (oX)
    float x_right_arrow_end = screenWidth - 10;
    float y_right_arrow_start = screenHeight / 2;
    float y_right_arrow_end_up = screenHeight / 2 - 10;
    float y_right_arrow_end_down = screenHeight / 2 + 10;

    float zoom_camera = 1.0;

    InitWindow(screenWidth, screenHeight, "Square_function_graphic");

    SetTargetFPS(90);
    

    Camera2D cameraPosition = {};
    cameraPosition.target = (Vector2){0, 0};
    cameraPosition.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    cameraPosition.zoom = zoom_camera;

    while (!WindowShouldClose())
    {
        cameraPosition.zoom = zoom_camera;

        float x_max = screenWidth * 3;
        float y_max = screenHeight * 3;
    
        float scale_x = 5;
        float scale_y = 5;

        /*
        //стрелки для осей
        DrawLine(x_up_arrow_start, y_up_arrow_start, x_up_arrow_end_left, y_up_arrow_end, DARKGRAY);  //oY arrow
        DrawLine(x_up_arrow_start, y_up_arrow_start, x_up_arrow_end_right, y_up_arrow_end, DARKGRAY);

        DrawLine(x_right_arrow_start, y_right_arrow_start, x_right_arrow_end, y_right_arrow_end_up, DARKGRAY);  //oX arrow
        DrawLine(x_right_arrow_start, y_right_arrow_start, x_right_arrow_end, y_right_arrow_end_down, DARKGRAY);
        */

        //сдвиг области видимости
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
            if (IsKeyDown(KEY_DOWN)) 
            {
                cameraPosition.target.y += step_camera;

                y_right_arrow_start -= step_camera;
                y_right_arrow_end_up -= step_camera;
                y_right_arrow_end_down -= step_camera;
            }

        }
        if (cameraPosition.target.y > (-screenHeight * 3 + 15.0))
        {
            if (IsKeyDown(KEY_UP)) 
            {
                cameraPosition.target.y -= step_camera;

                y_right_arrow_start += step_camera;
                y_right_arrow_end_up += step_camera;
                y_right_arrow_end_down += step_camera;
            }
        }

        //изменение зума
        if (IsKeyDown(KEY_EQUAL))
            zoom_camera += 0.25;
        if (IsKeyDown(KEY_MINUS))
            zoom_camera -= 0.25;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(cameraPosition);

        //отрисовка сетки графика
        rlPushMatrix();
        rlTranslatef(0, 450, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(1000, 5);
        rlPopMatrix();
        DrawGrid(1000, 10);  

        //Легенда на графике + корни на оси
        switch (quantity)
        {
            case ONE_ROOT:
                DrawText("The equation has 1 solution", 10, 10, 20, BLACK);
                DrawText(TextFormat("x1 = x2 = %lg", x1), 10, 30, 20, BLACK);

                DrawCircle(x1 * scale_x, 0, 2, RED);
                break;
            case TWO_ROOTS:
                DrawText("The equation has two solutions", 10, 10, 20, BLACK);
                DrawText(TextFormat("x1 =  %lg", x1), 10, 30, 20, BLACK);
                DrawText(TextFormat("x2 =  %lg", x2), 10, 50, 20, BLACK);

                DrawCircle(x1 * scale_x, 0, 2, RED);
                DrawCircle(x2 * scale_x, 0, 2, RED);

                break;
            default:
                break;
        }

        //отрисовка координатных осей
        DrawLine(-screenWidth * 4, 0, screenWidth * 4, 0, DARKGRAY);
        DrawLine(0, -screenHeight * 4, 0, screenHeight * 4, DARKGRAY);

        //отрисовка графика
        for (double x = - screenWidth * 3; x <= screenWidth * 3; x += 0.002)
        {
            double y = (x) * (x) * coeffs.a + (x) * coeffs.b + coeffs.c;

            DrawPixel(x * scale_x, -y * scale_y, SKYBLUE);
        }
        
        //отрисовка разметки оси оХ
        for (double x = - screenWidth * 3; x <= screenWidth * 3; x += 5)
        {
            DrawCircle(x, 0, 1, DARKGRAY);
        }

        //отрисовка разметки оси oY
        for (double y = - screenWidth * 3; y <= screenWidth * 3; y += 5)
        {
            DrawCircle(0, y, 1, DARKGRAY);
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}