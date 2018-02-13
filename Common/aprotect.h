#pragma once

#include <type_traits>

template<typename T, unsigned Seed>
class Protected
{
	using data_t = T;

	static_assert(!std::is_reference<data_t>::value, "data type cannot be reference");
	
	union union_t
	{
		data_t value;
		char data[sizeof(data_t)];
	};

	union_t _0, _1;

	__forceinline void enc()
	{
		for (int i = 0; i < sizeof(data_t); ++i)
			
			_1.data[i] = _0.data[i] ^ ~((sizeof(data_t) + i + Seed));
	}

	__forceinline bool check() const
	{
		for (int i = 0; i < sizeof(data_t); ++i)
			
			if (_1.data[i] ^ _0.data[i] != ~((sizeof(data_t) + i + Seed)))
				
				return false;

		return true;
	}

public:
	__forceinline Protected()
	{
		_0.value = { };

		enc();
	}

	__forceinline Protected(const data_t& value)
	{
		_0.value = value;

		enc();
	}

	__forceinline ~Protected() = default;

	Protected<T, Seed>& operator=(Protected<T, Seed>&) = delete;
	Protected<T, Seed>& operator=(Protected<T, Seed>&&) = delete;
	Protected<T, Seed>& operator=(const Protected<T, Seed>&) = delete;

	__forceinline Protected<T, Seed>& operator=(const data_t& value)
	{
		_0.value = value;

		enc();

		return *this;
	}

	__forceinline operator const data_t&() const
	{
		return check() ? _0.value : data_t { };
	}
};