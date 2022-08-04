#pragma once

#include "vinpch.hpp"

//Multithreaded task dispatcher

namespace Vin {

	class Task {

	};

	class TaskManager {
	public:
		static void Dispatch();
	private:
		static std::vector<Task*> tasks;
	};

}
