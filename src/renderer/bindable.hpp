#pragma once

namespace Vin {
	template<typename... Args>
	class Bindable {
	public:
		virtual void Bind(Args... args) = 0;
		virtual void Unbind() {};
	};
}
