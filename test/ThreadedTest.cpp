#include <iostream>
#include <thread>

#include "sstl/Vector.h"
#include "sstl/Memory.h"
#include "sstl/Threading.h"

#include "TestShared.h"

int main() {

	TThreadSafe<SObject> obj1{100, "Hey"};
	obj1->print();

	TThreadSafe<SObject> obj2{"Hey2", 200};
	obj2->print();

	TThreadSafe<TShared<SObject>> obj3{300, "Hey3"};
	obj3->print();

	TThreadSafe<TShared<SObject>> obj4{"Hey4", 400};
	obj4->print();

	TThreadSafe<TShared<SObject>> obj5{obj4};
	obj4->print();
	obj5->print();

	TThreadSafe<TUnique<SObject>> obj6{600, "Hey6"};
	obj6->print();

	TThreadSafe<TUnique<SObject>> obj7{"Hey7", 700};
	obj7->print();

	TThreadSafe<TUnique<SObject>> obj8{std::move(obj7)};
	obj8->print();

	return 0;
}

int main2() {

	TThreadSafe<TVector<TUnique<SObject>>> vec;

	char c = 'y';
	while (c != 'n') {
		std::thread threadOne([&] {
			vec->push(TUnique<SObject>{100, "Thread One"});
			vec->top()->print();
			//vec->doFor(0, [](const TUnique<SObject>& obj) { obj->print(); });
			vec->pop(static_cast<size_t>(0));
		});
		vec->push(TUnique<SObject>{101, "Thread Two"});
		vec->top()->print();
		//vec->doFor(0, [](TUnique<SObject>& obj) { obj->print(); });
		vec->pop(static_cast<size_t>(0));

		std::cout << "Continue?" << std::endl;
		std::cin >> c;

		threadOne.join();
	}

	vec->clear();

	return 0;
}