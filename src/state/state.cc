#include "state.h"
#include "../addresses.h"

State& State::instance() {
  static State instance;

  return instance;
}

State::State()
: m_running(false),
  m_previousState(GameState::UNKNOWN),
  m_currentState(GameState::UNKNOWN),
  m_discordManager(nullptr)
{
  discord::Core* core;
  discord::Core::Create(1321163064900587643, DiscordCreateFlags_Default, &core);

  m_discordManager = core;
}

bool State::isRunning() {
  return m_running;
}

GameState State::getPreviousState() {
  return m_previousState;
}

GameState State::getState() {
  return m_currentState;
}

discord::Core* State::getDiscordCore() {
  return m_discordManager;
}

GameState State::getInternalState() {
  if (CalculateAddress(0x3E314C8, { 0x918, 0x0, 0x110, 0x28, 0x160 }) == NULL) {
    return GameState::UNKNOWN;
  }

  return *reinterpret_cast<GameState*>(CalculateAddress(0x3E314C8, { 0x918, 0x0, 0x110, 0x28, 0x160 }));
}

void State::setState(GameState state) {
  m_previousState = m_currentState;
  m_currentState = state;

  handleStateUpdate();
}

void State::setRunning(bool running) {
  m_running = running;
}

void State::updateActivity(discord::Activity activity) {
  m_currentActivity = activity;
  m_discordManager->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}

void State::handleStateUpdate() {
  discord::Activity activity {};

  switch (m_currentState) {
    case CHARACTER_SELECT:
      activity.SetDetails("Selecting a character");
  }
}
