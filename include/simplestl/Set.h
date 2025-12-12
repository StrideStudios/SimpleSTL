#pragma once

#include <unordered_set>
#include "Container.h"

template <typename TType>
struct TSet : TSingleAssociativeContainer<TType> {

	[[nodiscard]] virtual size_t getSize() const override {
		return m_Container.size();
	}

	virtual const TType& top() const override {
		return *m_Container.begin();
	}

	virtual void resize(const size_t amt) override {
		for (size_t i = getSize(); i < amt; ++i) {
			m_Container.emplace();
		}
	}

	virtual void resize(const size_t amt, std::function<void(TType&)> func) override {
		for (size_t i = getSize(); i < amt; ++i) {
			TType obj;
			func(obj);
			m_Container.emplace(std::move(obj));
		}
	}

	virtual void reserve(const size_t amt) override {
		m_Container.reserve(amt);
	}

	virtual const TType& push() override {
		m_Container.emplace();
		return top();
	}

	virtual void push(const TType& obj) override {
		if constexpr (is_copyable_v<TType>) {
			m_Container.emplace(obj);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void push(TType&& obj) override {
		if constexpr (is_moveable_v<TType>) {
			m_Container.emplace(std::move(obj));
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void replace(const TType& tgt, const TType& obj) override {
		if constexpr (is_copyable_v<TType>) {
			// Since this container is unordered, replacing doesn't need to set at the same index
			pop(tgt);
			m_Container.insert(obj);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void replace(const TType& tgt, TType&& obj) override {
		if constexpr (is_moveable_v<TType>) {// Since this container is unordered, replacing doesn't need to set at the same index
			pop(tgt);
			m_Container.insert(std::move(obj));
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void pop() override {
		m_Container.erase(m_Container.begin());
	}

	virtual void pop(const TType& obj) override {
		m_Container.erase(obj);
	}

	virtual void forEach(const std::function<void(const TType&)>& func) override {
		for (auto itr = m_Container.begin(); itr != m_Container.end(); ++itr) {
			func(*itr);
		}
	}

protected:

	struct Hasher {
		size_t operator()(const TType& p) const noexcept {
			return getHash(p);
		}
	};

	std::unordered_set<TType, Hasher> m_Container;
};