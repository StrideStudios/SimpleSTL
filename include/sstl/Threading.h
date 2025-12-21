#pragma once

#include <mutex>
#include <functional>

#include "Memory.h"

// Thanks to Jonathan Wakely on Stack Exchange.
// https://stackoverflow.com/questions/16859519/how-to-wrap-calls-of-every-member-function-of-a-class-in-c11
template <typename TType>
class TThreadSafe {

	template<typename TParent, typename TMutex>
	struct safe_lock : std::lock_guard<TMutex> {

		explicit safe_lock(TParent* parent, TMutex& mtx) noexcept
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
		std::enable_if_t<std::conjunction_v<is_complete<TType>, std::is_constructible<TType, TArgs...>>, int> = 0
	>
	TThreadSafe(TArgs&&... args)
	noexcept(std::is_nothrow_constructible_v<TType, TArgs...>)
	: m_obj(std::forward<TArgs>(args)...) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_constructible<TType, const TOtherType&>>, int> = 0
	>
	TThreadSafe(const TOtherType& otr)
	noexcept(std::is_nothrow_constructible_v<TType, const TOtherType&>)
	: m_obj(otr) {}

	TThreadSafe(const TType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TType>)
	: m_obj(otr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_constructible<TType, TOtherType&>>, int> = 0
	>
	TThreadSafe(TOtherType& otr)
	noexcept(std::is_nothrow_constructible_v<TType, TOtherType&>)
	: m_obj(otr) {}

	TThreadSafe(TType& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TType>)
	: m_obj(otr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_constructible<TType, TOtherType>>, int> = 0
	>
	TThreadSafe(TOtherType&& otr)
	noexcept(std::is_nothrow_constructible_v<TType, TOtherType>)
	: m_obj(std::forward<TOtherType>(otr)) {}

	TThreadSafe(TType&& otr)
	noexcept(std::is_nothrow_move_constructible_v<TType>)
	: m_obj(std::forward<TType>(otr)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_constructible<TType, const TOtherType&>>, int> = 0
	>
	TThreadSafe(const TThreadSafe<TOtherType>& otr)
	noexcept(std::is_nothrow_constructible_v<TType, const TOtherType&>)
	: TThreadSafe(otr.m_obj) {}

	TThreadSafe(const TThreadSafe& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TType>)
	: TThreadSafe(otr.m_obj) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_constructible<TType, TOtherType&>>, int> = 0
	>
	TThreadSafe(TThreadSafe<TOtherType>& otr)
	noexcept(std::is_nothrow_constructible_v<TType, TOtherType&>)
	: TThreadSafe(otr.m_obj) {}

	TThreadSafe(TThreadSafe& otr)
	noexcept(std::is_nothrow_copy_constructible_v<TType>)
	: TThreadSafe(otr.m_obj) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_constructible<TType, TOtherType>>, int> = 0
	>
	TThreadSafe(TThreadSafe<TOtherType>&& otr)
	noexcept(std::is_nothrow_constructible_v<TType, TOtherType>)
	: TThreadSafe(std::forward<TOtherType>(otr.m_obj)) {}

	TThreadSafe(TThreadSafe&& otr)
	noexcept(std::is_nothrow_move_constructible_v<TType>)
	: TThreadSafe(std::forward<TType>(otr.m_obj)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_assignable<TType&, const TOtherType&>>, int> = 0
	>
	TThreadSafe& operator=(const TOtherType& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, const TOtherType&>) {
		this->m_obj = otr;
		return *this;
	}

	TThreadSafe& operator=(const TType& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, const TType&>) {
		this->m_obj = otr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_assignable<TType&, TOtherType&>>, int> = 0
	>
	TThreadSafe& operator=(TOtherType& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType&>) {
		this->m_obj = otr;
		return *this;
	}

	TThreadSafe& operator=(TType& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TType&>) {
		this->m_obj = otr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_assignable<TType&, TOtherType>>, int> = 0
	>
	TThreadSafe& operator=(TOtherType&& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType>) {
		this->m_obj = std::forward<TOtherType>(otr);
		return *this;
	}

	TThreadSafe& operator=(TType&& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TType>) {
		this->m_obj = std::forward<TType>(otr);
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_assignable<TType&, const TOtherType&>>, int> = 0
	>
	TThreadSafe& operator=(const TThreadSafe<TOtherType>& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, const TOtherType&>) {
		this->m_obj = otr.m_obj;
		return *this;
	}

	TThreadSafe& operator=(const TThreadSafe& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, const TType&>) {
		this->m_obj = otr.m_obj;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_assignable<TType&, TOtherType&>>, int> = 0
	>
	TThreadSafe& operator=(TThreadSafe<TOtherType>& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType&>) {
		this->m_obj = otr.m_obj;
		return *this;
	}

	TThreadSafe& operator=(TThreadSafe& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TType&>) {
		this->m_obj = otr.m_obj;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::conjunction_v<is_complete<TOtherType>, std::is_assignable<TType&, TOtherType>>, int> = 0
	>
	TThreadSafe& operator=(TThreadSafe<TOtherType>&& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TOtherType>) {
		this->m_obj = std::forward<TOtherType>(otr.m_obj);
		return *this;
	}

	TThreadSafe& operator=(TThreadSafe&& otr)
	noexcept(std::is_nothrow_assignable_v<TType&, TType>) {
		this->m_obj = std::forward<TType>(otr.m_obj);
		return *this;
	}

	void lockFor(const std::function<void(TType&)>& func) {
		std::lock_guard lock(mtx);
		func(m_obj);
	}

	decltype(auto) operator->() {
		if constexpr (TUnfurled<std::remove_reference_t<TType>>::isManaged) {
			return safe_lock(m_obj.get(), mtx);
		} else {
			return safe_lock(&m_obj, mtx);
		}
	}

	decltype(auto) operator->() const {
		if constexpr (TUnfurled<std::remove_reference_t<TType>>::isManaged) {
			return safe_lock(m_obj.get(), mtx);
		} else {
			return safe_lock(&m_obj, mtx);
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

private:
	template <typename>
	friend class TThreadSafe;

	TType m_obj;

	mutable std::recursive_mutex mtx;
};