#pragma once

#include <memory>

namespace sstl {
#ifndef SIMPLESTL_INIT
	template <typename TType, typename... TArgs>
	constexpr bool is_initializable_v = false;

	template <typename TType, typename... TArgs>
	struct is_initializable : std::bool_constant<is_initializable_v<TType, TArgs...>> {};
#else
	#if CXX_VERSION >= 20
		template <typename TType, typename... TArgs>
		concept is_initializable_v =
		requires(TType& obj, TArgs&&... args) {
			obj.init(std::forward<TArgs>(args)...);
		};

		template <typename TType, typename... TArgs>
		struct is_initializable : std::bool_constant<is_initializable_v<TType, TArgs...>> {};
	#else
		template <typename, typename TType, typename... TArgs>
		struct is_initializable : std::false_type {};

		template <typename TType, typename... TArgs>
		struct is_initializable<
			std::void_t<decltype(std::declval<TType&>().init(std::declval<TArgs>()...))>,
			TType,
			TArgs...
		> : std::true_type {};

	template <typename TType, typename... TArgs>
	constexpr bool is_initializable_v = is_initializable<void, TType, TArgs...>::value;
	#endif
#endif

#ifndef SIMPLESTL_DESTROY
	template <typename TType>
	constexpr bool is_destroyable_v = false;

	template <typename TType>
	struct is_destroyable : std::bool_constant<is_destroyable_v<TType>> {};
#else
	#if CXX_VERSION >= 20
		template <typename TType>
		concept is_destroyable =
		requires(TType& obj) {
			obj.destroy();
		};

		template <typename TType>
		constexpr bool is_destroyable_v = is_destroyable<TType>;
	#else
		template <typename TType, typename = void>
		struct is_destroyable : std::false_type {};

		template <typename TType>
		struct is_destroyable<
			std::void_t<decltype(std::declval<TType&>().destroy())>,
			TType
		> : std::true_type {};

		template <typename TType>
		constexpr bool is_destroyable_v = is_destroyable<TType>::value;
	#endif
#endif
	template <typename TType>
	struct deleter {
		constexpr deleter() noexcept = default;

		template <typename TOtherType, std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0>
		deleter(const deleter<TOtherType>&) noexcept {}

		void operator()(TType* ptr) const noexcept {
			static_assert(0 < sizeof(TType), "Can't delete an incomplete type!");
			if constexpr (sstl::is_destroyable_v<TType>) {
				ptr->destroy();
			}
			delete ptr;
		}
	};

	template <typename TType>
	void delete_impl(void* p) noexcept {
		static_assert(0 < sizeof(TType), "Can't delete an incomplete type!");
		TType* ptr = static_cast<TType*>(p);
		if constexpr (sstl::is_destroyable_v<TType>) {
			ptr->destroy();
		}
		delete ptr;
	}

	template <typename TType>
	struct delayed_deleter {
		typedef void (*Func)(void* ptr);
		Func fn = nullptr;

		constexpr delayed_deleter() noexcept = default;
		constexpr delayed_deleter(const Func fn) noexcept: fn(fn) {}

		template <typename TOtherType, std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0>
		delayed_deleter(const delayed_deleter<TOtherType>& otr) noexcept: fn(otr.fn) {}

		void operator()(void* ptr) const noexcept {
			if (fn) fn(ptr);
		}
	};
}

template <typename TType>
struct TUnique {

	_CONSTEXPR23 TUnique(std::unique_ptr<TType, sstl::deleter<TType>>&& ptr) noexcept
	: m_ptr(std::move(ptr)) {}

	// If not default constructible, default to nullptr, otherwise, let Args constructor handle it
	template <typename TOtherType = TType,
		std::enable_if_t<!std::is_default_constructible_v<TOtherType>, int> = 0
	>
	_CONSTEXPR23 TUnique() noexcept {}

	_CONSTEXPR23 TUnique(nullptr_t) noexcept {}

	_CONSTEXPR23 TUnique& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 explicit TUnique(TOtherType* ptr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: m_ptr(ptr, sstl::delayed_deleter<TOtherType>(&sstl::delete_impl<TOtherType>)) {}

	template <typename... TArgs,
		std::enable_if_t<
			std::conjunction_v<
				std::negation<std::is_null_pointer<std::decay_t<TArgs>>>...,
				std::negation<std::is_same<std::decay_t<TArgs>, TUnique>>...
			>,
			int> = 0
	>
	_CONSTEXPR23 TUnique(TArgs&&... args) noexcept {
		make(std::make_index_sequence<sizeof...(TArgs)>{}, std::forward<TArgs>(args)...);
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TUnique(const TUnique<TOtherType>&) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TUnique(TUnique<TOtherType>&) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TUnique(TUnique<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TUnique& operator=(const TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TUnique& operator=(TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TUnique& operator=(TUnique<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
	}

	// Releases ownership of the pointer, in this case it will be destroyed
	void destroy() noexcept {
		m_ptr.reset();
	}

	template <typename TOtherType>
	_CONSTEXPR23 TOtherType* staticCast() const noexcept {
		return static_cast<TOtherType*>(this->m_ptr.get());
	}

	template <typename TOtherType>
	_CONSTEXPR23 TOtherType* dynamicCast() const noexcept {
		return dynamic_cast<TOtherType*>(this->m_ptr.get());
	}

	template <typename TOtherType>
	_CONSTEXPR23 TOtherType* reinterpretCast() const noexcept {
		return reinterpret_cast<TOtherType*>(this->m_ptr.get());
	}

	template <typename TOtherType>
	_CONSTEXPR23 TOtherType* constCast() const noexcept {
		return const_cast<TOtherType*>(this->m_ptr.get());
	}

	_CONSTEXPR23 TType* operator->() const noexcept {
		return m_ptr.get();
	}

	_CONSTEXPR23 TType* operator*() const noexcept {
		return m_ptr.get();
	}

	_CONSTEXPR23 TType* get() const noexcept { return m_ptr.get(); }

	_CONSTEXPR23 operator bool() const noexcept {
		return static_cast<bool>(m_ptr);
	}

	_CONSTEXPR23 friend bool operator<(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr < snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator<=(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr <= snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator>(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr > snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator>=(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr >= snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator==(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr == snd.m_ptr;
	}

	// Compare raw pointer
	_CONSTEXPR23 friend bool operator==(const TUnique& fst, const void* snd) noexcept {
		return fst.m_ptr.get() == snd;
	}

	_CONSTEXPR23 friend bool operator!=(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	_CONSTEXPR23 friend bool operator!=(const TUnique& fst, const void* snd) noexcept {
		return fst.m_ptr.get() != snd;
	}

	_CONSTEXPR23 friend size_t getHash(const TUnique& obj) noexcept {
		std::hash<std::unique_ptr<TType, sstl::deleter<TType>>> ptrHash;
		return ptrHash(obj.m_ptr);
	}

private:
	template <typename>
	friend struct TUnique;

	template <typename>
	friend struct TFrail;

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
		m_ptr = std::unique_ptr<TType, sstl::delayed_deleter<TType>>{new TType(std::get<CtorN>(std::forward<TupleType>(argsTuple))...), sstl::delayed_deleter<TType>(&sstl::delete_impl<TType>)};

		// The offset is the last element of CtorIdx, the same as it's size
		constexpr size_t initOffset = sizeof...(CtorN);
		if constexpr (sstl::is_initializable_v<TType, std::tuple_element_t<initOffset + InitN, TupleType>...>) {
			m_ptr->init(std::get<initOffset + InitN>(std::forward<TupleType>(argsTuple))...);
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
		if constexpr (ctorArgs == 0 && !std::is_default_constructible_v<TType>) {
			static_assert(0 < sizeof(TType), "No such constructor!");
		}
		// Test if the underlying type is constructible with the elements in Tuple
		if constexpr (std::is_constructible_v<TType, std::tuple_element_t<N, Tuple>...>) {
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

	std::unique_ptr<TType, sstl::delayed_deleter<TType>> m_ptr = nullptr;

};

template <typename>
struct TWeak;

template <typename TType>
struct TShared {

	_CONSTEXPR23 TShared(const std::shared_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	_CONSTEXPR23 TShared(std::shared_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	_CONSTEXPR23 TShared(std::shared_ptr<TType>&& ptr) noexcept
	: m_ptr(std::move(ptr)) {}

	template <typename TOtherType>
	_CONSTEXPR23 TShared(const TWeak<TOtherType>& shared) noexcept;

	template <typename TOtherType>
	_CONSTEXPR23 TShared(TWeak<TOtherType>& shared) noexcept;

	template <typename TOtherType>
	_CONSTEXPR23 TShared(const std::weak_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared) {}

	template <typename TOtherType>
	_CONSTEXPR23 TShared(std::weak_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared) {}

	// If not default constructible, default to nullptr, otherwise, let Args constructor handle it
	template <typename TOtherType = TType,
		std::enable_if_t<!std::is_default_constructible_v<TOtherType>, int> = 0
	>
	_CONSTEXPR23 TShared() noexcept {}

	_CONSTEXPR23 TShared(nullptr_t) noexcept {}

	_CONSTEXPR23 TShared& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 explicit TShared(TOtherType* ptr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: m_ptr(ptr, sstl::deleter<TType>()) {}

	// prefer init because SharedFrom works there
	template <typename... TArgs,
		std::enable_if_t<
			std::conjunction_v<
				std::negation<std::is_null_pointer<std::decay_t<TArgs>>>...,
				std::negation<std::is_same<std::decay_t<TArgs>, TShared>>...
			>,
			int> = 0
	>
	_CONSTEXPR23 TShared(TArgs&&... args) noexcept {
		make(std::make_index_sequence<sizeof...(TArgs)>{}, std::forward<TArgs>(args)...);
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TShared(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TShared(TShared<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	_CONSTEXPR23 TShared(const TShared& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	_CONSTEXPR23 TShared(TShared& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TShared(TShared<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TShared& operator=(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TShared& operator=(TShared<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	_CONSTEXPR23 TShared& operator=(const TShared& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	_CONSTEXPR23 TShared& operator=(TShared& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 TShared& operator=(TShared<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
	}

	// Releases ownership of the pointer, note the object will not be destroyed unless all other shared pointers are
	void destroy() noexcept {
		m_ptr.reset();
	}

	template <typename TOtherType>
	_CONSTEXPR23 TShared<TOtherType> staticCast() const noexcept {
		return TShared<TOtherType>{std::static_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	_CONSTEXPR23 TShared<TOtherType> dynamicCast() const noexcept {
		return TShared<TOtherType>{std::dynamic_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	_CONSTEXPR23 TShared<TOtherType> reinterpretCast() const noexcept {
		return TShared<TOtherType>{std::reinterpret_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	_CONSTEXPR23 TShared<TOtherType> constCast() const noexcept {
		return TShared<TOtherType>{std::const_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	_CONSTEXPR23 TType* operator->() const noexcept {
		return m_ptr.get();
	}

	_CONSTEXPR23 TType* operator*() const noexcept {
		return m_ptr.get();
	}

	_CONSTEXPR23 TType* get() const noexcept { return m_ptr.get(); }

	_CONSTEXPR23 operator bool() const noexcept {
		return static_cast<bool>(m_ptr);
	}

	_CONSTEXPR23 friend bool operator<(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr < snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator<=(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr <= snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator>(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr > snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator>=(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr >= snd.m_ptr;
	}

	_CONSTEXPR23 friend bool operator==(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr == snd.m_ptr;
	}

	// Compare raw pointer
	_CONSTEXPR23 friend bool operator==(const TShared& fst, const void* snd) noexcept {
		return fst.m_ptr.get() == snd;
	}

	_CONSTEXPR23 friend bool operator!=(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	_CONSTEXPR23 friend bool operator!=(const TShared& fst, const void* snd) noexcept {
		return fst.m_ptr.get() != snd;
	}

	_CONSTEXPR23 friend size_t getHash(const TShared& obj) noexcept {
		std::hash<std::shared_ptr<TType>> ptrHash;
		return ptrHash(obj.m_ptr);
	}

private:

	template <typename>
	friend struct TShared;

	template <typename>
	friend struct TWeak;

	template <typename>
	friend struct TFrail;

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
		m_ptr = std::shared_ptr<TType>{new TType(std::get<CtorN>(std::forward<TupleType>(argsTuple))...), sstl::deleter<TType>()};

		// The offset is the last element of CtorIdx, the same as it's size
		constexpr size_t initOffset = sizeof...(CtorN);
		if constexpr (sstl::is_initializable_v<TType, std::tuple_element_t<initOffset + InitN, TupleType>...>) {
			m_ptr->init(std::get<initOffset + InitN>(std::forward<TupleType>(argsTuple))...);
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
		if constexpr (ctorArgs == 0 && !std::is_default_constructible_v<TType>) {
			static_assert(0 < sizeof(TType), "No such constructor!");
		}
		// Test if the underlying type is constructible with the elements in Tuple
		if constexpr (std::is_constructible_v<TType, std::tuple_element_t<N, Tuple>...>) {
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

	std::shared_ptr<TType> m_ptr = nullptr;

};

template <typename TType>
struct TWeak {

	TWeak(const std::weak_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	TWeak(std::weak_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	TWeak(std::weak_ptr<TType>&& ptr) noexcept
	: m_ptr(std::move(ptr)) {}

	template <typename TOtherType>
	TWeak(const TShared<TOtherType>& shared) noexcept
	: m_ptr(shared.m_ptr) {}

	template <typename TOtherType>
	TWeak(TShared<TOtherType>& shared) noexcept
	: m_ptr(shared.m_ptr) {}

	template <typename TOtherType>
	TWeak(const std::shared_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared) {}

	template <typename TOtherType>
	TWeak(std::shared_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared) {}

	TWeak() = default;

	TWeak(nullptr_t) noexcept {}

	TWeak& operator=(nullptr_t) noexcept {
		m_ptr.reset();
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TWeak(const TWeak<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TWeak(TWeak<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	TWeak(const TWeak& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	TWeak(TWeak& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TWeak(TWeak<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TWeak& operator=(const TWeak<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TWeak& operator=(TWeak<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	TWeak& operator=(const TWeak& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	TWeak& operator=(TWeak& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TWeak& operator=(TWeak<TOtherType>&& otr)
#if CXX_VERSION >= 20
noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
noexcept {
#endif
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
	}

	// Resets weak pointer to an empty state, since this pointer has no ownership it will no destroy any objects
	void destroy() noexcept {
		m_ptr.reset();
	}

	template <typename TOtherType>
	TShared<TOtherType> staticCast() const noexcept {
		if (auto ptr = m_ptr.lock()) {
			return TShared<TOtherType>{std::static_pointer_cast<TOtherType, TType>(ptr)};
		}
		return nullptr;
	}

	template <typename TOtherType>
	TShared<TOtherType> dynamicCast() const noexcept {
		if (auto ptr = m_ptr.lock()) {
			return TShared<TOtherType>{std::dynamic_pointer_cast<TOtherType, TType>(ptr)};
		}
		return nullptr;
	}

	template <typename TOtherType>
	TShared<TOtherType> reinterpretCast() const noexcept {
		if (auto ptr = m_ptr.lock()) {
			return TShared<TOtherType>{std::reinterpret_pointer_cast<TOtherType, TType>(ptr)};
		}
		return nullptr;
	}

	template <typename TOtherType>
	TShared<TOtherType> constCast() const noexcept {
		if (auto ptr = m_ptr.lock()) {
			return TShared<TOtherType>{std::const_pointer_cast<TOtherType, TType>(ptr)};
		}
		return nullptr;
	}

	// Workaround for not being able to reference TWeak from TShared
	operator std::shared_ptr<TType>() const noexcept {
		return m_ptr.lock();
	}

	operator TShared<TType>() const noexcept {
		return TShared<TType>{m_ptr.lock()};
	}

	TShared<TType> operator->() const noexcept {
		return TShared<TType>{m_ptr.lock()};
	}

	TShared<TType> operator*() const noexcept {
		return TShared<TType>{m_ptr.lock()};
	}

	TShared<TType> get() const noexcept { return TShared<TType>{m_ptr.lock()}; }

	operator bool() const noexcept {
		return !m_ptr.expired();
	}

	friend bool operator<(const TWeak& fst, const TWeak& snd) noexcept {
		return fst.m_ptr.owner_before(snd.m_ptr);
	}

	friend bool operator<=(const TWeak& fst, const TWeak& snd) noexcept {
		return !snd.m_ptr.owner_before(fst.m_ptr);
	}

	friend bool operator>(const TWeak& fst, const TWeak& snd) noexcept {
		return snd.m_ptr.owner_before(fst.m_ptr);
	}

	friend bool operator>=(const TWeak& fst, const TWeak& snd) noexcept {
		return !fst.m_ptr.owner_before(snd.m_ptr);
	}

	// TODO: c++26 owner_equal c++26 owner_hash
	friend bool operator==(const TWeak& fst, const TWeak& snd) noexcept {
		return !fst.m_ptr.owner_before(snd.m_ptr) &&
			!snd.m_ptr.owner_before(fst.m_ptr);
	}

	// Compare raw pointer
	friend bool operator==(const TWeak& fst, const void* snd) noexcept {
		auto ptr = fst.m_ptr.lock();
		return ptr && ptr.get() == snd;
	}

	friend bool operator!=(const TWeak& fst, const TWeak& snd) noexcept {
		return fst.m_ptr.owner_before(snd.m_ptr) ||
			snd.m_ptr.owner_before(fst.m_ptr);
	}

	// Compare raw pointer
	friend bool operator!=(const TWeak& fst, const void* snd) noexcept {
		auto ptr = fst.m_ptr.lock();
		return !ptr || ptr.get() != snd;
	}

	friend size_t getHash(const TWeak& obj) noexcept {
		if (auto ptr = obj.m_ptr.lock()) {
			std::hash<std::shared_ptr<TType>> ptrHash;
			return ptrHash(ptr);
		}
		return 0;
	}

private:

	template <typename>
	friend struct TShared;

	template <typename>
	friend struct TWeak;

	template <typename>
	friend struct TFrail;

	std::weak_ptr<TType> m_ptr;
};

template <typename TType>
template <typename TOtherType>
_CONSTEXPR23 TShared<TType>::TShared(const TWeak<TOtherType>& shared) noexcept
	: m_ptr(shared.m_ptr) {}

template <typename TType>
template <typename TOtherType>
_CONSTEXPR23 TShared<TType>::TShared(TWeak<TOtherType>& shared) noexcept
: m_ptr(shared.m_ptr) {}

template <typename TType>
struct TFrail {

	TFrail(const TWeak<TType>& ptr) noexcept
	: m_ptr(ptr.m_ptr.lock().get()) {}

	TFrail(TWeak<TType>& ptr) noexcept
	: m_ptr(ptr.m_ptr.lock().get()) {}

	TFrail(const std::weak_ptr<TType>& ptr) noexcept
	: m_ptr(ptr.lock().get()) {}

	TFrail(std::weak_ptr<TType>& ptr) noexcept
	: m_ptr(ptr.lock().get()) {}

	template <typename TOtherType>
	TFrail(const TShared<TOtherType>& shared) noexcept
	: m_ptr(shared.m_ptr.get()) {}

	template <typename TOtherType>
	TFrail(TShared<TOtherType>& shared) noexcept
	: m_ptr(shared.m_ptr.get()) {}

	template <typename TOtherType>
	TFrail(const std::shared_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared.get()) {}

	template <typename TOtherType>
	TFrail(std::shared_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared.get()) {}

	template <typename TOtherType>
	TFrail(const TUnique<TOtherType>& unique) noexcept
	: m_ptr(unique.m_ptr.get()) {}

	template <typename TOtherType>
	TFrail(TUnique<TOtherType>& unique) noexcept
	: m_ptr(unique.m_ptr.get()) {}

	template <typename TOtherType>
	TFrail(const std::unique_ptr<TOtherType>& unique) noexcept
	: m_ptr(unique.get()) {}

	template <typename TOtherType>
	TFrail(std::unique_ptr<TOtherType>& unique) noexcept
	: m_ptr(unique.get()) {}

	TFrail() = default;

	TFrail(TType* ptr): m_ptr(ptr) {}

	TFrail(nullptr_t) noexcept {}

	TFrail& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TFrail(const TFrail<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TFrail(TFrail<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	TFrail(const TFrail& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	TFrail(TFrail& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TFrail(TFrail<TOtherType>&& otr)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>)
#else
	noexcept
#endif
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TFrail& operator=(const TFrail<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TFrail& operator=(TFrail<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	TFrail& operator=(const TFrail& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	TFrail& operator=(TFrail& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TFrail& operator=(TFrail<TOtherType>&& otr)
#if CXX_VERSION >= 20
noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
noexcept {
#endif
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
	}

	template <typename TOtherType>
	TFrail<TOtherType> staticCast() const noexcept {
		return TFrail<TOtherType>{static_cast<TOtherType*>(m_ptr)};
	}

	template <typename TOtherType>
	TFrail<TOtherType> dynamicCast() const noexcept {
		return TFrail<TOtherType>{dynamic_cast<TOtherType*>(m_ptr)};
	}

	template <typename TOtherType>
	TFrail<TOtherType> reinterpretCast() const noexcept {
		return TFrail<TOtherType>{reinterpret_cast<TOtherType*>(m_ptr)};
	}

	template <typename TOtherType>
	TFrail<TOtherType> constCast() const noexcept {
		return TFrail<TOtherType>{const_cast<TOtherType*>(m_ptr)};
	}

	TType* operator->() const noexcept {
		return m_ptr;
	}

	TType* operator*() const noexcept {
		return m_ptr;
	}

	TType* get() const noexcept { return m_ptr; }

	operator bool() const noexcept {
		return m_ptr != nullptr;
	}

	friend bool operator<(const TFrail& fst, const TFrail& snd) noexcept {
		return fst.m_ptr < snd.m_ptr;
	}

	friend bool operator<=(const TFrail& fst, const TFrail& snd) noexcept {
		return fst.m_ptr <= snd.m_ptr;
	}

	friend bool operator>(const TFrail& fst, const TFrail& snd) noexcept {
		return fst.m_ptr > snd.m_ptr;
	}

	friend bool operator>=(const TFrail& fst, const TFrail& snd) noexcept {
		return fst.m_ptr >= snd.m_ptr;
	}

	// TODO: c++26 owner_equal c++26 owner_hash
	friend bool operator==(const TFrail& fst, const TFrail& snd) noexcept {
		return fst.m_ptr == snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator==(const TFrail& fst, const void* snd) noexcept {
		return fst.m_ptr == snd;
	}

	friend bool operator!=(const TFrail& fst, const TFrail& snd) noexcept {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator!=(const TFrail& fst, const void* snd) noexcept {
		return fst.m_ptr != snd;
	}

	friend size_t getHash(const TFrail& obj) noexcept {
		std::hash<TType*> ptrHash;
		return ptrHash(obj.m_ptr);
	}

private:

	template <typename>
	friend struct TFrail;

	TType* m_ptr = nullptr;
};

template <typename TType>
struct TSharedFrom {
	using _Esft_type = TSharedFrom;

	_NODISCARD TShared<TType> getShared() {
		return TShared<TType>(_Wptr);
	}

	_NODISCARD TShared<const TType> getShared() const {
		return TShared<const TType>(_Wptr);
	}

	_NODISCARD TWeak<TType> getWeak() noexcept {
		return _Wptr;
	}

	_NODISCARD TWeak<const TType> getWeak() const noexcept {
		return _Wptr;
	}

protected:
	constexpr TSharedFrom() noexcept : _Wptr() {}

	TSharedFrom(const TSharedFrom&) noexcept : _Wptr() {}

	TSharedFrom& operator=(const TSharedFrom&) noexcept {
		return *this;
	}

	~TSharedFrom() = default;

private:
	template <typename>
	friend struct TShared;

	template <typename>
	friend struct std::shared_ptr;

	// Due to how this works internally, it has to be a std::weak_ptr, and called _Wptr
	mutable std::weak_ptr<TType> _Wptr;
};

template <typename TType>
struct TUnfurled {
	using Type = TType;
	constexpr static bool isManaged = false;

	template <typename TOtherType = TType, typename... TArgs,
		std::enable_if_t<std::is_convertible_v<TOtherType, TType>, int> = 0
	>
	constexpr static TType create(TArgs&&... args)
#if CXX_VERSION >= 20
noexcept(std::is_nothrow_convertible_v<TOtherType, TType>) {
#else
noexcept {
#endif
		return make<TType>(std::make_index_sequence<sizeof...(TArgs)>{}, std::forward<TArgs>(args)...);
	}

private:

	template<typename TOtherType, typename... TArgs, std::size_t... CtorN, std::size_t... InitN>
	static TOtherType make_impl(
		std::index_sequence<CtorN...>,
		std::index_sequence<InitN...>,
		TArgs&&... args
	) {
		// To prevent multiple forwards for args, create a forwarding tuple and forward arguments from it
		auto argsTuple = std::forward_as_tuple(std::forward<TArgs>(args)...);
		using TupleType = decltype(argsTuple);

		// Create the object by getting the arguments associated with it, auto-forwards because of argsTuple
		TOtherType obj(std::get<CtorN>(std::forward<TupleType>(argsTuple))...);

		// The offset is the last element of CtorIdx, the same as it's size
		constexpr size_t initOffset = sizeof...(CtorN);
		if constexpr (sstl::is_initializable_v<TOtherType, std::tuple_element_t<initOffset + InitN, TupleType>...>) {
			obj.init(std::get<initOffset + InitN>(std::forward<TupleType>(argsTuple))...);
		}

		return obj;
	}

	// Try all possible prefix sizes (largest first)
	template<typename TOtherType, typename... TArgs, std::size_t... N>
	static TOtherType make(std::index_sequence<N...>, TArgs&&... args) {
		// This is the tuple we use to test slicing
		using Tuple = std::tuple<TArgs&&...>;

		// number of Ctor Args to try and total size of arguments
		constexpr size_t ctorArgs = sizeof...(N);
		constexpr size_t tupSize = sizeof...(TArgs);

		// We have run out of args, this is an invalid call
		if constexpr (ctorArgs == 0 && !std::is_default_constructible_v<TOtherType>) {
			static_assert(0 < sizeof(TOtherType), "No such constructor!");
		}
		// Test if the underlying type is constructible with the elements in Tuple
		if constexpr (std::is_constructible_v<TOtherType, std::tuple_element_t<N, Tuple>...>) {
			return make_impl<TOtherType>(
					std::make_index_sequence<ctorArgs>{},
					std::make_index_sequence<tupSize - ctorArgs>{},
					std::forward<TArgs>(args)...
				);
		} else {
			// Recursive call if we still have args left to try
			return make<TOtherType>(std::make_index_sequence<ctorArgs - 1>{}, std::forward<TArgs>(args)...);
		}
	}
};

template <typename TType>
struct TUnfurled<TShared<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
	constexpr static auto get = &TShared<TType>::get;

	template <typename TOtherType = TType, typename... TArgs,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 static TShared<TType> create(TArgs&&... args)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		return TShared<TOtherType>(std::forward<TArgs>(args)...);
	}
};

template <typename TType>
struct TUnfurled<TUnique<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
	constexpr static auto get = &TUnique<TType>::get;

	template <typename TOtherType = TType, typename... TArgs,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	_CONSTEXPR23 static TUnique<TType> create(TArgs&&... args)
#if CXX_VERSION >= 20
	noexcept(std::is_nothrow_convertible_v<TOtherType*, TType*>) {
#else
	noexcept {
#endif
		return TUnique<TOtherType>(std::forward<TArgs>(args)...);
	}
};

template <typename TType>
struct TUnfurled<TWeak<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
	constexpr static auto get = &TWeak<TType>::get;
};

template <typename TType>
struct TUnfurled<TFrail<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
	constexpr static auto get = &TFrail<TType>::get;
};

/*
 * Determine whether TType is a managed pointer or not
 */

namespace sstl {
	template <typename TType>
	constexpr bool is_managed_v = TUnfurled<TType>::isManaged;

	template <typename TType>
	struct is_managed : std::bool_constant<is_managed_v<TType>> {};

	template <typename TType>
	_CONSTEXPR23 TType* getUnfurled(TType* type) {
		return type;
	}

	template <typename TType>
	_CONSTEXPR23 const TType* getUnfurled(const TType* type) {
		return type;
	}

	template <typename TType>
	_CONSTEXPR23 typename TUnfurled<TType>::Type* getUnfurled(TType& type) {
		if constexpr (TUnfurled<TType>::isManaged) {
			return type.get();
		} else {
			return &type;
		}
	}

	template <typename TType>
	_CONSTEXPR23 const typename TUnfurled<TType>::Type* getUnfurled(const TType& type) {
		if constexpr (TUnfurled<TType>::isManaged) {
			return type.get();
		} else {
			return &type;
		}
	}
}