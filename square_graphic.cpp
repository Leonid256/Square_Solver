#include "/opt/homebrew/opt/raylib/include/raylib.h"
#include "function.hpp"
#include <stdio.h>
#include "rlgl.h"
//---------------------------------------------------------------------------------------------
/**
    \brief Постройка графика
    \details Данная функция строит график уравнения в случае 1 или 2 корней. На графике возможно передвижение с помощью стрелок, 
    а также изменение зума клавишами + и - . И то, и то ограничено в пределах с помощью констант
    \param [in] coeffs коэффициенты уравнения
    \param [in] x1, x2 корни уравнения
    \note В случае одного решения у уравнения, корень x2 = NAN
    \note При отсутствии ограничения на зум, при переходе его в отрицательную область, график переворачивается. Поэтому ограничение обязательно
    \param [in] quantity количество корней
    \return 0 в случае успешного завершения
*/
int square_function_graphic(coefficients coeffs, double x1, double x2, enum_decisions quantity)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float step_camera = 5.0;
    const float zoom_max = 3.0;
    const float zoom_min = 0.5;

    float zoom_camera = 1.0;
    float x_top = -coeffs.b / (2 * coeffs.a);
    float y_top = x_top * x_top * coeffs.a + x_top * coeffs.b + coeffs.c;

    InitWindow(screenWidth, screenHeight, "Square_function_graphic");

    SetTargetFPS(60);
    

    Camera2D cameraPosition = {};
    cameraPosition.target = (Vector2){0, 0};
    cameraPosition.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    cameraPosition.zoom = zoom_camera;

    while (!WindowShouldClose())
    {
        cameraPosition.zoom = zoom_camera;

        float x_max = screenWidth * 2;
        float y_max = screenHeight * 2.5;
    
        float scale_x = 5;
        float scale_y = 5;

        //сдвиг области видимости
        if (cameraPosition.target.x < (x_max - 15.0))
        {
            if (IsKeyDown(KEY_RIGHT)) 
                cameraPosition.target.x += step_camera;
        }
        if (cameraPosition.target.x > (-x_max + 15.0))
        {
            if (IsKeyDown(KEY_LEFT)) 
                cameraPosition.target.x -= step_camera;
        }

        if (cameraPosition.target.y < (y_max - 15.0))
        {
            if (IsKeyDown(KEY_DOWN)) 
                cameraPosition.target.y += step_camera;
        }
        if (cameraPosition.target.y > (-y_max + 15.0))
        {
            if (IsKeyDown(KEY_UP)) 
                cameraPosition.target.y -= step_camera;
        }

        if (IsKeyDown(KEY_ESCAPE))
            CloseWindow();

        //изменение зума
        if (IsKeyDown(KEY_EQUAL) && (zoom_camera < zoom_max))
            zoom_camera += 0.25;
        if (IsKeyDown(KEY_MINUS) && (zoom_camera > zoom_min))
            zoom_camera -= 0.25;

        BeginDrawing();
        ClearBackground(RAYWHITE);
//----------------------------------
        BeginMode2D(cameraPosition);

        //отрисовка сетки графика
        rlPushMatrix();
        rlTranslatef(0, 2000, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(4000, 5);
        rlPopMatrix();
        DrawGrid(2000, 10);  

        //отрисовка координатных осей
        DrawLine(-x_max * 2, 0, x_max * 2, 0, DARKGRAY);
        DrawLine(0, -y_max * 2, 0, y_max * 2, DARKGRAY);

        //отрисовка нуля
        if (y_top < 0)  
        {
            if (x_top < 0)
                DrawText("0", 3, -10, 5, BLACK);
            else if (x_top > 0)
                DrawText("0", 3, -10, 5, BLACK);
            else
                ;
        }
        else if (y_top > 0)
        {
            if (x_top < 0)
                DrawText("0", -8, -10, 5, BLACK);
            else if (x_top > 0)
                DrawText("0", -8, 3, 5, BLACK);
            else
                ;
        }
        else
            ;

        //отрисовка графика
        for (double x = - x_max; x <= x_max; x += 0.002)
        {
            double y = (x) * (x) * coeffs.a + (x) * coeffs.b + coeffs.c;

            DrawPixel(x * scale_x, -y * scale_y, SKYBLUE);
        }
        
        //отрисовка разметки оси оХ
        for (double x = - x_max; x <= x_max; x += 5)
        {
            DrawCircle(x, 0, 1, DARKGRAY);
        }

        //отрисовка разметки оси oY
        for (double y = - y_max; y <= y_max; y += 5)
        {
            DrawCircle(0, y, 1, DARKGRAY);
        }

        //корни на оси
        DrawText(TextFormat("%lg", x_top), x_top * scale_x, 1, 2, BLACK);
        DrawText(TextFormat("%lg", y_top), 1, -y_top * scale_y, 2, BLACK);

        //отметка корней на осях
        switch (quantity)
        {
            case DECISION_ONE_ROOT:
                DrawCircle(x1 * scale_x, 0, 3, PINK);

                break;
            case DECISION_TWO_ROOTS:
                DrawCircle(x1 * scale_x, 0, 3, PINK);
                DrawCircle(x2 * scale_x, 0, 3, PINK);

                break;
            default:
                break;
        }

        //пунктиры к вершине
        Vector2 top_on_x = {x_top * scale_x, 0.0};
        Vector2 top_graphic = {x_top * scale_x, -y_top * scale_y};
        Vector2 top_on_y = {0, -y_top * scale_y};

        DrawLineEx(top_on_x, top_graphic, 1, PURPLE);
        DrawLineEx(top_graphic, top_on_y, 1, PURPLE);

        EndMode2D();
//----------------------------------
        //фон для легенды графика
        DrawRectangle(0, 0, 340, 70, WHITE);
        DrawLine(0, 70, 340, 70, BLACK);
        DrawLine(340, 0, 340, 70, BLACK);

        //Легенда графика
        switch (quantity)
        {
            case DECISION_ONE_ROOT:
                DrawText("The equation has 1 solution", 10, 10, 20, BLACK);
                DrawText(TextFormat("x1 = x2 = %lg", x1), 10, 30, 20, BLACK);

                break;
            case DECISION_TWO_ROOTS:
                DrawText("The equation has two solutions", 10, 10, 20, BLACK);
                DrawText(TextFormat("x1 =  %lg", x1), 10, 30, 20, BLACK);
                DrawText(TextFormat("x2 =  %lg", x2), 10, 50, 20, BLACK);

                break;
            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}