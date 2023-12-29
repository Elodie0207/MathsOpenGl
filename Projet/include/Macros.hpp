//
// Created by ianpo on 29/12/2023.
//

#pragma once

#define REI_CONCAT(x, y) x ## y
#define REI_COMBINE(x, y) REI_CONCAT(x, y)

// Defining the "_USE_MATH_DEFINES" for visual studio in the main hpp
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef __FUNCSIG__
#ifdef __PRETTY_FUNCTION__
		#define __FUNCSIG__ __PRETTY_FUNCTION__
	#else
		#define __FUNCSIG__ __FUNCTION__
	#endif
#endif

#ifdef REI_USE_SIMPLE_FUNCTION
#define REI_FUNC __FUNCTION__
#else
#define REI_FUNC __FUNCSIG__
#endif

#define REI_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


#ifdef REI_DEBUG
#ifndef REI_USE_ASSERT
        #define REI_USE_ASSERT 1
    #endif

    #ifndef REI_PROFILING
        #define REI_PROFILING 1
    #endif
#endif

#ifndef REI_DONT_PROFILE
    #ifndef REI_PROFILING
        #define REI_PROFILING 1
    #endif
#endif
