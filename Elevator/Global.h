#pragma once

enum class UserState {
	WaitingUpside = 0,
	WaitingDownSide = 1,
	Staying = 2,
	OnElevator = 3,
	ForPurge = 4
};

enum class ElevatorState {
	Idle = 0,
	Moving = 1,
	Loading = 2,
	UnLoading = 3,
};

enum class ElevatorMoveState {
	Upside = 0,
	DownSide = 1
};

const int floor_per_block = 2;
const int sync_period = 100; //ms(real time)
const int floor_count = 12;   
const double board_speed = 0.7;//time
const int target_null = -1;
const int elevator_speed = 1; //1 block/time

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