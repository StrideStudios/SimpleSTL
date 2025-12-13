#pragma once

#include <algorithm>
#include <vector>
#include "Container.h"

// Priority Vector is a vector that makes itself into a max heap, it is guaranteed the top value is always the largest
template <typename TType>
struct TMaxHeap : TSequenceContainer<TType> {

	[[nodiscard]] virtual size_t getSize() const override {
		return m_Container.size();
	}

	virtual TType& top() override {
		return m_Container.front();
	}

	virtual const TType& top() const override {
		return m_Container.front();
	}

	virtual TType& bottom() override {
		return m_Container.back();
	}

	virtual const TType& bottom() const override {
		return m_Container.back();
	}

	virtual TType& get(size_t index) override {
		return m_Container[index];
	}

	virtual const TType& get(size_t index) const override {
		return m_Container[index];
	}

	virtual void resize(size_t amt) override {
		m_Container.resize(amt);
		std::make_heap(m_Container.begin(), m_Container.end(), std::less<TType>{});
	}

	virtual void resize(const size_t amt, std::function<void(TType&, size_t)> func) override {
		const size_t previousSize = getSize();
		m_Container.resize(amt);
		for (size_t i = previousSize; i < getSize(); ++i) {
			func(get(i), i);
		}
		std::make_heap(m_Container.begin(), m_Container.end(), std::less<TType>{});
	}

	virtual void reserve(size_t amt) override {
		m_Container.reserve(amt);
	}

	virtual TType& push() override {
		m_Container.emplace_back();
		std::push_heap(m_Container.begin(), m_Container.end(), std::less<TType>{});
		return get(getSize() - 1);
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (is_copyable_v<TType>) {
			m_Container.push_back(obj);
			std::push_heap(m_Container.begin(), m_Container.end(), std::less<TType>{});
			return getSize() - 1;
		} else {
			throw std::runtime_error("Type is not copyable");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (is_moveable_v<TType>) {
			m_Container.push_back(std::move(obj));
			std::push_heap(m_Container.begin(), m_Container.end(), std::less<TType>{});
			return getSize() - 1;
		} else {
			throw std::runtime_error("Type is not moveable");
		}
	}

	virtual void pop() override {
		std::pop_heap(m_Container.begin(), m_Container.end(), std::less<TType>{});
		m_Container.pop_back();
	}

	virtual void forEach(const std::function<void(size_t, TType&)>& func) override {
		size_t i = 0;
		for (auto itr = m_Container.begin(); itr != m_Container.end(); ++itr, ++i) {
			func(i, *itr);
		}
	}

	virtual void forEachReverse(const std::function<void(size_t, TType&)>& func) override {
		size_t i = getSize() - 1;
		for (auto itr = m_Container.rbegin(); itr != m_Container.rend(); ++itr, --i) {
			func(i, *itr);
		}
	}

protected:

	virtual void push(const size_t index, const TType& obj) override {
		push(obj);
	}

	virtual void push(const size_t index, TType&& obj) override {
		push(std::move(obj));
	}

	virtual void replace(const size_t index, const TType& obj) override {
		if constexpr (is_copyable_v<TType>) {
			push(index, obj);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void replace(const size_t index, TType&& obj) override {
		if constexpr (is_moveable_v<TType>) {
			push(index, std::move(obj));
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void pop(const size_t index) override {
		pop();
	}

	std::vector<TType> m_Container;
};