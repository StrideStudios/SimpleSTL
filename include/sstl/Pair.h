#pragma once

#include <utility>

// To keep it simpler than the default std::pair, we rewrite it to be more usable.
// Not used internally in maps, just for Container use for implicit conversion and easy usage
template <typename TKeyType, typename TValueType>
struct TPair {

	using KeyType = TKeyType;
	using ValueType = TValueType;

    template <class TOtherKeyType = TKeyType, class TOtherValueType = TValueType,
        std::enable_if_t<std::conjunction_v<std::is_default_constructible<TOtherKeyType>, std::is_default_constructible<TOtherValueType>>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::conjunction_v<std::_Is_implicitly_default_constructible<TOtherKeyType>, std::_Is_implicitly_default_constructible<TOtherValueType>>)
#endif
	TPair()
	noexcept(std::is_nothrow_default_constructible_v<TOtherKeyType> && std::is_nothrow_default_constructible_v<TOtherValueType>) // strengthened
	: m_fst(), m_snd() {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, const TOtherKeyType&>, std::is_constructible<TValueType, const TOtherValueType&>>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::conjunction_v<std::is_convertible<const TOtherKeyType&, TKeyType>, std::is_convertible<const TOtherValueType&, TValueType>>)
#endif
	TPair(const TPair<TOtherKeyType, TOtherValueType>& pair)
	noexcept(std::is_nothrow_constructible_v<TKeyType, const TOtherKeyType&> && std::is_nothrow_constructible_v<TValueType, const TOtherValueType&>)
	: m_fst(pair.m_fst), m_snd(pair.m_snd) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType&>, std::is_constructible<TValueType, TOtherValueType&>>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType&, TKeyType>, std::is_convertible<TOtherValueType&, TValueType>>)
#endif
	TPair(TPair<TOtherKeyType, TOtherValueType>& pair)
	noexcept(std::is_nothrow_constructible_v<TKeyType, TOtherKeyType&> && std::is_nothrow_constructible_v<TValueType, TOtherValueType&>)
	: m_fst(pair.m_fst), m_snd(pair.m_snd) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType>, std::is_constructible<TValueType, TOtherValueType>>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType, TKeyType>, std::is_convertible<TOtherValueType, TValueType>>)
#endif
	TPair(TPair<TOtherKeyType, TOtherValueType>&& pair)
	noexcept(std::is_nothrow_constructible_v<TKeyType, TOtherKeyType> && std::is_nothrow_constructible_v<TValueType, TOtherValueType>)
	: m_fst(std::forward<TOtherKeyType>(pair.m_fst)), m_snd(std::forward<TOtherValueType>(pair.m_snd)) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, const TOtherKeyType&>, std::is_constructible<TValueType, const TOtherValueType&>>, int> = 0
	>
#if CXX_VERSION >= 20
    constexpr explicit(!std::conjunction_v<std::is_convertible<const TOtherKeyType&, TKeyType>, std::is_convertible<const TOtherValueType&, TValueType>>)
#endif
	TPair(const std::pair<TOtherKeyType, TOtherValueType>& pair)
	noexcept(std::is_nothrow_constructible_v<TKeyType, const TOtherKeyType&> && std::is_nothrow_constructible_v<TValueType, const TOtherValueType&>)
	: m_fst(pair.first), m_snd(pair.second) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType&>, std::is_constructible<TValueType, TOtherValueType&>>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType&, TKeyType>, std::is_convertible<TOtherValueType&, TValueType>>)
#endif
	TPair(std::pair<TOtherKeyType, TOtherValueType>& pair)
	noexcept(std::is_nothrow_constructible_v<TKeyType, TOtherKeyType&> && std::is_nothrow_constructible_v<TValueType, TOtherValueType&>)
	: m_fst(pair.first), m_snd(pair.second) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType>, std::is_constructible<TValueType, TOtherValueType>>, int> = 0
	>
#if CXX_VERSION >= 20
    constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType, TKeyType>, std::is_convertible<TOtherValueType, TValueType>>)
#endif
	TPair(std::pair<TOtherKeyType, TOtherValueType>&& pair)
	noexcept(std::is_nothrow_constructible_v<TKeyType, TOtherKeyType> && std::is_nothrow_constructible_v<TValueType, TOtherValueType>)
	: m_fst(std::forward<TOtherKeyType>(pair.first)), m_snd(std::forward<TOtherValueType>(pair.second)) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, const TOtherKeyType&>, std::is_constructible<TValueType, const TOtherValueType&>>, int> = 0
	>
#if CXX_VERSION >= 20
    constexpr explicit(!std::conjunction_v<std::is_convertible<const TOtherKeyType&, TKeyType>, std::is_convertible<const TOtherValueType&, TValueType>>)
#endif
	TPair(const TOtherKeyType& fst, const TOtherValueType& snd)
	noexcept(std::is_nothrow_constructible_v<TKeyType, const TOtherKeyType&> && std::is_nothrow_constructible_v<TValueType, const TOtherValueType&>)
	: m_fst(fst), m_snd(snd) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType&>, std::is_constructible<TValueType, TOtherValueType&>>, int> = 0
	>
#if CXX_VERSION >= 20
	constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType&, TKeyType>, std::is_convertible<TOtherValueType&, TValueType>>)
#endif
	TPair(TOtherKeyType& fst, TOtherValueType& snd)
	noexcept(std::is_nothrow_constructible_v<TKeyType, TOtherKeyType&> && std::is_nothrow_constructible_v<TValueType, TOtherValueType&>)
	: m_fst(fst), m_snd(snd) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType>, std::is_constructible<TValueType, TOtherValueType>>, int> = 0
	>
#if CXX_VERSION >= 20
    constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType, TKeyType>, std::is_convertible<TOtherValueType, TValueType>>)
#endif
	TPair(TOtherKeyType&& fst, TOtherValueType&& snd)
	noexcept(std::is_nothrow_constructible_v<TKeyType, TOtherKeyType> && std::is_nothrow_constructible_v<TValueType, TOtherValueType>)
	: m_fst(std::forward<TOtherKeyType>(fst)), m_snd(std::forward<TOtherValueType>(snd)) {}

	operator std::pair<TKeyType, TValueType>() const {
		return std::make_pair(m_fst, m_snd);
	}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType&, const TOtherKeyType&>, std::is_assignable<TValueType&, const TOtherValueType&>>, int> = 0
	>
	TPair& operator=(const TPair<TOtherKeyType, TOtherValueType>& otr)
	noexcept(std::is_nothrow_assignable_v<TKeyType&, const TOtherKeyType&> && std::is_nothrow_assignable_v<TValueType&, const TOtherValueType&>) {
		m_fst = otr.m_fst;
		m_snd = otr.m_snd;
		return *this;
	}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType&, TOtherKeyType&>, std::is_assignable<TValueType&, TOtherValueType&>>, int> = 0
	>
	TPair& operator=(TPair<TOtherKeyType, TOtherValueType>& otr)
	noexcept(std::is_nothrow_assignable_v<TKeyType&, TOtherKeyType&> && std::is_nothrow_assignable_v<TValueType&, TOtherValueType&>) {
		m_fst = otr.m_fst;
		m_snd = otr.m_snd;
		return *this;
	}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType&, TOtherKeyType>, std::is_assignable<TValueType&, TOtherValueType>>, int> = 0
	>
	TPair& operator=(TPair<TOtherKeyType, TOtherValueType>&& otr)
	noexcept(std::is_nothrow_assignable_v<TKeyType&, TOtherKeyType> && std::is_nothrow_assignable_v<TValueType&, TOtherValueType>) {
		m_fst = std::forward<TOtherKeyType>(otr.m_fst);
		m_snd = std::forward<TOtherValueType>(otr.m_snd);
		return *this;
	}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType&, const TOtherKeyType&>, std::is_assignable<TValueType&, const TOtherValueType&>>, int> = 0
	>
	TPair& operator=(const std::pair<TOtherKeyType, TOtherValueType>& otr)
	noexcept(std::is_nothrow_assignable_v<TKeyType&, const TOtherKeyType&> && std::is_nothrow_assignable_v<TValueType&, const TOtherValueType&>) {
    	m_fst = otr.first;
    	m_snd = otr.second;
    	return *this;
    }

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType&, TOtherKeyType&>, std::is_assignable<TValueType&, TOtherValueType&>>, int> = 0
	>
	TPair& operator=(std::pair<TOtherKeyType, TOtherValueType>& otr)
	noexcept(std::is_nothrow_assignable_v<TKeyType&, TOtherKeyType&> && std::is_nothrow_assignable_v<TValueType&, TOtherValueType&>) {
    	m_fst = otr.first;
    	m_snd = otr.second;
    	return *this;
    }

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType&, TOtherKeyType>, std::is_assignable<TValueType&, TOtherValueType>>, int> = 0
	>
	TPair& operator=(std::pair<TOtherKeyType, TOtherValueType>&& otr)
	noexcept(std::is_nothrow_assignable_v<TKeyType&, TOtherKeyType> && std::is_nothrow_assignable_v<TValueType&, TOtherValueType>) {
    	m_fst = std::forward<TOtherKeyType>(otr.first);
    	m_snd = std::forward<TOtherValueType>(otr.second);
    	return *this;
    }

	template <class TOtherKeyType, class TOtherValueType>
	friend constexpr bool operator==(const TPair& lhs, const TPair<TOtherKeyType, TOtherValueType>& rhs) {
		return lhs.m_fst == rhs.m_fst && lhs.m_snd == rhs.m_snd;
	}

	template <class TOtherKeyType, class TOtherValueType>
	friend constexpr bool operator==(const TPair& lhs, const std::pair<TOtherKeyType, TOtherValueType>& rhs) {
    	return lhs.m_fst == rhs.first && lhs.m_snd == rhs.second;
    }

	const TKeyType& key() const { return m_fst; }
	TKeyType& key() { return m_fst; }
	const TKeyType& index() const { return m_fst; }
	TKeyType& index() { return m_fst; }
	const TKeyType& first() const { return m_fst; }
	TKeyType& first() { return m_fst; }

	const TValueType& value() const { return m_snd; }
	TValueType& value() { return m_snd; }
	const TValueType& obj() const { return m_snd; }
	TValueType& obj() { return m_snd; }
	const TValueType& second() const { return m_snd; }
	TValueType& second() { return m_snd; }

private:

	template<typename, typename>
	friend struct TPair;

	TKeyType m_fst;
	TValueType m_snd;
};

template <typename TKeyType, typename TValueType>
TPair(const std::pair<TKeyType, TValueType>&) -> TPair<TKeyType, TValueType>;

template <typename TKeyType, typename TValueType>
TPair(std::pair<TKeyType, TValueType>&) -> TPair<TKeyType, TValueType>;

template <typename TKeyType, typename TValueType>
TPair(std::pair<TKeyType, TValueType>&&) -> TPair<TKeyType, TValueType>;

template <typename TKeyType, typename TValueType>
TPair(const TKeyType&, const TValueType&) -> TPair<TKeyType, TValueType>;

template <typename TKeyType, typename TValueType>
TPair(TKeyType&, TValueType&) -> TPair<TKeyType, TValueType>;

template <typename TKeyType, typename TValueType>
TPair(TKeyType&&, TValueType&&) -> TPair<TKeyType, TValueType>;
