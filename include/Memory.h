#pragma once

#include <memory>

template <typename TType>
struct TUnique {

	TUnique(): m_ptr(std::make_unique<TType>()) {}

	TUnique(const TType& inValue): m_ptr(std::make_unique<TType>(inValue)) {}

	template <typename... TArgs>
	requires std::is_constructible_v<TType, TArgs...>
	TUnique(const TArgs... args): m_ptr(std::make_unique<TType>(args...)) {}

	TType* operator->() const {
		return m_ptr.get();
	}

	TType* operator*() const {
		return m_ptr.get();
	}

	TType* get() { return m_ptr.get(); }

	const TType* get() const { return m_ptr.get(); }

	friend bool operator<(const TUnique& fst, const TUnique& snd) {
		return *fst.get() < *snd.get();
	}

	friend bool operator==(const TUnique& fst, const TUnique& snd) {
		return *fst.get() == *snd.get();
	}

	friend size_t getHash(const TUnique& obj) {
		return getHash(*obj.get());
	}

private:

	std::unique_ptr<TType> m_ptr;

};

template <typename TType>
struct TShared {

	TShared(): m_ptr(std::make_shared<TType>()) {}

	TShared(const TType& inValue): m_ptr(std::make_shared<TType>(inValue)) {}

	template <typename... TArgs>
	requires std::is_constructible_v<TType, TArgs...>
	TShared(const TArgs... args): m_ptr(std::make_shared<TType>(args...)) {}

	TType* operator->() const {
		return m_ptr.get();
	}

	TType* operator*() const {
		return m_ptr.get();
	}

	TType* get() { return m_ptr.get(); }

	const TType* get() const { return m_ptr.get(); }

	friend bool operator<(const TShared& fst, const TShared& snd) {
		return *fst.get() < *snd.get();
	}

	friend bool operator==(const TShared& fst, const TShared& snd) {
		return *fst.get() == *snd.get();
	}

	friend size_t getHash(const TShared& obj) {
		return getHash(*obj.get());
	}

private:

	std::shared_ptr<TType> m_ptr;

};

template <typename TType>
struct TUnfurled {
	using Type = TType;
	constexpr static bool isManaged = false;
};

template <typename TType>
struct TUnfurled<TShared<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
};

template <typename TType>
struct TUnfurled<TUnique<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
};

template <typename TType>
TUnfurled<TType>::Type* getUnfurled(TType* type) {
	if constexpr (TUnfurled<TType>::isManaged) {
		return type->get();
	} else {
		return type;
	}
}

template <typename TType>
const TUnfurled<TType>::Type* getUnfurled(const TType* type) {
	if constexpr (TUnfurled<TType>::isManaged) {
		return type->get();
	} else {
		return type;
	}
}

template <typename TType>
TUnfurled<TType>::Type* getUnfurled(TType& type) {
	if constexpr (TUnfurled<TType>::isManaged) {
		return type.get();
	} else {
		return &type;
	}
}

template <typename TType>
const TUnfurled<TType>::Type* getUnfurled(const TType& type) {
	if constexpr (TUnfurled<TType>::isManaged) {
		return type.get();
	} else {
		return &type;
	}
}