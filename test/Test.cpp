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

struct Abstract {
	Abstract() = default;
	Abstract(const size_t id): id(id) {}
	virtual ~Abstract() = default;
	virtual void print() const = 0;

	friend bool operator<(const Abstract& fst, const Abstract& snd) {
		return fst.id < snd.id;
	}

	friend bool operator==(const Abstract& fst, const Abstract& snd) {
		return fst.id == snd.id;
	}

	friend size_t getHash(const Abstract& obj) {
		return obj.id;
	}

	int id = 0;
};

struct Parent : Abstract{
	Parent() = default;
	Parent(const size_t id): Abstract(id) {}
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

template <typename TType>
struct Test {

	Test() = default;

	template <typename... TArgs>
	Test(TArgs&&... args)
	: m_ptr(std::make_shared<TType>(std::forward<TArgs>(args)...)) {}

	std::shared_ptr<TType> m_ptr = nullptr;

};

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Abstract, typename TUnfurled<TType>::Type>
#endif
void containerTest(const std::string& containerName, TSequenceContainer<TType>& container) {

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i+500);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	SHUFFLE(vec, rng);

	container.resize(10, [&](const size_t index) {
		return TUnfurled<TType>::template create<SObject>(vec[index], containerName);
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t i = 0; i < size; ++i) {
		getUnfurled(container.top())->print();
		container.pop();
	}
	std::cout << std::endl;

	container.clear();

}


template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Abstract, typename TUnfurled<TType>::Type>
#endif
void transferTest(const std::string& containerName, TSequenceContainer<TType>& container) {

	{
		std::cout << "Vector Transfer Test" << std::endl;

		TVector<TType> from;
		from.push(TUnfurled<TType>::template create<SObject>(100, containerName));

		std::cout << "Pre Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});
		std::cout << "to:" << std::endl;
		container.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});

		assert(from.getSize() == 1);

		from.transfer(container, 0);

		std::cout << "Post Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});
		std::cout << "to:" << std::endl;
		container.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});
		std::cout << std::endl;

		assert(container.getSize() == 1);

		container.clear();
	}

	{
		std::cout << "List Transfer Test" << std::endl;

		TList<TType> from;
		from.push(TUnfurled<TType>::template create<SObject>(100, containerName));

		std::cout << "Pre Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});
		std::cout << "to:" << std::endl;
		container.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});

		assert(from.getSize() == 1);

		from.transfer(container, 0);

		std::cout << "Post Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});
		std::cout << "to:" << std::endl;
		container.forEach([](size_t index, const TType& obb) {getUnfurled(obb)->print();});
		std::cout << std::endl;

		assert(container.getSize() == 1);

		container.clear();
	}
}

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Abstract, typename TUnfurled<TType>::Type>
#endif
void containerTest(const std::string& containerName, TSingleAssociativeContainer<TType>& container) {

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	SHUFFLE(vec, rng);

	size_t i = 0;

	container.resize(10, [&] {
		auto object = TUnfurled<TType>::template create<SObject>(vec[i], containerName);
		++i;
		return object;
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t v = 0; v < size; ++v) {
		getUnfurled(container.top())->print();
		container.pop();
	}
	std::cout << std::endl;
}

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Abstract, typename TUnfurled<TType>::Type>
#endif
void transferTest(const std::string& containerName, TSingleAssociativeContainer<TType>& container) {

	{
		std::cout << "Set Transfer Test" << std::endl;

		TSet<TType> from;
		from.push(TUnfurled<TType>::template create<SObject>(100, containerName));

		std::cout << "Pre Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](const TType& obb) {getUnfurled(obb)->print();});
		std::cout << "to:" << std::endl;
		container.forEach([](const TType& obb) {getUnfurled(obb)->print();});

		assert(from.getSize() == 1);

		from.transfer(container, const_cast<TType&>(from.top()));

		std::cout << "Post Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](const TType& obb) {getUnfurled(obb)->print();});
		std::cout << "to:" << std::endl;
		container.forEach([](const TType& obb) {getUnfurled(obb)->print();});
		std::cout << std::endl;

		assert(container.getSize() == 1);

		container.clear();
	}
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

std::string enumToString(const MapEnum val) {
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
}

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Abstract, typename TUnfurled<TType>::Type>
#endif
void containerTest(const std::string& containerName, TAssociativeContainer<MapEnum, TType>& container) {

	std::vector<size_t> vec;
	for (size_t i = 0; i < 10; ++i) {
		vec.push_back(i);
	}

	std::random_device rd;
	std::mt19937 rng(rd());

	SHUFFLE(vec, rng);

	size_t i = 0;

	container.resize(10, [&] {
		auto pair = TPair<MapEnum, TType>{(MapEnum)vec[i], TUnfurled<TType>::template create<SObject>(vec[i], containerName)};
		++i;
		return pair;
	});
	assert(container.getSize() == 10);

	const size_t size = container.getSize();
	for (size_t v = 0; v < size; ++v) {
		MapEnum enm = (MapEnum)v;
		if (auto object = getUnfurled(container.get(enm))) {
			std::cout << "Key: " << enumToString(enm) << " ";
			object->print();
			container.pop(enm);
		}
	}

	std::cout << std::endl;
}

template <typename TType>
#if CXX_VERSION >= 20
requires std::is_base_of_v<Abstract, typename TUnfurled<TType>::Type>
#endif
void transferTest(const std::string& containerName, TAssociativeContainer<MapEnum, TType>& container) {

	{
		std::cout << "Map Transfer Test" << std::endl;

		TMap<MapEnum, TType> from;
		from.push(MapEnum::NONE, TUnfurled<TType>::template create<SObject>(100, containerName));

		std::cout << "Pre Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](const TPair<MapEnum, const TType&>& obb) {
			std::cout << "Key: " << enumToString(obb.key()) << " ";
			getUnfurled(obb.value())->print();
		});
		std::cout << "to:" << std::endl;
		container.forEach([](const TPair<MapEnum, const TType&>& obb) {
			std::cout << "Key: " << enumToString(obb.key()) << " ";
			getUnfurled(obb.value())->print();
		});

		assert(from.getSize() == 1);

		from.transfer(container, MapEnum::NONE);

		std::cout << "Post Transfer" << std::endl;
		std::cout << "from:" << std::endl;
		from.forEach([](const TPair<MapEnum, const TType&>& obb) {
			std::cout << "Key: " << enumToString(obb.key()) << " ";
			getUnfurled(obb.value())->print();
		});
		std::cout << "to:" << std::endl;
		container.forEach([](const TPair<MapEnum, const TType&>& obb) {
			std::cout << "Key: " << enumToString(obb.key()) << " ";
			getUnfurled(obb.value())->print();
		});
		std::cout << std::endl;

		assert(container.getSize() == 1);

		container.clear();
	}
}

#define DO_TEST(x) \
	{ x<Parent> container; containerTest(#x, container); transferTest(#x, container); } \
	{ x<TShared<Parent>> container; containerTest(#x " Shared", container); transferTest(#x " Shared", container); } \
	{ x<TUnique<Parent>> container; containerTest(#x " Unique", container); transferTest(#x " Unique", container); } \
	{ x<TUnique<Abstract>> container; containerTest(#x " Abstract Unique", container); transferTest(#x " Abstract Unique", container); }

#define DO_ARRAY_TEST(x) \
	{ x<Parent, 10> container; containerTest(#x, container); } \
	{ x<TShared<Parent>, 10> container; containerTest(#x " Shared", container); } \
	{ x<TUnique<Parent>, 10> container; containerTest(#x " Unique", container); }

#define DO_ASSOCIATIVE_TEST(x) \
	{ x<Parent> container; containerTest(#x, container); transferTest(#x, container); } \
	{ x<TShared<Parent>> container; containerTest(#x " Shared", container); transferTest(#x " Shared", container); } \
	{ x<TUnique<Parent>> container; containerTest(#x " Unique", container); transferTest(#x " Unique", container); } \
	{ x<TUnique<Abstract>> container; containerTest(#x " Abstract Unique", container); transferTest(#x " Abstract Unique", container); }

#define DO_MAP_TEST(x) \
	{ x<MapEnum, Parent> container; containerTest(#x, container); transferTest(#x, container); } \
	{ x<MapEnum, TShared<Parent>> container; containerTest(#x " Shared", container); transferTest(#x " Shared", container); } \
	{ x<MapEnum, TUnique<Parent>> container; containerTest(#x " Unique", container); transferTest(#x " Unique", container); } \
	{ x<MapEnum, TUnique<Abstract>> container; containerTest(#x " Abstract Unique", container); transferTest(#x " Abstract Unique", container); }

int main() {

	auto lamb = [](TUnique<Parent>& out) {
		out = TUnique<SObject>{200, "Hello"};
	};

	TUnique<Parent> obj;
	lamb(obj);
	obj->print();

	TUnique<Parent> obj01 = SObject{100, "Hello"};
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

	/*{
		TList<TUnique<SObject>> l1;
		TVector<TUnique<SObject>> l2;
		l1.push(SObject{5015, "List 1"});
		l2.push(SObject{5016, "Vector 2"});
		std::cout << "Pre Splice" << std::endl;
		std::cout << "L1:" << std::endl;
		l1.forEach([](size_t index, const TUnique<SObject>& obb) { obb->print(); });
		std::cout << "L2:" << std::endl;
		l2.forEach([](size_t index, const TUnique<SObject>& obb) { obb->print(); });
		std::cout << "Post Splice" << std::endl;
		l1.transfer(l2, 0);
		std::cout << "L1:" << std::endl;
		l1.forEach([](size_t index, const TUnique<SObject>& obb) { obb->print(); });
		std::cout << "L2:" << std::endl;
		l2.forEach([](size_t index, const TUnique<SObject>& obb) { obb->print(); });
	}*/

	DO_TEST(TVector)
	DO_TEST(TMaxHeap)
	DO_TEST(TMinHeap)
	DO_TEST(TDeque)
	DO_TEST(TList)
	DO_TEST(TForwardList)
	DO_ARRAY_TEST(TArray)
	DO_TEST(TStack)
	DO_TEST(TQueue)
	DO_ASSOCIATIVE_TEST(TSet)
	DO_ASSOCIATIVE_TEST(TMultiSet)
	DO_ASSOCIATIVE_TEST(TPrioritySet)
	DO_ASSOCIATIVE_TEST(TPriorityMultiSet)
	DO_MAP_TEST(TMap)
	DO_MAP_TEST(TMultiMap)
	DO_MAP_TEST(TPriorityMap)
	DO_MAP_TEST(TPriorityMultiMap)

	std::unique_ptr<Abstract> ab3 = std::make_unique<SObject>(SObject{100, "Hey"});

	TUnique<Abstract> abb = TUnique<SObject>{100, "Hey"};
	TUnique<SObject> abb2 = SObject{100, "Hey"};

	TPriorityMap<int, std::reference_wrapper<Abstract>> map;
	std::map<int, std::reference_wrapper<Abstract>> map2;
	std::vector<std::reference_wrapper<Abstract>> vec2;
	TVector<std::reference_wrapper<Abstract>> vec22;

	SObject obb = SObject{100, "Hey"};
	vec2.push_back(obb);

	std::vector<TUnique<SObject>> m_Container;
	m_Container.emplace_back(SObject{100, "Hello unique"});

	{
		TVector<TUnique<SObject>> vec;
		vec.push(SObject{100, "Hello unique"});
		SObject* obb = vec.top().get();

		if (vec.contains(obb)) {
			std::cout << "Found obb in vector" << std::endl;
		}
	}

	{
		TSet<TUnique<SObject>> set;
		set.push(SObject{100, "Hello unique"});
		SObject* obb = set.top().get();

		if (set.contains(obb)) {
			std::cout << "Found obb in set" << std::endl;
		}
	}

	Test<SObject> testObb = SObject{100, "Hello"};

	testObb.m_ptr->print();

	testObb = SObject{200, "Hello"};

	testObb.m_ptr->print();

	auto oj = SObject{300, "Hello"};
	testObb = oj;

	testObb.m_ptr->print();

	TShared<SObject> testSharedForWeak{100, "SObject"};

	std::cout << "Pre Weak Say: ";
	testSharedForWeak->print();

	TWeak<SObject> testWeak{testSharedForWeak};

	std::cout << "Weak Say: ";
	if (testWeak) {
		testWeak->print();
	} else {
		std::cout << "No Weak!";
	}

	TShared<SObject> testShared2{testWeak};

	std::cout << "Post Weak Say: ";
	testShared2->print();

	struct RequirePointer {
		RequirePointer() = default;
		RequirePointer(const TShared<SObject>& objectWithPointer) {
			std::cout << "Require pointer got pointer from: ";
			objectWithPointer->print();
		}
	};

	struct FromHelper : SObject, TSharedFrom<FromHelper> {

		using SObject::SObject;

		void init() {
			myFriend = getShared().staticCast<SObject>();
		}

	private:

		RequirePointer myFriend;
	};

	TShared<FromHelper> frend{100, "Hey"};
	frend->init();

	TSet<TUnique<SObject>> setObjs;
	setObjs.push(TUnique<SObject>{100, "Hello"});

	SObject* ptr = setObjs.top().get();

	std::cout << std::endl << std::endl;

	std::cout << "Pre pop: ";
	setObjs.forEach([](const TUnique<SObject>& obj) { obj->print(); });
	setObjs.pop(ptr);
	std::cout << "Post pop: ";
	setObjs.forEach([](const TUnique<SObject>& obj) { obj->print(); });

	std::cout << std::endl << std::endl;
	TSet<TUnique<SObject>> setObjs0;
	TSet<TUnique<SObject>> setObjs1;

	setObjs0.push(TUnique<SObject>{100, "Hello0"});
	SObject* transferPtr = setObjs0.top().get();
	std::cout << std::endl << std::endl;
	std::cout << "Pre set: ";

	std::cout << std::endl;
	std::cout << "obs0: ";
	setObjs0.forEach([](const TUnique<SObject>& obj) { obj->print(); });
	std::cout << std::endl;
	std::cout << "obs1: ";
	setObjs1.forEach([](const TUnique<SObject>& obj) { obj->print(); });

	setObjs0.transfer(setObjs1, transferPtr);

	std::cout << std::endl << std::endl;
	std::cout << "Post set: ";

	std::cout << std::endl;
	std::cout << "obs0: ";
	setObjs0.forEach([](const TUnique<SObject>& obj) { obj->print(); });
	std::cout << std::endl;
	std::cout << "obs1: ";
	setObjs1.forEach([](const TUnique<SObject>& obj) { obj->print(); });

	return 0;
}