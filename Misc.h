#include <algorithm>
#include <thread>
#include <mutex>


// convenient variable+mutex wrapper

template <typename T>
struct LockedItem {
	T value;
	std::mutex mutex;
	LockedItem(T init = T()) :value(init) {};
	~LockedItem() {};
};

typedef std::lock_guard<std::mutex> LockedItemGuard;


// less-redundant std::for_each

template <class Container, class Function>
Function for_each(Container &c, Function f) {
	return std::for_each(c.begin(), c.end(), f);
}
