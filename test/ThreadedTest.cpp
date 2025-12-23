#include <iostream>
#include <random>
#include <thread>

#include "sstl/List.h"
#include "sstl/Vector.h"
#include "sstl/Memory.h"
#include "sstl/Threading.h"

struct Parent {
	Parent() = default;
	Parent(const size_t id): id(id) {}
	virtual ~Parent() = default;
	virtual void print() const {
		std::cout << "ID: " << id << std::endl;
	}

	friend bool operator<(const Parent& fst, const Parent& snd) {
		return fst.id < snd.id;
	}

	friend bool operator==(const Parent& fst, const Parent& snd) {
		return fst.id == snd.id;
	}

	friend size_t getHash(const Parent& obj) {
		return obj.id;
	}

	int id = 0;
};

struct SObject : Parent{

	SObject() = default;
	SObject(const size_t id, std::string name): Parent(id), name(std::move(name)) {}

	std::string name = "None";

	virtual void print() const override {
		std::cout << "ID: " << id << " Name: " << name << std::endl;
	}

	friend bool operator<(const SObject& fst, const SObject& snd) {
		return fst.id < snd.id;
	}

	friend bool operator==(const SObject& fst, const SObject& snd) {
		return fst.id == snd.id;
	}

	friend size_t getHash(const SObject& obj) {
		return obj.id;
	}
};

int main() {

	TThreadSafe<TVector<TUnique<SObject>>> vec;

	char c = 'y';
	while (c != 'n') {
		std::thread threadOne([&] {
			vec->push(SObject{100, "Thread One"});
			vec->top()->print();
			//vec->doFor(0, [](const TUnique<SObject>& obj) { obj->print(); });
			vec->pop(static_cast<size_t>(0));
		});
		vec->push(SObject{101, "Thread Two"});
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