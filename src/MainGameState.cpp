#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <GameOverState.hpp> // la clase creada en el ejercicio 3
#include <iostream>
#include <random> // Necesario para generar números aleatorios
#include <time.h> // Necesario para inicializar la semilla

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
    // Lógica del pájaro (paso 1)
    const float gravity = 980.0f;
    player.vy += gravity * deltaTime;
    player.y += player.vy * deltaTime;

    // Lógica de los tubos (paso 3)
    
    // 1. Generación de tubos
    spawn_timer += deltaTime;
    if (spawn_timer >= SPAWN_EVERY) {
        spawn_timer = 0.0f; // Reinicia el temporizador
        
        // Genera un valor aleatorio para el desplazamiento vertical de los tubos
        // raylib.h ya incluye una función GetRandomValue() que es más sencilla
        float pipe_y_offset_top = (float)GetRandomValue(PIPE_H/2, GetScreenHeight() - (PIPE_H/2));
        
        // Define el espacio entre las tuberías. Un buen valor es 150.0f.
        const float PIPE_GAP = 150.0f;
        
        // Crea las tuberías superior e inferior
        PipePair new_pipes;
        
        // Tubería superior
        new_pipes.top.x = (float)GetScreenWidth(); // Empieza fuera de la pantalla, a la derecha
        new_pipes.top.y = pipe_y_offset_top - PIPE_H;
        new_pipes.top.width = PIPE_W;
        new_pipes.top.height = PIPE_H;

        // Tubería inferior
        new_pipes.bot.x = (float)GetScreenWidth();
        new_pipes.bot.y = new_pipes.top.y + PIPE_H + PIPE_GAP;
        new_pipes.bot.width = PIPE_W;
        new_pipes.bot.height = PIPE_H;

        // Añade el nuevo par de tubos a la cola
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
    // 1. Define el bounding box del pájaro
    Rectangle playerRec = {player.x - 17, player.y - 17, 34, 34};

    // Opcional: Dibuja el bounding box para depurar (luego lo puedes borrar)
    // DrawRectangleRec(playerRec, Fade(RED, 0.5f));

    // 2. Comprobación de colisiones con los tubos
    for (const auto& p : pipes) {
        if (CheckCollisionRecs(playerRec, p.top) || CheckCollisionRecs(playerRec, p.bot)) {
            // Cambia al estado de Game Over, pasando la puntuación actual
            this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
        }
    }

    // 3. Comprobación de colisiones con los bordes de la pantalla
    if (player.y < 0 || player.y > 512) {
        // Cambia al estado de Game Over, pasando la puntuación actual
        this->state_machine->add_state(std::make_unique<GameOverState>(score), true);
    }
}

void MainGameState::render()
{
    // Start drawing
    BeginDrawing();

    // Clear the background to a color
    ClearBackground(RAYWHITE); 

    // Dibuja el pájaro como un círculo rojo
    DrawCircle((int)player.x, (int)player.y, 17, RED);

    // --- Lógica para dibujar los tubos ---
    for (const auto& p : pipes) {
        // Dibuja la tubería superior
        DrawRectangleRec(p.top, GREEN);
        
        // Dibuja la tubería inferior
        DrawRectangleRec(p.bot, GREEN);
    }

    //MARCADOR
    std::string score_text = std::to_string(score);
    DrawText(score_text.c_str(), 10, 10, 40, BLACK);


    // Draw the welcome text
    DrawText("Bienvenido a Flappy Bird DCA", 20, 20, 20, BLACK);

    // End drawing
    EndDrawing();
}