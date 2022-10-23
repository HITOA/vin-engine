#pragma once

#include <fmt/core.h>

#include "math/quaternion.hpp"

template<>
struct fmt::formatter<Vin::Quaternion<float>> {
	char presentation = 'f';

	constexpr auto parse(format_parse_context& m_Ctx) -> decltype(m_Ctx.begin()) {
		auto it = m_Ctx.begin(), end = m_Ctx.end();
		if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
		if (it != end && *it != '}') throw format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(const Vin::Quaternion<float>& quat, FormatContext& m_Ctx) const -> decltype(m_Ctx.out()) {
		return presentation == 'f'
			? fmt::format_to(m_Ctx.out(), "({:.1f}, {:.1f}, {:.1f}, {:.1f})", quat.w, quat.x, quat.y, quat.z)
			: fmt::format_to(m_Ctx.out(), "({:.1e}, {:.1e}, {:.1e})", quat.w, quat.x, quat.y, quat.z);
	}
};
