#pragma once

#include <iostream>
#include <gtest/gtest.h>
#include <events/event.hpp>

class Dispatcher : public Vin::EventDispatcher {
public:
	void SendEvent() {
		Vin::Event e{
			Vin::EventType::KeyPressed,
			Vin::EventCategory::EventCategoryInput | Vin::EventCategory::EventCategoryKeyboard,
			"Key Pressed"
		};

		Dispatch(e);
	}
};

class Listener : public Vin::EventListener {
public:
	Listener() {
		dispatcher.RegisterListener(this);
		dispatcher.SendEvent();
		EXPECT_EQ(didReceivedEvent, true) << "Did not received the event";
	}
	void OnEvent(const Vin::Event& e) {
		EXPECT_EQ(e.type, Vin::EventType::KeyPressed) << "Wrong event type";
		didReceivedEvent = true;
	}
private:
	Dispatcher dispatcher{};
	bool didReceivedEvent{ false };
};

TEST(event_test, dispatch_test) {
	Listener listener{};
}
