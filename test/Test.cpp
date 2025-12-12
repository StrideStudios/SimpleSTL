#include <format>
#include <iostream>
#include <random>
#include <utility>

#include "Array.h"
#include "Deque.h"
#include "ForwardList.h"
#include "Vector.h"
#include "MaxHeap.h"
#include "MinHeap.h"
#include "List.h"
#include "MultiSet.h"
#include "PriorityMultiSet.h"
#include "Queue.h"
#include "Set.h"
#include "PrioritySet.h"
#include "Stack.h"
#include "Map.h"
#include "Memory.h"
#include "PriorityMap.h"
#include "MultiMap.h"
#include "PriorityMultiMap.h"

struct SObject {

	SObject() = default;
	SObject(size_t id, std::string name): id(id), name(std::move(name)) {}

	int id = 0;
	std::string name = "None";

	void print() const {
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
requires std::is_base_of_v<SObject, typename TUnfurled<TType>::Type>
void containerTest(const std::string& containerName, TSequenceContainer<TType, TSize>& container) {

	container.push(SObject{100, "Hello"});

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	std::ranges::shuffle(vec, rng);

	container.resize(10, [&](TType& obj, const size_t index) {
		obj = SObject{vec[index], containerName};
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t i = 0; i < size; ++i) {
		if (auto obj = getUnfurled(container.top())) {
			obj->print();
			container.pop();
		}
	}
	std::cout << std::endl;
}

template <typename TType>
requires std::is_base_of_v<SObject, typename TUnfurled<TType>::Type>
void containerTest(const std::string& containerName, TSingleAssociativeContainer<TType>& container) {

	container.push(SObject{100, "Hello"});

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	std::ranges::shuffle(vec, rng);

	size_t i = 0;

	container.resize(10, [&](TType& obj) {
		obj = SObject{vec[i], containerName};
		++i;
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t v = 0; v < size; ++v) {
		if (auto obj = getUnfurled(container.top())) {
			obj->print();
			container.pop();
		}
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
requires std::is_base_of_v<SObject, typename TUnfurled<TType>::Type>
void containerTest(const std::string& containerName, TAssociativeContainer<MapEnum, TType>& container) {

	container.push(MapEnum::NONE, SObject{100, "Hello"});

	std::vector<size_t> vec;
	for (size_t i = 1; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	std::ranges::shuffle(vec, rng);

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
		if (auto obj = getUnfurled(container.get(enm))) {
			std::cout << "Key: " << EnumToString(enm) << " ";
			obj->print();
			container.pop(enm);
		}
	}

	std::cout << std::endl;
}

#define DO_TEST(x) \
	{ x<SObject> container; containerTest(#x, container); } \
	{ x<TShared<SObject>> container; containerTest(#x " Shared", container); } \
	{ x<TUnique<SObject>> container; containerTest(#x " Unique", container); }


#define DO_ARRAY_TEST(x) \
	{ x<SObject, 10> container; containerTest(#x, container); } \
	{ x<TShared<SObject>, 10> container; containerTest(#x " Shared", container); } \
	{ x<TUnique<SObject>, 10> container; containerTest(#x " Unique", container); }

#define DO_MAP_TEST(x) \
	{ x<MapEnum, SObject> container; containerTest(#x, container); } \
	{ x<MapEnum, TShared<SObject>> container; containerTest(#x " Shared", container); } \
	{ x<MapEnum, TUnique<SObject>> container; containerTest(#x " Unique", container); }

int main() {

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