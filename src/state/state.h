#pragma once

#include <discord.h>
#include "../data/location.h"
#include "../data/character.h"
#include "logger.h"

enum GameState {
    UNKNOWN,
    UNKNOWN_1,
    UNKNOWN_2,
    UNKNOWN_3,
    LOADING,
    CHARACTER_SELECT,
    UNKNOWN_4,
    UNKNOWN_5,
    UNKNOWN_6,
    UNKNOWN_7,
    UNKNOWN_8,
    DUNGEON,
    UNKNOWN_10,
    UNKNOWN_11,
    VILLAGE,
    GUILD_BASE, // could be something else, honestly
    UNKNOWN_14,
    UNKNOWN_15,
    UNKNOWN_16,
    UNKNOWN_17,
    UNKNOWN_18,
    FIELD,
    UNKNOWN_20,
    UNKNOWN_21,
    UNKNOWN_22,

};

class State {
    private:
        State();

        bool m_running;
        GameState m_currentState;
        GameState m_previousState;
        discord::Core* m_discordManager;
        discord::Activity* m_currentActivity;
        Logger* m_logger;

        Location* m_currentLocation;
        Character* m_currentCharacter;

    public:
        State(State const&) = delete;
        void operator=(State const&)  = delete;

        static State& instance();

        Logger* logger();

        bool isRunning();
        GameState getState();
        GameState getPreviousState();
        discord::Core *getDiscordCore();
        GameState getInternalState();

        void setState(GameState newState);
        void setRunning(bool running);

        void updateActivity(std::function<void(discord::Activity *)> activityUpdater);
        void updateActivityIf(std::function<bool(discord::Activity*)> conditionalUpdater);
        void replaceActivity(std::function<discord::Activity *()> activityProvider);
        void refreshActivity();

        void update();
        void handleStateUpdate();
};