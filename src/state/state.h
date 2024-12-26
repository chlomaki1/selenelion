#pragma once

#include <discord.h>

enum GameState {
    UNKNOWN,
    UNKNOWN_1,
    UNKNOWN_2,
    UNKNOWN_3,
    LOADING,
    CHARACTER_SELECT,
};

class State {
    private:
        State();

        bool m_running;
        GameState m_currentState;
        GameState m_previousState;
        discord::Core* m_discordManager;
        discord::Activity m_currentActivity;

    public:
        State(State const&) = delete;
        void operator=(State const&)  = delete;

        static State& instance();

        bool isRunning();
        GameState getState();
        GameState getPreviousState();
        discord::Core *getDiscordCore();
        GameState getInternalState();
        void setState(GameState newState);
        void setRunning(bool running);
        void updateActivity(discord::Activity activity);

        void handleStateUpdate();
};
