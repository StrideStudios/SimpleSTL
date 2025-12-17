#pragma once

#include <memory>

template <typename TType>
struct TUnique {

	TUnique(std::unique_ptr<TType>&& ptr) noexcept
	: m_ptr(std::forward<std::unique_ptr<TType>>(ptr)) {}

	TUnique() noexcept
	: m_ptr(std::make_unique<TType>()) {}

	TUnique(nullptr_t) noexcept {}

	template <typename... TArgs>
	TUnique(TArgs&&... args) noexcept
	: m_ptr(std::make_unique<TType>(args...)) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TUnique(const TOtherType& otr) noexcept
	: m_ptr(std::make_unique<TOtherType>(otr)) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TUnique(TOtherType&& otr) noexcept
	: m_ptr(std::make_unique<TOtherType>(std::forward<TOtherType>(otr))) {}

	template <typename TOtherType>
	TUnique(const TUnique<TOtherType>&) = delete;

	template <typename TOtherType>
	TUnique(TUnique<TOtherType>&) = delete;

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TUnique(TUnique<TOtherType>&& otr) noexcept
	: m_ptr(std::forward<std::unique_ptr<TOtherType>>(otr.m_ptr)) {}

	TUnique(TUnique&& otr) noexcept
	: m_ptr(std::forward<std::unique_ptr<TType>>(otr.m_ptr)) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TUnique& operator=(const TOtherType& otr) noexcept {
		this->m_ptr = std::make_unique<TOtherType>(otr);
		return *this;
	}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TUnique& operator=(TOtherType&& otr) noexcept {
		this->m_ptr = std::make_unique<TOtherType>(std::forward<TOtherType>(otr));
		return *this;
	}

	template <typename TOtherType>
	TUnique& operator=(const TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType>
	TUnique& operator=(TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
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

	TType* get() const { return m_ptr.get(); }

	operator bool() const {
		return static_cast<bool>(m_ptr);
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

	// Compare raw pointer
	friend bool operator==(const TUnique& fst, const void* snd) {
		return fst.m_ptr.get() == snd;
	}

	friend bool operator!=(const TUnique& fst, const TUnique& snd) {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator!=(const TUnique& fst, const void* snd) {
		return fst.m_ptr.get() != snd;
	}

	friend size_t getHash(const TUnique& obj) {
		return getHash(*obj.get());
	}

private:
	template <typename>
	friend struct TUnique;

	std::unique_ptr<TType> m_ptr = nullptr;

};

template <typename TType>
struct TShared {

	TShared(const std::shared_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	TShared(std::shared_ptr<TType>&& ptr) noexcept
	: m_ptr(std::forward<std::shared_ptr<TType>>(ptr)) {}

	TShared() noexcept
	: m_ptr(std::make_shared<TType>()) {}

	TShared(nullptr_t) noexcept {}

	template <typename... TArgs>
	TShared(TArgs&&... args) noexcept
	: m_ptr(std::make_shared<TType>(args...)) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TShared(const TOtherType& otr) noexcept
	: m_ptr(std::make_shared<TOtherType>(otr)) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TShared(TOtherType&& otr) noexcept
	: m_ptr(std::make_shared<TOtherType>(std::forward<TOtherType>(otr))) {}

	template <typename TOtherType>
	TShared(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType>
	TShared(TShared<TOtherType>& otr) = delete;

	TShared(const TShared& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	TShared(TShared& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TShared(TShared<TOtherType>&& otr) noexcept
	: m_ptr(std::forward<std::shared_ptr<TOtherType>>(otr.m_ptr)) {}

	TShared(TShared&& otr) noexcept
	: m_ptr(std::forward<std::shared_ptr<TType>>(otr.m_ptr)) {}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TShared& operator=(const TOtherType& otr) noexcept {
		this->m_ptr = std::make_shared<TOtherType>(otr);
		return *this;
	}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TShared& operator=(TOtherType&& otr) noexcept {
		this->m_ptr = std::make_shared<TOtherType>(std::forward<TOtherType>(otr));
		return *this;
	}

	template <typename TOtherType>
	TShared& operator=(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType>
	TShared& operator=(TShared<TOtherType>& otr) = delete;

	TShared& operator=(const TShared& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	TShared& operator=(TShared& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <typename TOtherType>
#if CXX_VERSION >= 20
	requires std::is_base_of_v<TType, TOtherType>
#endif
	TShared& operator=(TShared<TOtherType>&& otr) noexcept {
		this->m_ptr = std::forward<std::shared_ptr<TOtherType>>(otr.m_ptr);
		return *this;
	}

	TShared& operator=(TShared&& otr) noexcept {
		this->m_ptr = std::forward<std::shared_ptr<TType>>(otr.m_ptr);
		return *this;
	}

	template <typename TOtherType>
	TShared<TOtherType> staticCast() const {
		return TShared<TOtherType>{std::static_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	TShared<TOtherType> dynamicCast() const {
		return TShared<TOtherType>{std::dynamic_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	TShared<TOtherType> reinterpretCast() const {
		return TShared<TOtherType>{std::reinterpret_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	TShared<TOtherType> constCast() const {
		return TShared<TOtherType>{std::const_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	TType* operator->() const {
		return m_ptr.get();
	}

	TType* operator*() const {
		return m_ptr.get();
	}

	TType* get() const { return m_ptr.get(); }

	operator bool() const {
		return static_cast<bool>(m_ptr);
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

	// Compare raw pointer
	friend bool operator==(const TShared& fst, const void* snd) {
		return fst.m_ptr.get() == snd;
	}

	friend bool operator!=(const TShared& fst, const TShared& snd) {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator!=(const TShared& fst, const void* snd) {
		return fst.m_ptr.get() != snd;
	}

	friend size_t getHash(const TShared& obj) {
		return getHash(*obj.get());
	}

private:

	template <typename>
	friend struct TShared;

	std::shared_ptr<TType> m_ptr = nullptr;

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
	constexpr static auto get = &TShared<TType>::get;
};

template <typename TType>
struct TUnfurled<TUnique<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
	constexpr static auto get = &TUnique<TType>::get;
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