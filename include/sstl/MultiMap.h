#pragma once

#include <unordered_map>
#include "Container.h"

template <typename TKeyType, typename TValueType>
struct TMultiMap : TAssociativeContainer<TKeyType, TValueType> {

	[[nodiscard]] virtual size_t getSize() const override {
		return m_Container.size();
	}

	virtual TPair<TKeyType, const TValueType&> top() const override {
		return TPair<TKeyType, const TValueType&>{*m_Container.begin()};
	}

	virtual TPair<TKeyType, const TValueType&> bottom() const override {
		return TPair<TKeyType, const TValueType&>{*m_Container.end()};
	}

	virtual bool contains(const TKeyType& key) const override {
		return ASSOCIATIVE_CONTAINS(m_Container, key);
	}

	virtual TValueType& get(const TKeyType& key) override {
		return m_Container.find(key)->second;
	}

	virtual const TValueType& get(const TKeyType& key) const override {
		return m_Container.find(key)->second;
	}

	virtual void resize(const size_t amt, std::function<void(TPair<TKeyType, TValueType>&)> func) override {
		for (size_t i = getSize(); i < amt; ++i) {
			TPair<TKeyType, TValueType> pair;
			func(pair);
			m_Container.emplace(std::move(pair.key), std::move(pair.value));
		}
	}

	virtual void reserve(size_t amt) override {
		m_Container.reserve(amt);
	}

	virtual TPair<TKeyType, const TValueType&> push() override {
		m_Container.emplace();
		return top();
	}

	virtual TValueType& push(const TKeyType& key) override {
		push(TPair<TKeyType, TValueType>{key, {}});
		return get(key);
	}

	virtual TValueType& push(const TKeyType& key, const TValueType& value) override {
		IF_CONSTEXPR (std::is_copy_constructible_v<TValueType>) {
			push(TPair<TKeyType, TValueType>{key, value});
			return get(key);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual TValueType& push(const TKeyType& key, TValueType&& value) override {
		IF_CONSTEXPR (std::is_move_constructible_v<TValueType>) {
			push(TPair<TKeyType, TValueType>{key, std::move(value)});
			return get(key);
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void push(const TPair<TKeyType, TValueType>& pair) override {
		IF_CONSTEXPR (std::is_copy_constructible_v<TValueType>) {
			m_Container.emplace(pair.key, pair.value);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void push(TPair<TKeyType, TValueType>&& pair) override {
		IF_CONSTEXPR (std::is_move_constructible_v<TValueType>) {
			m_Container.emplace(std::move(pair.key), std::move(pair.value));
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void replace(const TKeyType& key, const TValueType& obj) override {
		IF_CONSTEXPR (std::is_copy_constructible_v<TValueType>) {
			pop(key);
			push(TPair<TKeyType, TValueType>{key, obj});
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void replace(const TKeyType& key, TValueType&& obj) override {
		IF_CONSTEXPR (std::is_move_constructible_v<TValueType>) {
			pop(key);
			push(TPair<TKeyType, TValueType>{key, std::move(obj)});
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void clear() override {
		m_Container.clear();
	}

	virtual void pop() override {
		pop(m_Container.begin()->first);
	}

	virtual void pop(const TKeyType& key) override {
		m_Container.erase(key);
	}

	virtual void forEach(const std::function<void(TPair<TKeyType, const TValueType&>)>& func) override {
		for (auto itr = m_Container.begin(); itr != m_Container.end(); ++itr) {
			func(TPair<TKeyType, const TValueType&>{itr->first, itr->second});
		}
	}

protected:

	struct Hasher {
		size_t operator()(const TKeyType& p) const noexcept {
			return getHash(p);
		}
	};

	std::unordered_multimap<TKeyType, TValueType, Hasher> m_Container;
};