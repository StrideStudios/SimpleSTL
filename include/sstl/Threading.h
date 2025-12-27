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

	TThreadSafe() noexcept {}

	template <typename... TArgs,
		std::enable_if_t<
			std::conjunction_v<
				std::negation<std::is_null_pointer<std::decay_t<TArgs>>>...,
				std::negation<std::is_same<std::decay_t<TArgs>, TThreadSafe>>...
			>,
			int> = 0
	>
	TThreadSafe(TArgs&&... args) noexcept {
		make(std::make_index_sequence<sizeof...(TArgs)>{}, std::forward<TArgs>(args)...);
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

	template<typename... TArgs, std::size_t... CtorN, std::size_t... InitN>
	void make_impl(
		std::index_sequence<CtorN...>,
		std::index_sequence<InitN...>,
		TArgs&&... args
	) {
		// To prevent multiple forwards for args, create a forwarding tuple and forward arguments from it
		auto argsTuple = std::forward_as_tuple(std::forward<TArgs>(args)...);
		using TupleType = decltype(argsTuple);

		// Create the object by getting the arguments associated with it, auto-forwards because of argsTuple
		m_obj = TType(std::get<CtorN>(std::forward<TupleType>(argsTuple))...);

		// The offset is the last element of CtorIdx, the same as it's size
		constexpr size_t initOffset = sizeof...(CtorN);
		if constexpr (sstl::is_initializable_v<typename TUnfurled<TType>::Type, std::tuple_element_t<initOffset + InitN, TupleType>...>) {
			sstl::getUnfurled(m_obj)->init(std::get<initOffset + InitN>(std::forward<TupleType>(argsTuple))...);
		}
	}

	// Try all possible prefix sizes (largest first)
	template<typename... TArgs, std::size_t... N>
	void make(std::index_sequence<N...>, TArgs&&... args) {
		// This is the tuple we use to test slicing
		using Tuple = std::tuple<TArgs&&...>;

		// number of Ctor Args to try and total size of arguments
		constexpr size_t ctorArgs = sizeof...(N);
		constexpr size_t tupSize = sizeof...(TArgs);

		// We have run out of args, this is an invalid call
		if constexpr (ctorArgs == 0 && !std::is_default_constructible_v<typename TUnfurled<TType>::Type>) {
			static_assert(0 < sizeof(TType), "No such constructor!");
		}
		// Test if the underlying type is constructible with the elements in Tuple
		if constexpr (std::is_constructible_v<typename TUnfurled<TType>::Type, std::tuple_element_t<N, Tuple>...>) {
			make_impl(
					std::make_index_sequence<ctorArgs>{},
					std::make_index_sequence<tupSize - ctorArgs>{},
					std::forward<TArgs>(args)...
				);
		} else {
			// Recursive call if we still have args left to try
			make(std::make_index_sequence<ctorArgs - 1>{}, std::forward<TArgs>(args)...);
		}
	}

	TType m_obj;

	mutable std::recursive_mutex mtx;
};