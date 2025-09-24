#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <memory>
#include <chrono>

extern "C" {
    #include <raylib.h>
}

int main()
{
    // Initialize the game window
    InitWindow(288, 512, "Flappy Bird DCA");
    SetTargetFPS(60); 

    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);

    // Main game loop provided by raylib
    while (!WindowShouldClose()) // This loop runs until the user closes the window
    {
        delta_time = GetFrameTime(); // Get the time passed since the last frame
        
        state_machine.handle_state_changes(delta_time);
        if (state_machine.getCurrentState() != nullptr) {
            state_machine.getCurrentState()->handleInput();
            state_machine.getCurrentState()->update(delta_time);
            state_machine.getCurrentState()->render(); 
        }
    }

    // De-initialize the game window
    CloseWindow();

    return 0;
}