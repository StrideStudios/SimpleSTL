#pragma once

#include <functional>
#include <stdexcept>

#if CXX_VERSION >= 20
#include <algorithm>
#endif

#include "Pair.h"

#if CXX_VERSION >= 20
#define ASSOCIATIVE_CONTAINS(c, x) c.contains(x)
#define RANGES(func, c, x) std::ranges::func(c, x)
#define REMOVE_RANGES(c, x) RANGES(remove, c, x).begin()
#else
#define ASSOCIATIVE_CONTAINS(c, x) c.find(x) != c.end()
#define RANGES(func, c, x) std::func(c.begin(), c.end(), x)
#define REMOVE_RANGES(c, x) RANGES(remove, c, x)
#endif

#define FIND(c, x) RANGES(find, c, x)
#define DISTANCE(c, x) RANGES(distance, c.begin(), FIND(c, x))
#define CONTAINS(c, x) FIND(c, x) != c.end()
#define ERASE(c, x) c.erase(REMOVE_RANGES(c, x), c.end())

// Makes it easy to see if a function is guaranteed or not
#define GUARANTEED = 0;
#define NOT_GUARANTEED { throw std::runtime_error("Attempted Usage of unimplemented function in TContainer."); }

// A basic container of any amount of objects
// A size of 0 implies a dynamic array
template <typename TType, size_t TSize = 0>
struct TSequenceContainer {

	virtual ~TSequenceContainer() = default;

	// Returns the size of the container
	[[nodiscard]] virtual size_t getSize() const
		GUARANTEED

	// Gets the first element possible, or the 'top' of the container
	virtual TType& top()
		GUARANTEED
	// Gets the first element possible, or the 'top' of the container
	virtual const TType& top() const
		GUARANTEED

	// Gets the first element possible, or the 'top' of the container
	virtual TType& bottom()
		NOT_GUARANTEED
	// Gets the first element possible, or the 'top' of the container
	virtual const TType& bottom() const
		NOT_GUARANTEED

	// Checks if a certain index is contained within the container
	virtual bool contains(const size_t index) const {
		return index > 0 && index < getSize();
	}

	// Checks if a certain object is contained within the container
	virtual bool contains(const TType& obj) const
		GUARANTEED

	// Find a certain element in the container
	virtual size_t find(const TType& obj) const
		GUARANTEED

	// Get an element at a specified index
	// Note: Certain limited containers will ignore index and return top, ex: queue or stack
	virtual TType& get(size_t index)
		GUARANTEED
	// Get an element at a specified index
	// Note: Certain limited containers will ignore index and return top, ex: queue or stack
	virtual const TType& get(size_t index) const
		GUARANTEED

	virtual TType& operator[](const size_t index) { return get(index); }
	virtual const TType& operator[](const size_t index) const { return get(index); }

	// Fills container with n defaulted elements
	virtual void resize(size_t amt = TSize)
		GUARANTEED

	// Fills container with TType& elements with size TSize
	void resize(std::function<void(TType&)> func) {
		resize(TSize, func);
	}
	// Fills container with TType& elements with size amt
	virtual void resize(size_t amt, std::function<void(TType&, size_t)> func)
		GUARANTEED

	// Reserves memory for n elements
	virtual void reserve(size_t amt = TSize)
		NOT_GUARANTEED

	// Adds a defaulted element to the container
	virtual TType& push()
		GUARANTEED
	// Adds an element to the container, returning the index where it was added
	virtual size_t push(const TType& obj)
		GUARANTEED
	// Adds an element to the container, returning the index where it was added
	virtual size_t push(TType&& obj)
		GUARANTEED
	// Inserts an element at a specified index
	virtual void push(size_t index, const TType& obj)
		GUARANTEED
	// Inserts an element at a specified index
	virtual void push(size_t index, TType&& obj)
		GUARANTEED

	// Replaces an element at a specified index, and returns the original
	virtual void replace(size_t index, const TType&)
		GUARANTEED
	// Replaces an element at a specified index, and returns the original
	virtual void replace(size_t index, TType&&)
		GUARANTEED

	// Removes all elements from the container
	virtual void clear()
		GUARANTEED

	// Removes the topmost element from the container
	virtual void pop()
		GUARANTEED
	// Removes an element at the specified index
	virtual void pop(size_t index)
		GUARANTEED
	// Removes a certain object from the container
	virtual void pop(const TType& obj)
		GUARANTEED

	// Moves an object at index from this to container otr
	virtual void transfer(TSequenceContainer& otr, const size_t index) {
		// Prefer move, but copy if not available
		auto& obj = get(index);
		if constexpr (std::is_move_constructible_v<TType>) {
			otr.push(std::move(obj));
		} else {
			otr.push(obj);
		}
		pop(index);
	}

	// Iterates through each element
	virtual void forEach(const std::function<void(size_t, TType&)>& func)
		GUARANTEED
	// Iterates through each element, const version
	virtual void forEach(const std::function<void(size_t, const TType&)>& func) const
		GUARANTEED

	// Iterates through each element in reverse, not guaranteed because some containers cannot be iterated both ways, such as forward_list
	virtual void forEachReverse(const std::function<void(size_t, TType&)>& func)
		NOT_GUARANTEED
	// Iterates through each element in reverse, const version
	virtual void forEachReverse(const std::function<void(size_t, const TType&)>& func) const
		NOT_GUARANTEED
};

// Designed to be a container with a key for indexing
template <typename TKeyType, typename TValueType>
struct TAssociativeContainer {

	virtual ~TAssociativeContainer() = default;

	// Returns the size of the container
	[[nodiscard]] virtual size_t getSize() const
		GUARANTEED

	// Gets the first element possible, or the 'top' of the container
	virtual TPair<TKeyType, const TValueType&> top() const
		GUARANTEED

	// Gets the last element possible, or the 'bottom' of the container
	virtual TPair<TKeyType, const TValueType&> bottom() const
		GUARANTEED

	// Checks if a certain key is contained within the container
	virtual bool contains(const TKeyType& key) const
		GUARANTEED

	// Get an element at a specified key
	virtual TValueType& get(const TKeyType& key)
		GUARANTEED
	// Get an element at a specified key
	virtual const TValueType& get(const TKeyType& key) const
		GUARANTEED

	// Fills container with TType& elements with size amt
	virtual void resize(size_t amt, std::function<void(TPair<TKeyType, TValueType>&)> func)
		GUARANTEED

	// Reserves memory for n elements
	virtual void reserve(size_t amt)
		NOT_GUARANTEED

	// Adds a defaulted element to the container
	virtual TPair<TKeyType, const TValueType&> push()
		GUARANTEED
	// Adds a defaulted element at key to the container
	virtual TValueType& push(const TKeyType& key)
		GUARANTEED
	// Adds an element value at key to the container
	virtual TValueType& push(const TKeyType& key, const TValueType& value)
		GUARANTEED
	// Adds an element value at key to the container
	virtual TValueType& push(const TKeyType& key, TValueType&& value)
		GUARANTEED
	// Adds an element to the container
	virtual void push(const TPair<TKeyType, TValueType>& pair)
		GUARANTEED
	// Adds an element to the container
	virtual void push(TPair<TKeyType, TValueType>&& pair)
		GUARANTEED
	// Replaces a specified element at key with another element
	virtual void replace(const TKeyType& key, const TValueType& obj)
		GUARANTEED
	// Replaces a specified element at key with another element
	virtual void replace(const TKeyType& key, TValueType&& obj)
		GUARANTEED

	// Removes all elements from the container
	virtual void clear()
		GUARANTEED

	// Removes the topmost element from the container
	virtual void pop()
		GUARANTEED
	// Removes an element at key from the container
	virtual void pop(const TKeyType& key)
		GUARANTEED

	// Moves an object at key from this to container otr
	virtual void transfer(TAssociativeContainer& otr, const TKeyType& key)
		GUARANTEED

	// Iterates through each element (Maps do not support reverse iteration)
	virtual void forEach(const std::function<void(TPair<TKeyType, const TValueType&>)>& func) const
		GUARANTEED
};

// Designed to be a container without indexing
template <typename TType>
struct TSingleAssociativeContainer {

	virtual ~TSingleAssociativeContainer() = default;

	// Returns the size of the container
	[[nodiscard]] virtual size_t getSize() const
		GUARANTEED

	// Gets the first element possible, or the 'top' of the container
	virtual const TType& top() const
		GUARANTEED

	// Gets the last element possible, or the 'bottom' of the container
	virtual const TType& bottom() const
		GUARANTEED

	// Checks if a certain index is contained within the container
	virtual bool contains(const size_t index) const {
		return index > 0 && index < getSize();
	}

	// Checks if a certain object is contained within the container
	virtual bool contains(const TType& obj) const
		GUARANTEED

	// Fills container with n defaulted elements
	virtual void resize(size_t amt)
		GUARANTEED

	// Fills container with TType& elements with size amt
	virtual void resize(size_t amt, std::function<void(TType&)> func)
		GUARANTEED

	// Reserves memory for n elements
	virtual void reserve(size_t amt)
		NOT_GUARANTEED

	// Adds a defaulted element to the container
	virtual const TType& push()
		GUARANTEED
	// Adds an element to the container
	virtual void push(const TType& obj)
		GUARANTEED
	// Adds an element to the container
	virtual void push(TType&& obj)
		GUARANTEED

	// Replaces a specified element with another element
	virtual void replace(const TType&, const TType&)
		GUARANTEED
	// Replaces a specified element with another element
	virtual void replace(const TType&, TType&&)
		GUARANTEED

	// Removes all elements from the container
	virtual void clear()
		GUARANTEED

	// Removes the topmost element from the container
	virtual void pop()
		GUARANTEED
	// Removes an element from the container
	virtual void pop(const TType&)
		GUARANTEED

	// Moves an object from this to container otr
	// Has to be overridden due to object lifetimes and extraction
	virtual void transfer(TSingleAssociativeContainer& otr, TType& obj)
		GUARANTEED

	// Iterates through each element
	virtual void forEach(const std::function<void(const TType&)>& func) const
		GUARANTEED
};