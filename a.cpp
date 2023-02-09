#include <iostream>

namespace foo {
	class A {
	public:
		bar::B* b;
		A() {
			std::cout << "A" << std::endl;
		}
	};
}
namespace bar {
	class B {
	public:
		foo::A* a;
		B() {
			std::cout << "B" << std::endl;
		}
	};
}

int main() {
	std::cout << "Hello, World!" << std::endl;
	return 0;
}