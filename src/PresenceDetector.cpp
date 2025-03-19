//
// Created by thorgan on 3/17/25.
//

#include "PresenceDetector.h"
#include <MyAny.h>
#include <environment.h>
#include <utils.h>

PresenceDetector::PresenceDetector(Broker *broker, const bool value) {

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Creating PresenceDetector with value: %s\n", toString(value).c_str());
	}


    this->a_name = PRESENCE_DETECTOR;
    this->a_broker = broker;
    this->a_value = value;
}

void PresenceDetector::setValue(const std::string & value) {

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("Setting %s value %s...\n", this->getName().c_str(), value.c_str());
	}

    if (isBool(value)) {
        const bool position = parseBool(value);

        if (*IS_DEBUG_MODE) {
            // DEBUG
            Serial.printf("current value: %s/ new value: %s\n", toString(this->a_value).c_str(), toString(position).c_str());
        }

        // Ignore repeated values
        if (this->a_value == position) return;

        this->a_value = position;

        this->Notify();

        // TODO -> implement lightSensor logic when changing position

        this->a_broker->pub(this->getName(), toString(position));
        return;
    }

	if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.printf("invalid value: %s\n", value.c_str());
	}
}

const std::string PresenceDetector::getValue() const {
    return toString(this->a_value);
}

const MyAny PresenceDetector::getValueReference() const {
	return * new MyAny((void *) & this->a_value, "bool");
}

void PresenceDetector::Update(const std::string & module_name, const std::string & value) {
}

void PresenceDetector::Attach(IObserver *observer) {
  this->a_observers.push_back(observer);
}

void PresenceDetector::Detach(IObserver *observer) {
    for (auto itr = this->a_observers.begin(); itr != this->a_observers.end();)
    {
        if (*itr == observer)
            itr = this->a_observers.erase(itr);
        else
            ++itr;
    }
}

void PresenceDetector::Notify() {

    if (*IS_DEBUG_MODE) {
        // DEBUG
        Serial.println("PresenceDetector notifying observers of an update...");
    }

    int i = 0;
    for (IObserver* observer : this->a_observers) {
        if (*IS_DEBUG_MODE) {
            // DEBUG
            Serial.printf("PresenceDetector notifying observer #%d of an update...\n", i);
        }
        ++i;

        observer->Update(PRESENCE_DETECTOR, this->getValue());
    }
}
