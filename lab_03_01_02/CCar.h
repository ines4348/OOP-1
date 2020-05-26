#pragma once
#include <array>
enum class Direction
{
	Forward,
	Back,
	InPlace
};

const int GEAR_MIN = -1;
const int GEAR_MAX = 5;

const std::array<std::pair<int, int>, 7> SPEED_RANGES = { {
	{ 0, 20 },
	{ 0, 150 },
	{ 0, 30 },
	{ 20, 50 },
	{ 30, 60 },
	{ 40, 90 },
	{ 50, 150 },
} };

class CCar
{
public:
	bool TurnOnEngine();
	bool TurnOffEngine();
	bool SetGear(int gear);
	bool SetSpeed(int speed);

	bool IsEngineOn() const;
	Direction GetDirection() const;
	int GetGear() const;
	int GetSpeed() const;

private:
	bool m_isEngineOn = false;
	Direction m_direction = Direction::InPlace;
	int m_gear = 0;
	int m_speed = 0;
};

