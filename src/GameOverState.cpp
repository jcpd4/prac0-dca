#include <GameOverState.hpp>
#include <StateMachine.hpp>
#include <MainGameState.hpp> 
#include <memory>
#include <string> 


extern "C" {
    #include <raylib.h>
}

GameOverState::GameOverState(int score) : finalScore(score) {} // Inicializa la variable

void GameOverState::init() {
}

void GameOverState::handleInput() {
    // Si se presiona la tecla de espacio, reinicia el juego
    if (IsKeyPressed(KEY_SPACE)) {
        this->state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
}

void GameOverState::update(float deltaTime) {
}

void GameOverState::render() {
    BeginDrawing();

    ClearBackground(RAYWHITE); 

    // Dibuja el texto "Game Over" en el centro de la pantalla
    const char* message = "Game Over";
    int textWidth = MeasureText(message, 30);
    int x = (GetScreenWidth() - textWidth) / 2;
    int y = (GetScreenHeight() / 2) - 15; // Ajuste para centrar el texto verticalmente

    DrawText(message, x, y, 30, BLACK);

    // Dibuja la puntuación final
    std::string scoreMessage = "Score: " + std::to_string(finalScore);
    int scoreWidth = MeasureText(scoreMessage.c_str(), 20);
    DrawText(scoreMessage.c_str(), (GetScreenWidth() - scoreWidth) / 2, GetScreenHeight() / 2 + 10, 20, BLACK);


    EndDrawing();
}