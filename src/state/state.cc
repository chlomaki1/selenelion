#include <functional>
#include <format>

#include "state.h"
#include "data.h"

#include <thread>
#include "../addresses.h"
#include "../util.h"

State& State::instance() {
  static State instance;
  return instance;
}

State::State()
:	m_running(false),
	m_previousState(GameState::UNKNOWN),
	m_currentState(GameState::UNKNOWN),
	m_discordManager(nullptr),
	m_logger(new Logger()),
	m_currentLocation(new Location(L"Unknown")),
	m_currentCharacter(new Character(0, L"_ Unknown _"))
{
	discord::Core* core;
	discord::Core::Create(1321163064900587643, DiscordCreateFlags_Default, &core);

	m_discordManager = core;
	m_currentActivity = new discord::Activity();
}

Logger* State::logger() {
  return m_logger;
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
	if (memory::CalculateAddress(0x3E314C8, { 0x918, 0x0, 0x110, 0x28, 0x160 }) == NULL) {
		return GameState::UNKNOWN;
	}

	return memory::ReadMemory(memory::CalculateAddress(0x3E314C8, { 0x918, 0x0, 0x110, 0x28, 0x160 }), GameState::UNKNOWN);
}

void State::setState(GameState state) {
  m_previousState = m_currentState;
  m_currentState = state;
}

void State::setRunning(bool running) {
  m_running = running;
}

// ...

void State::replaceActivity(std::function<discord::Activity*()> activityProvider) {
  m_currentActivity = activityProvider();
  m_discordManager->ActivityManager().UpdateActivity(*m_currentActivity, [this](discord::Result result) {
    if (result != discord::Result::Ok) {
      m_logger->log(Level::ERR, "RPC", std::format("Could not update Discord RPC: {}", static_cast<int>(result)));
    } else {
      m_logger->log(Level::INFO, "RPC", "Successfully updated Discord RPC to match state");
    }
  });
}

void State::updateActivity(std::function<void(discord::Activity*)> activityUpdater) {
  activityUpdater(m_currentActivity);

  m_discordManager->ActivityManager().UpdateActivity(*m_currentActivity, [this](discord::Result result) {
    if (result != discord::Result::Ok) {
      m_logger->log(Level::ERR, "RPC", std::format("Could not update Discord RPC: {}", static_cast<int>(result)));
    } else {
      m_logger->log(Level::INFO, "RPC", "Successfully updated Discord RPC to match state");
    }
  });
}

void State::updateActivityIf(std::function<bool(discord::Activity*)> conditionalUpdater) {
	// Attempt to modify `m_currentActivity` inside the lambda.
	// If it returns `true`, we proceed to update the activity.
	if (conditionalUpdater(m_currentActivity)) {
		m_discordManager->ActivityManager().UpdateActivity(*m_currentActivity, [this](discord::Result result) {
		  if (result != discord::Result::Ok) {
			m_logger->log(Level::ERR, "RPC",
						  std::format("Could not update Discord RPC: {}", static_cast<int>(result)));
		  } else {
			m_logger->log(Level::INFO, "RPC", "Successfully updated Discord RPC to match state");
		  }
		});
	}
}

void State::update() {
	if (m_currentState > CHARACTER_SELECT) {
		this->updateActivityIf([&](discord::Activity* activity) {
			bool shouldUpdate = false;

			std::cout << "trying to update activity" << std::endl;
			std::cout << activity->GetAssets().GetLargeText() << std::endl;
			std::cout << util::wcstrtocstr(m_currentLocation->getName()) << std::endl;

			if (strcmp(activity->GetAssets().GetLargeText(), util::wcstrtocstr(m_currentLocation->getName())) != 0) {
				shouldUpdate = true;

				activity->GetAssets().SetLargeImage(Data::instance().getDungeonImage(m_currentLocation->getName()));
				activity->GetAssets().SetLargeText(util::wcstrtocstr(m_currentLocation->getName()));
				m_logger->log(Level::DEBUG, "RPC", std::format("Changed current location to {}", util::wcstrtocstr(m_currentLocation->getName())));
			}

			/*if (strcmp(
				activity->GetAssets().GetSmallText(),
				std::format(
					"Lv{} {}",
					m_currentCharacter->getLevel(),
					util::wcstrtocstr(m_currentCharacter->getName())
				).c_str()
			) != 0) {
				shouldUpdate = true;
				activity->GetAssets().SetSmallText(std::format(
					"Lv{} {}",
					m_currentCharacter->getLevel(),
					util::wcstrtocstr(m_currentCharacter->getName())).c_str()
				);
			}*/

			return shouldUpdate;
		});
	}

	m_discordManager->RunCallbacks();
}

void State::handleStateUpdate() {
  m_logger->log(Level::INFO, "Game", std::format("State switched: {} (previously {})", static_cast<int>(m_currentState), static_cast<int>(m_previousState)));

	if (m_currentState == CHARACTER_SELECT) {
		m_currentLocation->setName(L"Unknown");
		m_currentCharacter->setName(L"Unknown");
		m_currentCharacter->setLevel(0);

		this->replaceActivity([]() {
		  discord::Activity* newActivity = new discord::Activity();

		  newActivity->SetDetails("Selecting a character");

		  // Set up newActivity as needed
		  return newActivity;
		});
	} else {
		m_currentLocation->setName(L"Unknown");
		m_currentCharacter->setName(L"Unknown");
		m_currentCharacter->setLevel(memory::ReadMemory(memory::CalculateAddress(0x03E314C8, { 0x830, 0x10, 0x8, 0x10, 0x0, 0x28, 0x20 }), 0));
	}

	if (m_currentState == VILLAGE || m_currentState == GUILD_BASE) {
		this->updateActivity([this](discord::Activity* activity) {
			activity->GetAssets().SetLargeImage(Data::instance().getDungeonImage(m_currentLocation->getName()));
			activity->GetAssets().SetSmallImage("default");
			activity->GetAssets().SetSmallText(std::format("Lv{} {}", m_currentCharacter->getLevel(), util::wcstrtocstr(m_currentCharacter->getName())).c_str());
			activity->GetTimestamps().SetStart(0LL);

			if (m_currentState == VILLAGE) {
				activity->SetDetails("In a village");
			} else if (m_currentState == GUILD_BASE) {
				activity->SetDetails("In a special location");
			}
		});
	} else if (m_currentState == DUNGEON) {
		this->updateActivity([this](discord::Activity* activity) {
			activity->SetDetails("In a dungeon");
			activity->GetTimestamps().SetStart(util::getCurrentTimestamp());
		});
	}
}