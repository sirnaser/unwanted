#ifndef GAMESTATE_H
#define GAMESTATE_H

using namespace std;

enum class AppState {
    SPLASH,    // Phase 1: Welcome screen
    LOGIN,     // Phase 1: Name input + map select
    GAME       // Phase 1 board + Phase 2 logic
};

#endif // GAMESTATE_H
