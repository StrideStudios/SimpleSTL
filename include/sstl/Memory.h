#pragma once

#include <memory>

template <typename TType>
struct TUnique {

	TUnique(): m_ptr(std::make_unique<TType>()) {}

	template <typename... TArgs>
	TUnique(TArgs&&... args): m_ptr(std::make_unique<TType>(args...)) {}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TUnique(const TOtherType& otr): m_ptr(std::make_unique<TOtherType>(otr)) {}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TUnique(TOtherType&& otr): m_ptr(std::make_unique<TOtherType>(std::forward<TOtherType>(otr))) {}

	template <typename TOtherType>
	TUnique(const TUnique<TOtherType>&) = delete;

	template <typename TOtherType>
	TUnique(TUnique<TOtherType>&) = delete;

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TUnique(TUnique<TOtherType>&& otr): m_ptr(std::forward<std::unique_ptr<TOtherType>>(otr.m_ptr)) {}

	TUnique(TUnique&& otr): m_ptr(std::forward<std::unique_ptr<TType>>(otr.m_ptr)) {}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TUnique& operator=(const TOtherType& otr) noexcept {
		this->m_ptr = std::make_unique<TOtherType>(otr);
		return *this;
	}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TUnique& operator=(TOtherType&& otr) noexcept {
		this->m_ptr = std::make_unique<TOtherType>(std::forward<TOtherType>(otr));
		return *this;
	}

	template <typename TOtherType>
	TUnique& operator=(const TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType>
	TUnique& operator=(TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TUnique& operator=(TUnique<TOtherType>&& otr) noexcept {
		this->m_ptr = std::forward<std::unique_ptr<TOtherType>>(otr.m_ptr);
		return *this;
	}

	TUnique& operator=(TUnique&& otr) noexcept {
		this->m_ptr = std::forward<std::unique_ptr<TType>>(otr.m_ptr);
		return *this;
	}

	TType* operator->() const {
		return m_ptr.get();
	}

	TType* operator*() const {
		return m_ptr.get();
	}

	TType* get() { return m_ptr.get(); }

	const TType* get() const { return m_ptr.get(); }

	operator bool() const {
		return get() != nullptr;
	}

	friend bool operator<(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr < snd.m_ptr;
	}

	friend bool operator<=(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr <= snd.m_ptr;
	}

	friend bool operator>(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr > snd.m_ptr;
	}

	friend bool operator>=(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr >= snd.m_ptr;
	}

	friend bool operator==(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr == snd.m_ptr;
	}

	friend bool operator!=(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr != snd.m_ptr;
	}

	friend size_t getHash(const TUnique& obj) {
		return getHash(*obj.get());
	}

private:
	template <typename>
	friend struct TUnique;

	std::unique_ptr<TType> m_ptr;

};

template <typename TType>
struct TShared {

	TShared(): m_ptr(std::make_shared<TType>()) {}

	template <typename... TArgs>
	TShared(TArgs&&... args): m_ptr(std::make_shared<TType>(args...)) {}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TShared(const TOtherType& otr): m_ptr(std::make_shared<TOtherType>(otr)) {}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TShared(TOtherType&& otr): m_ptr(std::make_shared<TOtherType>(std::forward<TOtherType>(otr))) {}

	template <typename TOtherType>
	TShared(const TShared<TOtherType>&) = delete;

	template <typename TOtherType>
	TShared(TShared<TOtherType>&) = delete;

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TShared(TShared<TOtherType>&& otr): m_ptr(std::forward<std::shared_ptr<TOtherType>>(otr.m_ptr)) {}

	TShared(TShared&& otr): m_ptr(std::forward<std::shared_ptr<TType>>(otr.m_ptr)) {}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TShared& operator=(const TOtherType& otr) noexcept {
		this->m_ptr = std::make_shared<TOtherType>(otr);
		return *this;
	}

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TShared& operator=(TOtherType&& otr) noexcept {
		this->m_ptr = std::make_shared<TOtherType>(std::forward<TOtherType>(otr));
		return *this;
	}

	template <typename TOtherType>
	TShared& operator=(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType>
	TShared& operator=(TShared<TOtherType>& otr) = delete;

	template <typename TOtherType>
	requires std::is_base_of_v<TType, TOtherType>
	TShared& operator=(TShared<TOtherType>&& otr) noexcept {
		this->m_ptr = std::forward<std::shared_ptr<TOtherType>>(otr.m_ptr);
		return *this;
	}

	TShared& operator=(TShared&& otr) noexcept {
		this->m_ptr = std::forward<std::shared_ptr<TType>>(otr.m_ptr);
		return *this;
	}

	TType* operator->() const {
		return m_ptr.get();
	}

	TType* operator*() const {
		return m_ptr.get();
	}

	TType* get() { return m_ptr.get(); }

	const TType* get() const { return m_ptr.get(); }

	operator bool() const {
		return get() != nullptr;
	}

	friend bool operator<(const TShared& fst, const TShared& snd) {
		return fst.m_ptr < snd.m_ptr;
	}

	friend bool operator<=(const TShared& fst, const TShared& snd) {
		return fst.m_ptr <= snd.m_ptr;
	}

	friend bool operator>(const TShared& fst, const TShared& snd) {
		return fst.m_ptr > snd.m_ptr;
	}

	friend bool operator>=(const TShared& fst, const TShared& snd) {
		return fst.m_ptr >= snd.m_ptr;
	}

	friend bool operator==(const TShared& fst, const TShared& snd) {
		return fst.m_ptr == snd.m_ptr;
	}

	friend bool operator!=(const TShared& fst, const TShared& snd) {
		return fst.m_ptr != snd.m_ptr;
	}

	friend size_t getHash(const TShared& obj) {
		return getHash(*obj.get());
	}

private:

	template <typename>
	friend struct TShared;

	std::shared_ptr<TType> m_ptr;

	TShared(const TShared&) = delete;
	TShared& operator=(const TShared&) = delete;

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
TType* getUnfurled(TType* type) {
	return type;
}

template <typename TType>
const TType* getUnfurled(const TType* type) {
	return type;
}

template <typename TType>
TType* getUnfurled(TType& type) {
	return &type;
}

template <typename TType>
const TType* getUnfurled(const TType& type) {
	return &type;
}

template <typename TType>
TType* getUnfurled(TShared<TType>& type) {
	return type.get();
}

template <typename TType>
const TType* getUnfurled(const TShared<TType>& type) {
	return type.get();
}

template <typename TType>
TType* getUnfurled(TUnique<TType>& type) {
	return type.get();
}

template <typename TType>
const TType* getUnfurled(const TUnique<TType>& type) {
	return type.get();
}