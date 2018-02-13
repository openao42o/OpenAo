#pragma once

#include "AtumProtocol.h"
#include "CompileUtils.h"

// I don't think you want to read this

// This is just a metalist<0, 1, 2, ... , 255>
// The reason I have splitted it is to work with Resharper C++
// It actually evaluates constexpr stuff while you write code

using metaseries256 = metaseries<0, 63>::append_list<metaseries<64, 127>>::append_list<metaseries<128, 191>>::append_list<metaseries<192, 255>>;


namespace proclist
{
	// #2 Don't start here, first check #1 (scroll down)
	//
	// These two classes are defined later, they will basically contain something like
	//
	// using list = metalist<T0, T0::abc, T0::def, ... >;
	//
	// and
	//
	// using T1 = T1<T0::abc>;
	// using list = metalist<T1, T1::xyz, T1::uvy, ... >;
	//
	// respectively, so that we can define which message types are in use

	template<typename socket_t> struct ProcessesT0;

	template<typename socket_t, T0> struct ProcessesT1;


	// #3 Another class used like a function
	//
	// if msgtype != T_NA
	// this class has "value = &socket_t::HandlerT1<msgtype>::handler"
	// otherwise
	// this class has "value = nullptr"

	template<typename socket_t, MessageType_t msgtype> struct get_handlerT1
	{
		using result_t = ProcessResult(*const)(socket_t*, const char*, int, int&);
		static constexpr auto value = result_t(&socket_t::template HandlerT1<msgtype>::handler);
	};

	template<typename socket_t> struct get_handlerT1<socket_t, T_NA> { static constexpr auto value = nullptr; };

	// #1 Start here
	template<typename socket_t, T0 t0> struct get_handlersT1
	{
		// result_t is required by metalist to determine the type of
		// the metalist result, in this case it is pointer to function that handles messages
		using result_t = ProcessResult(*const)(socket_t*, const char*, int, int&);

		// this will be used to transform series 0 to 255
		// into transformer<0> to transformer<255>
		template<size_t N> struct transformer
		{
			// check if this T1 is used
			// ProcessesT1<socket_t, t0>::list is a metalist (check #2)
			static constexpr bool valid = ProcessesT1<socket_t, t0>::list::contains(T1<t0>(N));

			// msgtype is T0 << 8 + T1 if valid
			// otherwise T_NA
			static constexpr MessageType_t msgtype = valid ? (unsigned(t0) << 8) + N : T_NA;

			// get a pointer to the function that handles that message
			// or nullptr for T_NA
			static constexpr result_t value = get_handlerT1<socket_t, msgtype>::value;
		};
	};


	// #4 Same logic
	//
	// if you can understand this code you are ok

	template<typename socket_t, T0 t0> struct get_handlerT0
	{
		using result_t = ProcessResult(*const)(socket_t*, MessageType_t, const char*, int, int&);
		static constexpr auto value = result_t(&socket_t::template HandlerT0<t0>);
	};

	template<typename socket_t> struct get_handlerT0<socket_t, T0::NA> { static constexpr auto value = nullptr; };

	template<typename socket_t> struct get_handlersT0
	{
		using result_t = ProcessResult(*const)(socket_t*, MessageType_t, const char*, int, int&);

		template<size_t N> struct transformer
		{
			static constexpr bool valid = ProcessesT0<socket_t>::list::contains(T0(N));

			static constexpr T0 t0 = valid ? T0(N) : T0::NA;

			static constexpr result_t value = get_handlerT0<socket_t, t0>::value;
		};
	};
}