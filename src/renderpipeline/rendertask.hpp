#pragma once

namespace Vin {

	template<typename T>
	struct Identifier {
		static unsigned long long GetId(T& v) {
			return 0;
		}
	};

	template<typename T, typename Id = Identifier<T>>
	struct RenderTask {
		unsigned long long id;
		T data;

		RenderTask() = delete;
		RenderTask(T& data) : id{ Id::GetId(data) }, data { data } {};
	};

}
