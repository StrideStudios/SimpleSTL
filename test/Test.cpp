#include <iostream>
#include <random>
#include <cassert>

#include "sstl/Array.h"
#include "sstl/Deque.h"
#include "sstl/ForwardList.h"
#include "sstl/Vector.h"
#include "sstl/MaxHeap.h"
#include "sstl/MinHeap.h"
#include "sstl/List.h"
#include "sstl/MultiSet.h"
#include "sstl/PriorityMultiSet.h"
#include "sstl/Queue.h"
#include "sstl/Set.h"
#include "sstl/PrioritySet.h"
#include "sstl/Stack.h"
#include "sstl/Map.h"
#include "sstl/Memory.h"
#include "sstl/PriorityMap.h"
#include "sstl/MultiMap.h"
#include "sstl/PriorityMultiMap.h"

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

template <typename TType, size_t TSize>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Parent, typename TUnfurled<TType>::Type>
#endif
void containerTest(const std::string& containerName, TSequenceContainer<TType, TSize>& container) {

	TType obj = SObject{100, "Hello"};
	TType obj2 = SObject{100, "Hello"};
	container.push(std::move(obj));
	std::cout << "Container " << containerName << (container.contains(obj2) ? " DOES " : " DOES NOT ") << "Contain Object" << std::endl;

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i+500);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	RANGES(shuffle, vec, rng);

	container.resize(10, [&](TType& object, const size_t index) {
		object = SObject{vec[index], containerName};
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t i = 0; i < size; ++i) {
		getUnfurled(container.top())->print();
		container.pop();
	}
	std::cout << std::endl;
}

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Parent, typename TUnfurled<TType>::Type>
#endif
void containerTest(const std::string& containerName, TSingleAssociativeContainer<TType>& container) {

	TType obj = SObject{100, "Hello"};
	TType obj2 = SObject{100, "Hello"};
	container.push(std::move(obj));
	std::cout << "Container " << containerName << (container.contains(obj2) ? " DOES " : " DOES NOT ") << "Contain Object" << std::endl;

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	RANGES(shuffle, vec, rng);

	size_t i = 0;

	container.resize(10, [&](TType& object) {
		object = SObject{vec[i], containerName};
		++i;
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t v = 0; v < size; ++v) {
		getUnfurled(container.top())->print();
		container.pop();
	}
	std::cout << std::endl;
}

enum class MapEnum : uint8_t {
	NONE,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE
};

inline size_t getHash(const MapEnum& obj) {
	return (size_t)obj;
}

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Parent, typename TUnfurled<TType>::Type>
#endif
void containerTest(const std::string& containerName, TAssociativeContainer<MapEnum, TType>& container) {

	TType obj = SObject{100, "Hello"};
	container.push(MapEnum::NONE, std::move(obj));
	std::cout << "Container " << containerName << (container.contains(MapEnum::NONE) ? " DOES " : " DOES NOT ") << "Contain Object" << std::endl;

	std::vector<size_t> vec;
	for (size_t i = 1; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	RANGES(shuffle, vec, rng);

	size_t i = 0;

	container.resize(10, [&](TPair<MapEnum, TType>& pair) {
		pair = TPair<MapEnum, TType>{(MapEnum)vec[i], SObject{vec[i], containerName}};
		++i;
	});
	assert(container.getSize() == 10);

	auto EnumToString = [](const MapEnum val) {
		switch (val) {
			case MapEnum::NONE:
				return "NONE";
			case MapEnum::ONE:
				return "ONE";
			case MapEnum::TWO:
				return "TWO";
			case MapEnum::THREE:
				return "THREE";
			case MapEnum::FOUR:
				return "FOUR";
			case MapEnum::FIVE:
				return "FIVE";
			case MapEnum::SIX:
				return "SIX";
			case MapEnum::SEVEN:
				return "SEVEN";
			case MapEnum::EIGHT:
				return "EIGHT";
			case MapEnum::NINE:
				return "NINE";
		}
		return "";
	};

	const size_t size = container.getSize();
	for (size_t v = 0; v < size; ++v) {
		MapEnum enm = (MapEnum)v;
		if (auto object = getUnfurled(container.get(enm))) {
			std::cout << "Key: " << EnumToString(enm) << " ";
			object->print();
			container.pop(enm);
		}
	}

	std::cout << std::endl;
}

#define DO_TEST(x) \
	{ x<Parent> container; containerTest(#x, container); } \
	{ x<TShared<Parent>> container; containerTest(#x " Shared", container); } \
	{ x<TUnique<Parent>> container; containerTest(#x " Unique", container); }

#define DO_ARRAY_TEST(x) \
	{ x<Parent, 10> container; containerTest(#x, container); } \
	{ x<TShared<Parent>, 10> container; containerTest(#x " Shared", container); } \
	{ x<TUnique<Parent>, 10> container; containerTest(#x " Unique", container); }

#define DO_MAP_TEST(x) \
	{ x<MapEnum, Parent> container; containerTest(#x, container); } \
	{ x<MapEnum, TShared<Parent>> container; containerTest(#x " Shared", container); } \
	{ x<MapEnum, TUnique<Parent>> container; containerTest(#x " Unique", container); }

int main() {

	auto lamb = [](TUnique<Parent>& out) {
		out = TUnique<SObject>{200, "Hello"};
	};

	TUnique<Parent> obj;
	lamb(obj);
	obj->print();

	TUnique<SObject> obj01 = {100, "Hello"};
	Parent* parent = obj01.get();

	parent->print();

	{
		TShared<SObject> obb = {100, "Hello2"};
		TShared<Parent> obb2 = obb.staticCast<Parent>();
		obb2->print();
	}

	{
		TShared<SObject> obb = {100, "Hello3"};
		TShared<Parent> obb2 = obb.staticCast<Parent>();
		TShared<SObject> obb3 = obb2.dynamicCast<SObject>();
		obb3->print();
	}

	DO_TEST(TVector)
	DO_TEST(TMaxHeap)
	DO_TEST(TMinHeap)
	DO_TEST(TDeque)
	DO_TEST(TList)
	DO_TEST(TForwardList)
	DO_ARRAY_TEST(TArray)
	DO_TEST(TStack)
	DO_TEST(TQueue)
	DO_TEST(TSet)
	DO_TEST(TMultiSet)
	DO_TEST(TPrioritySet)
	DO_TEST(TPriorityMultiSet)
	DO_MAP_TEST(TMap)
	DO_MAP_TEST(TMultiMap)
	DO_MAP_TEST(TPriorityMap)
	DO_MAP_TEST(TPriorityMultiMap)

	return 0;
}