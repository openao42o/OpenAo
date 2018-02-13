#pragma once

namespace ct
{
	// length of null terminated string 
	constexpr unsigned strlen(const char* str, unsigned i)
	{
		// why no out of bounds checking you may ask
		// because the compiler does it anyway
		// and it provides better error description too

		return str[i] != '\0' ? strlen(str, i + 1) : i;
	}
}

// length of null terminated string at compile time
constexpr unsigned cstrlen(const char* str) { return ct::strlen(str, 0); }



// length of an array at compile time
template<typename t, unsigned n> constexpr auto carrlen(const t(&)[n]) { return n; }


template<typename T, T... series> struct metalist
{
	static_assert(sizeof...(series) > 0, "empty metalist is invalid");

	template<typename F> struct transform
	{
		static constexpr size_t size = sizeof...(series);

		static constexpr typename F::result_t values[] = { (F::template transformer<series>::value)... };
	};

	template<T... more> using append = metalist<T, series..., more...>;
	template<T... more> using prepend = metalist<T, more..., series...>;

	template<typename list> using append_list = typename list::template prepend<series...>;
	template<typename list> using prepend_list = typename list::template append<series...>;

	static constexpr size_t size = sizeof...(series);

	static constexpr T values[size] = { series... };

	static constexpr bool contains(T x) { return contains(x, 0); }

protected:
	static constexpr bool contains(T x, size_t pos) { return pos < size && (values[pos] == x || contains(x, pos + 1)); }
};


template<size_t MIN, size_t MAX> struct metaseries : metalist<size_t, MAX>::template prepend_list<metaseries<MIN, MAX - 1>> { };

template<size_t MIN> struct metaseries<MIN, MIN> : metalist<size_t, MIN> { };