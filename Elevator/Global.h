#pragma once

enum class HumanState {
	WaitingUpside = 0,
	WaitingDownSide = 1,
	Staying = 2,
	OnElevator = 3,
};

enum class ElevatorState {
	Idle = 0,
	Transmitting = 1,
	Boarding = 2,
	Full = 4,
	Waiting = 8,
	Upper = 16,
	Lower = 32
};

const int floor_per_block = 2;
const int sync_period = 1000; //ms

int GetRandomPatience()
{
	return rand() % 100 + 20; //���d�ɶ�
}

template<typename Enum>
Enum operator |(Enum lhs, Enum rhs)
{
	static_assert(std::is_enum<Enum>::value,
		"template parameter is not an enum type");

	using underlying = typename std::underlying_type<Enum>::type;

	return static_cast<Enum> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);
}

template<typename Enum>
Enum operator &(Enum lhs, Enum rhs)
{
	static_assert(std::is_enum<Enum>::value,
		"template parameter is not an enum type");

	using underlying = typename std::underlying_type<Enum>::type;

	return static_cast<Enum> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);
}

template<typename Enum>
Enum operator ^(Enum lhs, Enum rhs)
{
	static_assert(std::is_enum<Enum>::value,
		"template parameter is not an enum type");

	using underlying = typename std::underlying_type<Enum>::type;

	return static_cast<Enum> (
		static_cast<underlying>(lhs) ^
		static_cast<underlying>(rhs)
		);
}

static int HID = 0;