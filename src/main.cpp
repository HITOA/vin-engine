#include <vin.hpp>
#include <error.hpp>
#include <exception>

int main(int argc, char* argv[]) {
	try {

	}
	catch (const std::exception& err) {
		Vin::HandleUnmanagedError(err, 1);
	}
}