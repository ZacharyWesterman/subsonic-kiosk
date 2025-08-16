#include "uid.hpp"

int uid() {
	volatile static int uid = 0;
	return uid++;
}
