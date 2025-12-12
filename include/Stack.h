#pragma once

#include "Deque.h"

// std::stack is already a wrapper around a deque, so we do that here too, and skip std::stack's own abstraction
template <typename TType>
struct TStack : TDeque<TType> {

	virtual void resize(const size_t amt, std::function<void(TType&, size_t)> func) override {
		const size_t previousSize = TDeque<TType>::getSize();
		TDeque<TType>::resize(amt);
		for (size_t i = previousSize; i < TDeque<TType>::getSize(); ++i) {
			func(TDeque<TType>::get(i), i);
		}
	}

	virtual TType& push() override {
		TDeque<TType>::m_Container.emplace_front();
		return TDeque<TType>::top();
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (is_copyable_v<TType>) {
			TDeque<TType>::m_Container.emplace_front(obj);
			return 0;
		} else {
			throw std::runtime_error("Type is not copyable");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (is_moveable_v<TType>) {
			TDeque<TType>::m_Container.emplace_front(std::move(obj));
			return 0;
		} else {
			throw std::runtime_error("Type is not moveable");
		}
	}

	virtual void replace(const size_t index, const TType& obj) override {
		if constexpr (is_copyable_v<TType>) {
			TDeque<TType>::pop(index);
			TDeque<TType>::push(index, obj);
		} else {
			throw std::runtime_error("Type is not copyable");
		}
	}

	virtual void replace(const size_t index, TType&& obj) override {
		if constexpr (is_moveable_v<TType>) {
			TDeque<TType>::pop(index);
			TDeque<TType>::push(index, std::move(obj));
		} else {
			throw std::runtime_error("Type is not moveable");
		}
	}

	virtual void pop() override {
		TDeque<TType>::pop(0);
	}

	virtual void forEach(const std::function<void(size_t, TType&)>& func) override {
		size_t i = 0;
		for (auto itr = TDeque<TType>::m_Container.begin(); itr != TDeque<TType>::m_Container.end(); ++itr, ++i) {
			func(i, *itr);
		}
	}

	virtual void forEachReverse(const std::function<void(size_t, TType&)>& func) override {
		size_t i = TDeque<TType>::getSize() - 1;
		for (auto itr = TDeque<TType>::m_Container.rbegin(); itr != TDeque<TType>::m_Container.rend(); ++itr, --i) {
			func(i, *itr);
		}
	}

protected:

	virtual TType& get(size_t index) override {
		return TDeque<TType>::top();
	}

	virtual const TType& get(size_t index) const override {
		return TDeque<TType>::top();
	}

	virtual void push(const size_t index, const TType& obj) override {
		push(obj);
	}

	virtual void push(const size_t index, TType&& obj) override {
		push(obj);
	}

	virtual void pop(const size_t index) override {
		pop();
	}
};