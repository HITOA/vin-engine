#pragma once

#include <fmt/format.h>

#include "math/matrix.hpp"

template<size_t M, size_t N>
struct fmt::formatter<Vin::Matrix<float, M, N>> {
	char presentation = 'f';

	constexpr auto parse(format_parse_context& m_Ctx) -> decltype(m_Ctx.begin()) {
		auto it = m_Ctx.begin(), end = m_Ctx.end();
		if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
		if (it != end && *it != '}') throw format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(const Vin::Matrix<float, M, N>& mat, FormatContext& m_Ctx) const -> decltype(m_Ctx.out()) {
		std::string r{"\n"};

		for (size_t y = 0; y < M; y++) {
			r += "\t[";
			for (size_t x = 0; x < N; x++) {
				if (presentation == 'f')
					r += fmt::format("{:.1f}, ", mat.data[x + y * N]);
				else
					r += fmt::format("{:.1e}, ", mat.data[x + y * N]);
			}
			r = r.substr(0, r.size() - 2);
			r += "]\n";
		}

		return fmt::format_to(m_Ctx.out(), r);
	}
};
