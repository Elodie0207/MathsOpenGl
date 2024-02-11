//
// Created by ianpo on 11/02/2024.
//

#pragma once

#include "Math.hpp"
#include "UUID.hpp"

namespace ColorHelper
{
	inline Color Random()
	{
		return {
				(double)UUID().operator uint64_t() / (double)UINT64_MAX,
				(double)UUID().operator uint64_t() / (double)UINT64_MAX,
				(double)UUID().operator uint64_t() / (double)UINT64_MAX,
				1,
		};
	}
}