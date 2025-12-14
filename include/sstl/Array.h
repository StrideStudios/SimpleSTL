#pragma once

#include <array>
#include "Container.h"

template <typename TType, size_t TSize>
struct TArray : TSequenceContainer<TType, TSize> {

	TArray() {
		TArray::clear();
	}

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

	virtual bool contains(size_t index) const override {
		return m_IsPopulated[index];
	}

	virtual bool contains(const TType& obj) const override {
		return CONTAINS(m_Container, obj);
	}

	virtual TType& get(size_t index) override {
		return m_Container[index];
	}

	virtual const TType& get(size_t index) const override {
		return m_Container[index];
	}

	virtual void resize(size_t amt) override {
		for (size_t i = 0; i < getSize(); ++i) {
			if (!m_IsPopulated[i]) {
				m_Container[i] = {};
				m_IsPopulated[i] = true;
			}
		}
	}

	virtual void resize(const size_t amt, std::function<void(TType&, size_t)> func) override {
		for (size_t i = 0; i < getSize(); ++i) {
			if (!m_IsPopulated[i]) {
				func(get(i), i);
				m_IsPopulated[i] = true;
			}
		}
	}

	virtual TType& push() override {
		return get(push(TType{}));
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			for (size_t i = 0; i < getSize(); ++i) {
				if (!m_IsPopulated[i]) { //is not populated
					m_IsPopulated[i] = true;
					m_Container[i] = obj;
					return i;
				}
			}
			throw std::runtime_error("Array is full, cannot add any more elements.");
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			for (size_t i = 0; i < getSize(); ++i) {
				if (!m_IsPopulated[i]) { //is not populated
					m_IsPopulated[i] = true;
					m_Container[i] = std::move(obj);
					return i;
				}
			}
			throw std::runtime_error("Array is full, cannot add any more elements.");
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void push(const size_t index, const TType& obj) override {
		replace(index, obj);
	}

	virtual void push(const size_t index, TType&& obj) override {
		replace(index, std::move(obj));
	}

	virtual void replace(const size_t index, const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			m_Container[index] = obj;
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void replace(const size_t index, TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			m_Container[index] = std::move(obj);
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	// Array is on stack, it is destroyed when out of scope
	virtual void clear() override {
		m_IsPopulated.fill(false);
	}

	virtual void pop() override {
		for (size_t i = getSize(); i > 0; --i) {
			if (m_IsPopulated[i - 1]) { //is populated
				m_IsPopulated[i - 1] = false;
				return;
			}
		}
		throw std::runtime_error("No element to be popped!");
	}

	virtual void pop(const size_t index) override {
		if (m_IsPopulated[index]) { //is populated
			m_IsPopulated[index] = false;
			return;
		}
		throw std::runtime_error("No element at index to be popped!");
	}

	virtual void pop(const TType& obj) override {
		forEach([&](size_t index, TType& otr) {
			if (obj == otr) {
				m_IsPopulated[index] = false;
			}
		});
	}

	virtual void forEach(const std::function<void(size_t, TType&)>& func) override {
		size_t i = 0;
		for (auto itr = m_Container.begin(); itr != m_Container.end(); ++itr, ++i) {
			func(i, *itr);
		}
	}

	virtual void forEach(const std::function<void(size_t, const TType&)>& func) const override {
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

	virtual void forEachReverse(const std::function<void(size_t, const TType&)>& func) const override {
		size_t i = getSize() - 1;
		for (auto itr = m_Container.rbegin(); itr != m_Container.rend(); ++itr, --i) {
			func(i, *itr);
		}
	}

protected:

	std::array<bool, TSize> m_IsPopulated;
	std::array<TType, TSize> m_Container;
};