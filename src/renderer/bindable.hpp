#pragma once

#include "core/identifiable.hpp"

namespace Vin {
	template<typename... Args>
	class Bindable : public Identifiable<Bindable<Args...>, unsigned short> {
	public:
		virtual void Bind(Args... args) = 0;
		virtual void Unbind() {};
	};
}
