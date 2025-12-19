#pragma once

#include <mutex>

#include "Memory.h"

// Thanks to Jonathan Wakely on Stack Exchange.
// https://stackoverflow.com/questions/16859519/how-to-wrap-calls-of-every-member-function-of-a-class-in-c11
template <typename TType>
class TThreadSafe {

	template<typename TParent, typename TMutex>
	struct lock : std::lock_guard<TMutex> {

		explicit lock(TParent* parent, TMutex& mtx) noexcept
		: std::lock_guard<TMutex>(mtx),
		  parent(parent) {}

		decltype(auto) operator->() const noexcept { return parent; }

		TParent* parent;
	};

public:

	/*
	 * Mutex cannot be copied nor moved, so we ignore it
	 */

	TThreadSafe() noexcept {}

	template <typename... TArgs,
		std::enable_if_t<std::is_constructible_v<TType, TArgs...>, int> = 0
	>
	TThreadSafe(TArgs&&... args) noexcept
	: m_obj(std::forward<TArgs>(args)...) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_constructible_v<TType, const TOtherType&>, int> = 0
	>
#if CXX_VERSION >= 20
    constexpr explicit(!std::is_convertible_v<const TOtherType&, TType>)
#endif
	TThreadSafe(const TOtherType& otr)
	noexcept(std::is_nothrow_constructible_v<TType, const TOtherType&>)
	: m_obj(otr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_constructible_v<TType, TOtherType&>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::is_convertible_v<TOtherType&, TType>)
#endif
	TThreadSafe(TOtherType& otr)
	noexcept(std::is_nothrow_constructible_v<TType, TOtherType&>)
	: m_obj(otr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_constructible_v<TType, TOtherType>, int> = 0
	>
#if CXX_VERSION >= 20
    constexpr explicit(!std::is_convertible_v<TOtherType, TType>)
#endif
	TThreadSafe(TOtherType&& otr)
	noexcept(std::is_constructible_v<TType, TOtherType>)
	: m_obj(std::forward<TOtherType>(otr)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_constructible_v<TType, const TOtherType&>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::is_convertible_v<const TOtherType&, TType>)
#endif
	TThreadSafe(const TThreadSafe<TOtherType>& otr)
	noexcept(std::is_constructible_v<TType, const TOtherType&>)
	: TThreadSafe(otr.m_obj) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_constructible_v<TType, TOtherType&>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::is_convertible_v<TOtherType&, TType>)
#endif
	TThreadSafe(TThreadSafe<TOtherType>& otr)
	noexcept(std::is_constructible_v<TType, TOtherType&>)
	: TThreadSafe(otr.m_obj) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_constructible_v<TType, TOtherType>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::is_convertible_v<TOtherType, TType>)
#endif
	TThreadSafe(TThreadSafe<TOtherType>&& otr)
	noexcept(std::is_constructible_v<TType, TOtherType>)
	: m_obj(std::forward<TOtherType>(otr.m_obj)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_assignable_v<TType&, const TOtherType&>, int> = 0
	>
	TThreadSafe& operator=(const TOtherType& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, const TOtherType&>) {
		this->m_obj = otr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_assignable_v<TType&, TOtherType&>, int> = 0
	>
	TThreadSafe& operator=(TOtherType& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType&>) {
		this->m_obj = otr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_assignable_v<TType&, TOtherType>, int> = 0
	>
	TThreadSafe& operator=(TOtherType&& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType>) {
		this->m_obj = std::forward<TOtherType>(otr);
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_assignable_v<TType&, const TOtherType&>, int> = 0
	>
	TThreadSafe& operator=(const TThreadSafe<TOtherType>& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, const TOtherType&>) {
		this->m_obj = otr.m_obj;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_assignable_v<TType&, TOtherType&>, int> = 0
	>
	TThreadSafe& operator=(TThreadSafe<TOtherType>& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType&>) {
		this->m_obj = otr.m_obj;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_assignable_v<TType&, TOtherType>, int> = 0
	>
	TThreadSafe& operator=(TThreadSafe<TOtherType>&& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType>) {
		this->m_obj = std::forward<TOtherType>(otr.m_obj);
		return *this;
	}

	decltype(auto) operator->() {
		if constexpr (TUnfurled<std::remove_reference_t<TType>>::isManaged) {
			return lock(m_obj.get(), mtx);
		} else {
			return lock(&m_obj, mtx);
		}
	}

	decltype(auto) operator->() const {
		if constexpr (TUnfurled<std::remove_reference_t<TType>>::isManaged) {
			return lock(m_obj.get(), mtx);
		} else {
			return lock(&m_obj, mtx);
		}
	}

	friend bool operator<(const TThreadSafe& fst, const TThreadSafe& snd) {
		return fst.m_obj < snd.m_obj;
	}

	friend bool operator<=(const TThreadSafe& fst, const TThreadSafe& snd) {
		return fst.m_obj <= snd.m_obj;
	}

	friend bool operator>(const TThreadSafe& fst, const TThreadSafe& snd) {
		return fst.m_obj > snd.m_obj;
	}

	friend bool operator>=(const TThreadSafe& fst, const TThreadSafe& snd) {
		return fst.m_obj >= snd.m_obj;
	}

	friend bool operator==(const TThreadSafe& fst, const TThreadSafe& snd) {
		return fst.m_obj == snd.m_obj;
	}

	friend bool operator!=(const TThreadSafe& fst, const TThreadSafe& snd) {
		return fst.m_obj != snd.m_obj;
	}

	friend size_t getHash(const TThreadSafe& obj) {
		return getHash(static_cast<const TType&>(obj));
	}

public:
	template <typename>
	friend class TThreadSafe;

	TType m_obj;

	mutable std::mutex mtx;
};