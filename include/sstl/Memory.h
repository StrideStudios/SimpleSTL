#pragma once

#include <memory>

template <typename TType>
struct TUnique {

	TUnique(std::unique_ptr<TType>&& ptr) noexcept
	: m_ptr(std::move(ptr)) {}

	TUnique() = default;

	TUnique(nullptr_t) noexcept {}

	TUnique& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <typename... TArgs>
	TUnique(TArgs&&... args)
	noexcept(std::is_nothrow_constructible_v<TType, TArgs...>)
	: m_ptr(std::make_unique<TType>(std::forward<TArgs>(args)...)) {}

	template <typename TOtherType = TType>
	TUnique(const TUnique<TOtherType>&) = delete;

	template <typename TOtherType = TType>
	TUnique(TUnique<TOtherType>&) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TUnique(TUnique<TOtherType>&& otr) noexcept
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType>
	TUnique& operator=(const TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType = TType>
	TUnique& operator=(TUnique<TOtherType>& otr) = delete;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TUnique& operator=(TUnique<TOtherType>&& otr) noexcept {
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
	}

	template <typename TOtherType>
	TOtherType* staticCast() const noexcept {
		return static_cast<TOtherType*>(this->m_ptr.get());
	}

	template <typename TOtherType>
	TOtherType* dynamicCast() const noexcept {
		return dynamic_cast<TOtherType*>(this->m_ptr.get());
	}

	template <typename TOtherType>
	TOtherType* reinterpretCast() const noexcept {
		return reinterpret_cast<TOtherType*>(this->m_ptr.get());
	}

	template <typename TOtherType>
	TOtherType* constCast() const noexcept {
		return const_cast<TOtherType*>(this->m_ptr.get());
	}

	TType* operator->() const noexcept {
		return m_ptr.get();
	}

	TType* operator*() const noexcept {
		return m_ptr.get();
	}

	TType* get() const noexcept { return m_ptr.get(); }

	operator bool() const noexcept {
		return static_cast<bool>(m_ptr);
	}

	friend bool operator<(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr < snd.m_ptr;
	}

	friend bool operator<=(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr <= snd.m_ptr;
	}

	friend bool operator>(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr > snd.m_ptr;
	}

	friend bool operator>=(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr >= snd.m_ptr;
	}

	friend bool operator==(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr == snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator==(const TUnique& fst, const void* snd) noexcept {
		return fst.m_ptr.get() == snd;
	}

	friend bool operator!=(const TUnique& fst, const TUnique& snd) noexcept {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator!=(const TUnique& fst, const void* snd) noexcept {
		return fst.m_ptr.get() != snd;
	}

	friend size_t getHash(const TUnique& obj) noexcept {
		std::hash<std::unique_ptr<TType>> ptrHash;
		return ptrHash(obj.m_ptr);
	}

private:
	template <typename>
	friend struct TUnique;

	std::unique_ptr<TType> m_ptr = nullptr;

};

template <typename>
struct TWeak;

template <typename TType>
struct TShared {

	TShared(const std::shared_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	TShared(std::shared_ptr<TType>& ptr) noexcept
	: m_ptr(ptr) {}

	TShared(std::shared_ptr<TType>&& ptr) noexcept
	: m_ptr(std::move(ptr)) {}

	template <typename TOtherType>
	TShared(const TWeak<TOtherType>& shared) noexcept;

	template <typename TOtherType>
	TShared(TWeak<TOtherType>& shared) noexcept;

	template <typename TOtherType>
	TShared(const std::weak_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared) {}

	template <typename TOtherType>
	TShared(std::weak_ptr<TOtherType>& shared) noexcept
	: m_ptr(shared) {}

	TShared() = default;

	TShared(nullptr_t) noexcept {}

	TShared& operator=(nullptr_t) noexcept {
		m_ptr = nullptr;
		return *this;
	}

	template <typename... TArgs>
	TShared(TArgs&&... args)
	noexcept(std::is_nothrow_constructible_v<TType, TArgs...>)
	: m_ptr(std::make_shared<TType>(std::forward<TArgs>(args)...)) {}

	template <typename TOtherType = TType>
	TShared(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType = TType>
	TShared(TShared<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	TShared(const TShared& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	TShared(TShared& otr) noexcept
	: m_ptr(otr.m_ptr) {}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TShared(TShared<TOtherType>&& otr) noexcept
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType>
	TShared& operator=(const TShared<TOtherType>& otr) = delete;

	template <typename TOtherType = TType>
	TShared& operator=(TShared<TOtherType>& otr) = delete;

	/*
	 * Allow copies of same type
	 */

	TShared& operator=(const TShared& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	TShared& operator=(TShared& otr) noexcept {
		this->m_ptr = otr.m_ptr;
		return *this;
	}

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_convertible_v<TOtherType*, TType*>, int> = 0
	>
	TShared& operator=(TShared<TOtherType>&& otr) noexcept {
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
	}

	template <typename TOtherType>
	TShared<TOtherType> staticCast() const noexcept {
		return TShared<TOtherType>{std::static_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	TShared<TOtherType> dynamicCast() const noexcept {
		return TShared<TOtherType>{std::dynamic_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	TShared<TOtherType> reinterpretCast() const noexcept {
		return TShared<TOtherType>{std::reinterpret_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	template <typename TOtherType>
	TShared<TOtherType> constCast() const noexcept {
		return TShared<TOtherType>{std::const_pointer_cast<TOtherType, TType>(this->m_ptr)};
	}

	TType* operator->() const noexcept {
		return m_ptr.get();
	}

	TType* operator*() const noexcept {
		return m_ptr.get();
	}

	TType* get() const noexcept { return m_ptr.get(); }

	operator bool() const noexcept {
		return static_cast<bool>(m_ptr);
	}

	friend bool operator<(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr < snd.m_ptr;
	}

	friend bool operator<=(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr <= snd.m_ptr;
	}

	friend bool operator>(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr > snd.m_ptr;
	}

	friend bool operator>=(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr >= snd.m_ptr;
	}

	friend bool operator==(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr == snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator==(const TShared& fst, const void* snd) noexcept {
		return fst.m_ptr.get() == snd;
	}

	friend bool operator!=(const TShared& fst, const TShared& snd) noexcept {
		return fst.m_ptr != snd.m_ptr;
	}

	// Compare raw pointer
	friend bool operator!=(const TShared& fst, const void* snd) noexcept {
		return fst.m_ptr.get() != snd;
	}

	friend size_t getHash(const TShared& obj) noexcept {
		std::hash<std::shared_ptr<TType>> ptrHash;
		return ptrHash(obj.m_ptr);
	}

private:

	template <typename>
	friend struct TShared;

	template <typename>
	friend struct TWeak;

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

	template <typename TOtherType = TType>
	TWeak(const TWeak<TOtherType>& otr) = delete;

	template <typename TOtherType = TType>
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
	TWeak(TWeak<TOtherType>&& otr) noexcept
	: m_ptr(std::move(otr.m_ptr)) {}

	template <typename TOtherType = TType>
	TWeak& operator=(const TWeak<TOtherType>& otr) = delete;

	template <typename TOtherType = TType>
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
	TWeak& operator=(TWeak<TOtherType>&& otr) noexcept {
		this->m_ptr = std::move(otr.m_ptr);
		return *this;
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

	std::weak_ptr<TType> m_ptr;
};

template <typename TType>
template <typename TOtherType>
TShared<TType>::TShared(const TWeak<TOtherType>& shared) noexcept
	: m_ptr(shared.m_ptr) {}

template <typename TType>
template <typename TOtherType>
TShared<TType>::TShared(TWeak<TOtherType>& shared) noexcept
: m_ptr(shared.m_ptr) {}

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
struct TUnfurled<TWeak<TType>> {
	using Type = TType;
	constexpr static bool isManaged = true;
	constexpr static auto get = &TWeak<TType>::get;
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