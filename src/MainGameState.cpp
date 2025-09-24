#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <GameOverState.hpp>
#include <iostream>
#include <random>
#include <time.h> 

extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState()
{
}

void MainGameState::init()
{
    // Inicialización de la posición del pájaro
    player.x = 100.0f;
    player.y = 200.0f;

    // Inicialización de la velocidad vertical
    player.vy = 0.0f;

    // ejercicio 5
    birdSprite = LoadTexture("assets/yellowbird-midflap.png");
    pipeSprite = LoadTexture("assets/pipe-green.png");

    // Define el ancho y alto del pájaro a partir de la textura
    player.width = (float)birdSprite.width;
    player.height = (float)birdSprite.height;
}

void MainGameState::handleInput()
{
    // Detecta si el usuario pulsa la tecla espacio
    if (IsKeyPressed(KEY_SPACE)) {
        player.vy = -300.0f;
    }
}

void MainGameState::update(float deltaTime)
{
    const float gravity = 980.0f;
    player.vy += gravity * deltaTime;
    player.y += player.vy * deltaTime;

    
    // 1. Generación de tubos
    spawn_timer += deltaTime;
    if (spawn_timer >= SPAWN_EVERY) {
        spawn_timer = 0.0f; 
        
        float pipe_y_offset_top = (float)GetRandomValue(PIPE_H/2, GetScreenHeight() - (PIPE_H/2));
        
        const float PIPE_GAP = 150.0f;
        
        // Crea las tuberías superior e inferior
        PipePair new_pipes;
        
        new_pipes.top.x = (float)GetScreenWidth(); // Empieza fuera de la pantalla, a la derecha
        new_pipes.top.y = pipe_y_offset_top - PIPE_H;
        new_pipes.top.width = PIPE_W;
        new_pipes.top.height = PIPE_H;

        new_pipes.bot.x = (float)GetScreenWidth();
        new_pipes.bot.y = new_pipes.top.y + PIPE_H + PIPE_GAP;
        new_pipes.bot.width = PIPE_W;
        new_pipes.bot.height = PIPE_H;

        pipes.push_back(new_pipes);
    }
    
    // 2. Movimiento de tubos
    for (auto& p : pipes) {
        p.top.x -= PIPE_SPEED * deltaTime;
        p.bot.x -= PIPE_SPEED * deltaTime;

            // Lógica para sumar la puntuación
        if (!p.scored && p.top.x < player.x) {
            score++;
            p.scored = true;
        }
    }
    
    // 3. Eliminación de tubos que están fuera de pantalla
    if (!pipes.empty() && pipes.front().top.x < -PIPE_W) {
        pipes.pop_front();
    }


    // EJERCICIO 3
    Rectangle playerRec = { player.x, player.y, (float)player.width, (float)player.height };
    // 2. Comprobación de colisiones con los tubos
    for (const auto& p : pipes) {
        if (CheckCollisionRecs(playerRec, p.top) || CheckCollisionRecs(playerRec, p.bot)) {
            this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
        }
    }

    // 3. Comprobación de colisiones con los bordes de la pantalla
    if (player.y < 0 || player.y > 512) {
        this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
    }
}

void MainGameState::render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE); 

    DrawTextureEx(birdSprite, {player.x, player.y}, 0.0f, 1.0f, WHITE);

    // --- Lógica para dibujar los tubos ---
    for (const auto& p : pipes) {
        DrawTextureEx(pipeSprite, { p.top.x + p.top.width, p.top.y + p.top.height }, 180.0f, 1.0f, WHITE);
        DrawTextureEx(pipeSprite, { p.bot.x, p.bot.y }, 0.0f, 1.0f, WHITE);
    }

    //MARCADOR
    std::string score_text = std::to_string(score);
    DrawText(score_text.c_str(), 10, 10, 40, BLACK);


    // Draw the welcome text
    DrawText("Bienvenido a Flappy Bird DCA", 20, 20, 20, BLACK);

    // End drawing
    EndDrawing();
}