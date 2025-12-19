#pragma once

#include "Deque.h"

// std::queue is already a wrapper around a deque, so we do that here too, and skip std::queue's own abstraction
template <typename TType>
struct TQueue : TDeque<TType> {

	virtual TType& top() override {
		return TDeque<TType>::m_Container.back();
	}

	virtual const TType& top() const override {
		return TDeque<TType>::m_Container.back();
	}

	virtual void resize(const size_t amt, std::function<void(TType&, size_t)> func) override {
		const size_t previousSize = TDeque<TType>::getSize();
		TDeque<TType>::resize(amt);
		for (size_t i = previousSize; i < TDeque<TType>::getSize(); ++i) {
			func(TDeque<TType>::get(i), i);
		}
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			TDeque<TType>::m_Container.emplace_back(obj);
			return TDeque<TType>::getSize() - 1;
		} else {
			throw std::runtime_error("Type is not copyable");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			TDeque<TType>::m_Container.emplace_back(std::forward<TType>(obj));
			return TDeque<TType>::getSize() - 1;
		} else {
			throw std::runtime_error("Type is not moveable");
		}
	}

	virtual void replace(const size_t index, const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			TDeque<TType>::pop(index);
			TDeque<TType>::push(index, obj);
		} else {
			throw std::runtime_error("Type is not copyable");
		}
	}

	virtual void replace(const size_t index, TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			TDeque<TType>::pop(index);
			TDeque<TType>::push(index, std::forward<TType>(obj));
		} else {
			throw std::runtime_error("Type is not moveable");
		}
	}

	virtual void pop() override {
		TDeque<TType>::pop(TDeque<TType>::getSize() - 1);
	}

	virtual void forEach(const std::function<void(size_t, TType&)>& func) override {
		size_t i = 0;
		for (auto itr = TDeque<TType>::m_Container.begin(); itr != TDeque<TType>::m_Container.end(); ++itr, ++i) {
			func(i, *itr);
		}
	}

	virtual void forEach(const std::function<void(size_t, const TType&)>& func) const override {
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

	virtual void forEachReverse(const std::function<void(size_t, const TType&)>& func) const override {
		size_t i = TDeque<TType>::getSize() - 1;
		for (auto itr = TDeque<TType>::m_Container.rbegin(); itr != TDeque<TType>::m_Container.rend(); ++itr, --i) {
			func(i, *itr);
		}
	}

protected:

	virtual TType& bottom() override {
		return TDeque<TType>::bottom();
	}

	virtual const TType& bottom() const override {
		return TDeque<TType>::bottom();
	}

	virtual TType& get(size_t index) override {
		return TDeque<TType>::get(index);
	}

	virtual const TType& get(size_t index) const override {
		return TDeque<TType>::get(index);
	}

	virtual void push(const size_t index, const TType& obj) override {
		TDeque<TType>::push(index, obj);
	}

	virtual void push(const size_t index, TType&& obj) override {
		TDeque<TType>::push(index, std::forward<TType>(obj));
	}

	virtual void pop(const size_t index) override {
		TDeque<TType>::pop(index);
	}
};