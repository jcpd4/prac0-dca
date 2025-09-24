#pragma once

#include <GameState.hpp>
#include <memory>

// Incluye raylib para usar sus funciones de dibujo y manejo de entrada
extern "C" {
    #include <raylib.h>
}

class GameOverState : public GameState
{
public:
    GameOverState(int score);
    ~GameOverState() = default;

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause(){};
    void resume(){};

private:
    int finalScore; // Variable para almacenar la puntuación

};