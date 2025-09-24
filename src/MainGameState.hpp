#pragma once
#include <GameState.hpp>
#include <deque> // Incluye esta librería para usar la cola

extern "C" {
    #include <raylib.h>
}


struct Bird {
    float x;
    float y;
    float vy;
    float width;  
    float height; 
};

struct PipePair {
    Rectangle top;
    Rectangle bot;
    bool scored = false; 
};

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        Bird player;

        // Aquí se declaran las variables para los tubos
        std::deque<PipePair> pipes;
        const float PIPE_W = 32.0f;
        const float PIPE_H = 320.0f;
        const float PIPE_SPEED = 150.0f;
        const float SPAWN_EVERY = 2.0f; 
        float spawn_timer = 0.0f;       
        int score = 0; 
        
        Texture2D birdSprite;
        Texture2D pipeSprite;

};