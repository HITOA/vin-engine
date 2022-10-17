#pragma once

#include <fmt/format.h>

#include "math/quaternion.hpp"

template<>
struct fmt::formatter<Vin::Quaternion<float>> {
	char presentation = 'f';

	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
		if (it != end && *it != '}') throw format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(const Vin::Quaternion<float>& quat, FormatContext& ctx) const -> decltype(ctx.out()) {
		return presentation == 'f'
			? fmt::format_to(ctx.out(), "({:.1f}, {:.1f}, {:.1f}, {:.1f})", quat.w, quat.x, quat.y, quat.z)
			: fmt::format_to(ctx.out(), "({:.1e}, {:.1e}, {:.1e})", quat.w, quat.x, quat.y, quat.z);
	}
};
