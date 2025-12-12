#pragma once

#include <utility>

// To keep it simpler than the default std::pair, we rewrite it to be more usable.
// Not used internally in maps, just for Container use for implicit conversion and easy usage
template <typename TKeyType, typename TValueType>
struct TPair {

	TPair():
	key(), value() {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherKeyType>, std::is_copy_constructible<TOtherValueType>>, int> = 0
	>
    constexpr explicit(!std::conjunction_v<std::is_convertible<const TOtherKeyType&, TOtherKeyType>, std::is_convertible<const TOtherValueType&, TOtherValueType>>)
	TPair(const std::pair<TKeyType, TValueType>& pair):
	key(pair.first), value(pair.second) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType>, std::is_constructible<TValueType, TOtherValueType>>, int> = 0
	>
    constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType, TKeyType>, std::is_convertible<TOtherValueType, TValueType>>)
	TPair(std::pair<TOtherKeyType, TOtherValueType>&& pair):
	key(std::forward<TOtherKeyType>(pair.first)), value(std::forward<TOtherValueType>(pair.second)) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_copy_constructible<TOtherKeyType>, std::is_copy_constructible<TOtherValueType>>, int> = 0
	>
    constexpr explicit(!std::conjunction_v<std::is_convertible<const TOtherKeyType&, TOtherKeyType>, std::is_convertible<const TOtherValueType&, TOtherValueType>>)
	TPair(const TOtherKeyType& key, const TOtherValueType& value):
	key(key), value(value) {}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TKeyType, TOtherKeyType>, std::is_constructible<TValueType, TOtherValueType>>, int> = 0
	>
    constexpr explicit(!std::conjunction_v<std::is_convertible<TOtherKeyType, TKeyType>, std::is_convertible<TOtherValueType, TValueType>>)
	TPair(TOtherKeyType&& key, TOtherValueType&& value):
	key(std::forward<TOtherKeyType>(key)), value(std::forward<TOtherValueType>(value)) {}

	operator std::pair<TKeyType, TValueType>() const {
		return std::make_pair(key, value);
	}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_copy_assignable<TOtherKeyType>, std::is_copy_assignable<TOtherValueType>>, int> = 0
	>
	TPair& operator=(const TPair<TOtherKeyType, TOtherValueType>& otr) {
		key = otr.key;
		value = otr.value;
		return *this;
	}

	template <typename TOtherKeyType = TKeyType, typename TOtherValueType = TValueType,
		std::enable_if_t<std::conjunction_v<std::is_assignable<TKeyType, TOtherKeyType>, std::is_assignable<TValueType, TOtherValueType>>, int> = 0
	>
	TPair& operator=(TPair<TOtherKeyType, TOtherValueType>&& otr) {
		key = std::forward<TOtherKeyType>(otr.key);
		value = std::forward<TOtherValueType>(otr.value);
		return *this;
	}

	template <class TOtherKeyType, class TOtherValueType>
	friend constexpr bool operator==(const TPair& lhs, const TPair<TOtherKeyType, TOtherValueType>& rhs) {
		return lhs.key == rhs.key && lhs.value == rhs.value;
	}

	// Use unions to allow for multiple names
		TKeyType key;
	/*union {
		TKeyType index;
	};*/

		TValueType value;
	/*union {
		TValueType obj;
	};*/
};