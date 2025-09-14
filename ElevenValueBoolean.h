Copyright © 2025 Cadell Richard Anderson

//ElevenValueBoolean.h

#pragma once
#define NOMINMAX

#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>

/**
 * @brief The ElevenValueBoolean class represents a boolean concept with eleven distinct states (0-10).
 */
class ElevenValueBoolean {
public:
    enum State : unsigned char {
        AbsoluteUncertainty = 0,
        HighlyUnlikely = 1,
        Unlikely = 2,
        SomewhatFalse = 3,
        ModeratelyFalse = 4,
        Neutral = 5,
        ModeratelyTrue = 6,
        SomewhatTrue = 7,
        Likely = 8,
        HighlyLikely = 9,
        AbsoluteCertainty = 10
    };

private:
    State current_state;
    double chaotic_x;
    double chaotic_y;

    State clampState(double value) const {
        if (value < 0.0) return static_cast<State>(0);
        if (value > 10.0) return static_cast<State>(10);
        return static_cast<State>(static_cast<unsigned char>(std::round(value)));
    }

    void updateChaoticState() {
        const double a = 1.4;
        const double b = 0.3;
        double next_x = 1.0 - a * chaotic_x * chaotic_x + chaotic_y;
        double next_y = b * chaotic_x;
        chaotic_x = next_x;
        chaotic_y = next_y;
    }

public:
    ElevenValueBoolean() : current_state(AbsoluteUncertainty), chaotic_x(0.0), chaotic_y(0.0) {}
    ElevenValueBoolean(State initial_state) : current_state(initial_state) {
        chaotic_x = static_cast<double>(initial_state) / 10.0 - 0.5;
        chaotic_y = 0.0;
    }
    ElevenValueBoolean(int initial_value) {
        current_state = clampState(static_cast<double>(initial_value));
        chaotic_x = static_cast<double>(current_state) / 10.0 - 0.5;
        chaotic_y = 0.0;
    }

    State getState() const {
        return current_state;
    }

    // Corrected: Re-added the setState method.
    void setState(State new_state) {
        current_state = new_state;
    }

    ElevenValueBoolean operator!() const {
        ElevenValueBoolean result = *this;
        result.updateChaoticState();
        double new_value = 10.0 - static_cast<double>(result.current_state);
        result.current_state = result.clampState(new_value);
        return result;
    }

    ElevenValueBoolean operator&&(const ElevenValueBoolean& other) const {
        double combined_value = (static_cast<double>(current_state) + static_cast<double>(other.current_state)) / 2.0;
        combined_value = std::min(combined_value, static_cast<double>(std::min(current_state, other.current_state) + 2));
        ElevenValueBoolean result = *this;
        result.updateChaoticState();
        result.current_state = result.clampState(combined_value);
        return result;
    }

    ElevenValueBoolean operator||(const ElevenValueBoolean& other) const {
        double combined_value = (static_cast<double>(current_state) + static_cast<double>(other.current_state)) / 2.0;
        combined_value = std::max(combined_value, static_cast<double>(std::max(current_state, other.current_state) - 2));
        ElevenValueBoolean result = *this;
        result.updateChaoticState();
        result.current_state = result.clampState(combined_value);
        return result;
    }

    bool operator==(const ElevenValueBoolean& other) const { return current_state == other.current_state; }
    bool operator!=(const ElevenValueBoolean& other) const { return current_state != other.current_state; }

    bool isTrue() const { return current_state >= ModeratelyTrue; }
    bool isFalse() const { return current_state <= ModeratelyFalse; }
    bool isNeutral() const { return current_state == Neutral; }
    bool isBoth() const { return current_state == AbsoluteCertainty; }
    bool isLikely() const { return current_state >= Likely; }
    bool isHighlyLikely() const { return current_state == HighlyLikely; }

    std::string toString() const {
        switch (current_state) {
        case AbsoluteUncertainty: return "AbsoluteUncertainty (0)";
        case HighlyUnlikely:      return "HighlyUnlikely (1)";
        case Unlikely:            return "Unlikely (2)";
        case SomewhatFalse:       return "SomewhatFalse (3)";
        case ModeratelyFalse:     return "ModeratelyFalse (4)";
        case Neutral:             return "Neutral (5)";
        case ModeratelyTrue:      return "ModeratelyTrue (6)";
        case SomewhatTrue:        return "SomewhatTrue (7)";
        case Likely:              return "Likely (8)";
        case HighlyLikely:        return "HighlyLikely (9)";
        case AbsoluteCertainty:   return "AbsoluteCertainty (10)";
        default:                  return "Unknown State";
        }
    }
};

std::ostream& operator<<(std::ostream& os, const ElevenValueBoolean& val) {
    return os << val.toString();
}
