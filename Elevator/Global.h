#pragma once

enum class HumanState {
	Waiting = 0,
	Staying = 1,
	OnElevator = 2,
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