#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/personality.h>
#include <sys/time.h>

#include <experimental/simd>

namespace stdx = std::experimental;
using namespace stdx::parallelism_v2;

template<size_t N>
using f = std::conditional_t<N == sizeof(float), float, std::conditional_t<sizeof(size_t) == sizeof(double), double, std::conditional_t<sizeof(size_t) == sizeof(long double), long double, void>>>;

template<size_t N>
using i = __int_with_sizeof_t<N / 8>;

template<size_t N>
using u = std::make_unsigned_t<i<N>>;

namespace sys
{
	bool init(); 
	bool halt();
#include "info.hpp"
#include "video.hpp"
	bool init() { return info::init() && video::init(); }
	bool halt() { return info::halt() && video::halt(); }
	bool step()
	{
		return info::step() && video::step();
	}
};
