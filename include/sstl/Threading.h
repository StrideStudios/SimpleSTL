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

		explicit safe_lock(TParent* parent, TMutex& mtx) noexcept(false)
		: std::lock_guard<TMutex>(mtx),
		  parent(parent) {}

		decltype(auto) operator->() const noexcept { return parent; }

		TParent* parent;
	};

public:

	/*
	 * Mutex cannot be copied nor moved, so we ignore it
	 */

	TThreadSafe() noexcept {
		if constexpr (sstl::is_initializable_v<TType>) {
			m_obj.init();
		}
	}

	template <typename... TArgs,
		std::enable_if_t<
			std::conjunction_v<
				std::negation<std::is_null_pointer<std::decay_t<TArgs>>>...,
				std::negation<std::is_same<std::decay_t<TArgs>, TThreadSafe>>...
			>,
			int> = 0
	>
	explicit TThreadSafe(TArgs&&... args) noexcept {
		m_obj = TType{std::forward<TArgs>(args)...};
		if constexpr (sstl::is_initializable_v<TType>) {
			m_obj.init();
		}
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TThreadSafe(const TThreadSafe<TOtherType>& otr) noexcept
	: TThreadSafe(otr.m_obj) {}

	TThreadSafe(const TThreadSafe& otr) noexcept
	: TThreadSafe(otr.m_obj) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TThreadSafe(TThreadSafe<TOtherType>& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: TThreadSafe(otr.m_obj) {}

	TThreadSafe(TThreadSafe& otr) noexcept
	: TThreadSafe(otr.m_obj) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TThreadSafe(TThreadSafe<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: TThreadSafe(std::move(otr.m_obj)) {}

	TThreadSafe(TThreadSafe&& otr) noexcept
	: TThreadSafe(std::move(otr.m_obj)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TThreadSafe& operator=(const TThreadSafe<TOtherType>& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		this->m_obj = otr.m_obj;
		return *this;
	}

	TThreadSafe& operator=(const TThreadSafe& otr) noexcept {
		this->m_obj = otr.m_obj;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TThreadSafe& operator=(TThreadSafe<TOtherType>& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		this->m_obj = otr.m_obj;
		return *this;
	}

	TThreadSafe& operator=(TThreadSafe& otr) noexcept {
		this->m_obj = otr.m_obj;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TThreadSafe& operator=(TThreadSafe<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		this->m_obj = std::move(otr.m_obj);
		return *this;
	}

	TThreadSafe& operator=(TThreadSafe&& otr) noexcept {
		this->m_obj = std::move(otr.m_obj);
		return *this;
	}

	void lockFor(const std::function<void(TType&)>& func) noexcept(false) {
		std::lock_guard lock(mtx);
		func(m_obj);
	}

	decltype(auto) operator->() noexcept(false) {
		if constexpr (TUnfurled<std::remove_reference_t<TType>>::isManaged) {
			return safe_lock(m_obj.get(), mtx);
		} else {
			return safe_lock(&m_obj, mtx);
		}
	}

	decltype(auto) operator->() const noexcept(false) {
		if constexpr (TUnfurled<std::remove_reference_t<TType>>::isManaged) {
			return safe_lock(m_obj.get(), mtx);
		} else {
			return safe_lock(&m_obj, mtx);
		}
	}

	friend bool operator<(const TThreadSafe& fst, const TThreadSafe& snd) noexcept {
		return fst.m_obj < snd.m_obj;
	}

	friend bool operator<=(const TThreadSafe& fst, const TThreadSafe& snd) noexcept {
		return fst.m_obj <= snd.m_obj;
	}

	friend bool operator>(const TThreadSafe& fst, const TThreadSafe& snd) noexcept {
		return fst.m_obj > snd.m_obj;
	}

	friend bool operator>=(const TThreadSafe& fst, const TThreadSafe& snd) noexcept {
		return fst.m_obj >= snd.m_obj;
	}

	friend bool operator==(const TThreadSafe& fst, const TThreadSafe& snd) noexcept {
		return fst.m_obj == snd.m_obj;
	}

	friend bool operator!=(const TThreadSafe& fst, const TThreadSafe& snd) noexcept {
		return fst.m_obj != snd.m_obj;
	}

	friend size_t getHash(const TThreadSafe& obj) noexcept {
		return getHash(static_cast<const TType&>(obj));
	}

private:
	template <typename>
	friend class TThreadSafe;

	TType m_obj;

	mutable std::recursive_mutex mtx;
};