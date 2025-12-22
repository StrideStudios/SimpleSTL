#pragma once

#include <memory>

// When the template is instantiated, it always evaluates to that value, regardless of completion
// So for c++20 we can use concepts, otherwise we ignore this requirement
#if CXX_VERSION > 20
template <typename TType>
concept Complete = requires { sizeof(TType) > 0; };
#define COMPLETE_TYPE Complete
#else
#define COMPLETE_TYPE typename
#endif

template <typename TType>
struct TUnique {

	TUnique(std::unique_ptr<TType>&& ptr) noexcept
	: m_ptr(std::forward<std::unique_ptr<TType>>(ptr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_default_constructible<TOtherType>>, int> = 0
	>
	TUnique()
	noexcept(std::is_nothrow_default_constructible_v<TOtherType>)
	: m_ptr(std::make_unique<TType>()) {}

	// If incomplete type or no default constructor is available for TType, we default to nullptr
	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::disjunction_v<std::negation<std::is_default_constructible<TOtherType>>>, int> = 0
	>
	TUnique() noexcept {}

	TUnique(nullptr_t) noexcept {}

	TUnique& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType, typename... TArgs,
		std::enable_if_t<std::conjunction_v<std::negation<std::is_null_pointer<TArgs>>..., std::is_constructible<TOtherType, TArgs...>>, int> = 0
	>
	TUnique(TArgs&&... args)
	noexcept(std::is_nothrow_constructible_v<TType, TArgs...>)
	: m_ptr(std::make_unique<TType>(std::forward<TArgs>(args)...)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_constructible<std::unique_ptr<TType>, TOtherType*>>, int> = 0
	>
	TUnique(const TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_constructible_v<std::unique_ptr<TType>, TOtherType*>)
	: m_ptr(std::make_unique<TOtherType>(otr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_constructible<std::unique_ptr<TType>, TOtherType*>>, int> = 0
	>
	TUnique(TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_constructible_v<std::unique_ptr<TType>, TOtherType*>)
	: m_ptr(std::make_unique<TOtherType>(otr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<TOtherType>, std::is_constructible<std::unique_ptr<TType>, TOtherType*>>, int> = 0
	>
	TUnique(TOtherType&& otr)
	noexcept(std::is_nothrow_move_constructible_v<TOtherType> && std::is_nothrow_constructible_v<std::unique_ptr<TType>, TOtherType*>)
	: m_ptr(std::make_unique<TOtherType>(std::forward<TOtherType>(otr))) {}

	template <COMPLETE_TYPE TOtherType = TType>
	TUnique(const TUnique<TOtherType>&) = delete;

	template <COMPLETE_TYPE TOtherType = TType>
	TUnique(TUnique<TOtherType>&) = delete;

	template <COMPLETE_TYPE TOtherType = TType>
	TUnique(TUnique<TOtherType>&& otr)
	: m_ptr(std::forward<std::unique_ptr<TOtherType>>(otr.m_ptr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_assignable<std::unique_ptr<TType>, TOtherType*>>, int> = 0
	>
	TUnique& operator=(const TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_assignable_v<std::unique_ptr<TType>, TOtherType*>) {
		this->m_ptr = std::unique_ptr<TType>(std::make_unique<TOtherType>(otr));
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_assignable<std::unique_ptr<TType>, TOtherType*>>, int> = 0
	>
	TUnique& operator=(TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_assignable_v<std::unique_ptr<TType>, TOtherType*>) {
		this->m_ptr = std::unique_ptr<TType>(std::make_unique<TOtherType>(otr));
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<TOtherType>, std::is_assignable<std::unique_ptr<TType>, TOtherType*>>, int> = 0
	>
	TUnique& operator=(TOtherType&& otr)
	noexcept(std::is_nothrow_move_constructible_v<TOtherType> && std::is_nothrow_assignable_v<std::unique_ptr<TType>, TOtherType*>) {
		this->m_ptr = std::unique_ptr<TType>(std::make_unique<TOtherType>(std::forward<TOtherType>(otr)));
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType>
	TUnique& operator=(const TUnique<TOtherType>& otr) = delete;

	template <COMPLETE_TYPE TOtherType = TType>
	TUnique& operator=(TUnique<TOtherType>& otr) = delete;

	template <COMPLETE_TYPE TOtherType = TType>
	TUnique& operator=(TUnique<TOtherType>&& otr) {
		this->m_ptr = std::forward<std::unique_ptr<TOtherType>>(otr.m_ptr);
		return *this;
	}

	template <COMPLETE_TYPE TOtherType>
	TOtherType* staticCast() const {
		return static_cast<TOtherType*>(this->m_ptr.get());
	}

	template <COMPLETE_TYPE TOtherType>
	TOtherType* dynamicCast() const {
		return dynamic_cast<TOtherType*>(this->m_ptr.get());
	}

	template <COMPLETE_TYPE TOtherType>
	TOtherType* reinterpretCast() const {
		return reinterpret_cast<TOtherType*>(this->m_ptr.get());
	}

	template <COMPLETE_TYPE TOtherType>
	TOtherType* constCast() const {
		return const_cast<TOtherType*>(this->m_ptr.get());
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

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_default_constructible<TOtherType>>, int> = 0
	>
	TShared()
	noexcept(std::is_nothrow_default_constructible_v<TOtherType>)
	: m_ptr(std::make_shared<TType>()) {}

	// If incomplete type or no default constructor is available for TType, we default to nullptr
	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::disjunction_v<std::negation<std::is_default_constructible<TOtherType>>>, int> = 0
	>
	TShared() noexcept {}

	TShared(nullptr_t) noexcept {}

	TShared& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType, typename... TArgs,
		std::enable_if_t<std::conjunction_v<std::negation<std::is_null_pointer<TArgs>>..., std::is_constructible<TOtherType, TArgs...>>, int> = 0
	>
	TShared(TArgs&&... args)
	: m_ptr(std::make_shared<TType>(std::forward<TArgs>(args)...)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_constructible<std::shared_ptr<TType>, TOtherType*>>, int> = 0
	>
	TShared(const TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_constructible_v<std::shared_ptr<TType>, TOtherType*>)
	: m_ptr(std::make_shared<TOtherType>(otr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_constructible<std::shared_ptr<TType>, TOtherType*>>, int> = 0
	>
	TShared(TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_constructible_v<std::shared_ptr<TType>, TOtherType*>)
	: m_ptr(std::make_shared<TOtherType>(otr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<TOtherType>, std::is_constructible<std::shared_ptr<TType>, TOtherType*>>, int> = 0
	>
	TShared(TOtherType&& otr)
	noexcept(std::is_nothrow_move_constructible_v<TOtherType> && std::is_nothrow_constructible_v<std::shared_ptr<TType>, TOtherType*>)
	: m_ptr(std::make_shared<TOtherType>(std::forward<TOtherType>(otr))) {}

	template <COMPLETE_TYPE TOtherType = TType>
	TShared(const TShared<TOtherType>& otr) = delete;

	template <COMPLETE_TYPE TOtherType = TType>
	TShared(TShared<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	template <COMPLETE_TYPE TOtherType = TType>
	TShared(const TShared& otr)
	: m_ptr(otr.m_ptr) {}

	template <COMPLETE_TYPE TOtherType = TType>
	TShared(TShared& otr)
	: m_ptr(otr.m_ptr) {}

	template <COMPLETE_TYPE TOtherType = TType>
	TShared(TShared<TOtherType>&& otr)
	: m_ptr(std::forward<std::shared_ptr<TOtherType>>(otr.m_ptr)) {}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_assignable<std::shared_ptr<TType>, TOtherType*>>, int> = 0
	>
	TShared& operator=(const TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_assignable_v<std::shared_ptr<TType>, TOtherType*>) {
		this->m_ptr = std::make_shared<TOtherType>(otr);
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherType>, std::is_assignable<std::shared_ptr<TType>, TOtherType*>>, int> = 0
	>
	TShared& operator=(TOtherType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TOtherType> && std::is_nothrow_assignable_v<std::shared_ptr<TType>, TOtherType*>) {
		this->m_ptr = std::make_shared<TOtherType>(otr);
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType,
		std::enable_if_t<std::conjunction_v<std::is_move_constructible<TOtherType>, std::is_assignable<std::shared_ptr<TType>, TOtherType*>>, int> = 0
	>
	TShared& operator=(TOtherType&& otr)
	noexcept(std::is_nothrow_move_constructible_v<TOtherType> && std::is_nothrow_assignable_v<std::shared_ptr<TType>, TOtherType*>) {
		this->m_ptr = std::make_shared<TOtherType>(std::forward<TOtherType>(otr));
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType>
	TShared& operator=(const TShared<TOtherType>& otr) = delete;

	template <COMPLETE_TYPE TOtherType = TType>
	TShared& operator=(TShared<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	template <COMPLETE_TYPE TOtherType = TType>
	TShared& operator=(const TShared& otr) {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType>
	TShared& operator=(TShared& otr) {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <COMPLETE_TYPE TOtherType = TType>
	TShared& operator=(TShared<TOtherType>&& otr) {
		this->m_ptr = std::forward<std::shared_ptr<TOtherType>>(otr.m_ptr);
		return *this;
	}

	template <COMPLETE_TYPE TOtherType>
	TShared<TOtherType> staticCast() const {
		return TShared<TOtherType>{std::static_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <COMPLETE_TYPE TOtherType>
	TShared<TOtherType> dynamicCast() const {
		return TShared<TOtherType>{std::dynamic_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <COMPLETE_TYPE TOtherType>
	TShared<TOtherType> reinterpretCast() const {
		return TShared<TOtherType>{std::reinterpret_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <COMPLETE_TYPE TOtherType>
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
struct TUnfurled<TType*> {
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
struct is_managed : std::bool_constant<TUnfurled<TType>::isManaged> {
	// Determine whether TType is a managed pointer or not
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