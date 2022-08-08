#pragma once

#include <fmt/format.h>

#include "core/math/matrix.hpp"

template<size_t M, size_t N>
struct fmt::formatter<Vin::Matrix<float, M, N>> {
	char presentation = 'f';

	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;
		if (it != end && *it != '}') throw format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(const Vin::Matrix<float, M, N>& mat, FormatContext& ctx) const -> decltype(ctx.out()) {
		std::string r{"\n"};

		for (size_t n = 0; n < N; n++) {
			r += "\t[";
			for (size_t m = 0; m < M; m++) {
				if (presentation == 'f')
					r += fmt::format("{:.1f}, ", mat(m, n));
				else
					r += fmt::format("{:.1e}, ", mat(m, n));
			}
			r = r.substr(0, r.size() - 2);
			r += "]\n";
		}

		return fmt::format_to(ctx.out(), r);
	}
};
