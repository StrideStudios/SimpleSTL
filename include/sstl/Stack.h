#pragma once

#include "Deque.h"

// std::stack is already a wrapper around a deque, so we do that here too, and skip std::stack's own abstraction
template <typename TType>
struct TStack : TDeque<TType> {

	virtual void resize(const size_t amt, std::function<TType(size_t)> func) override {
		const size_t previousSize = TDeque<TType>::getSize();
		for (size_t i = previousSize; i < amt; ++i) {
			TDeque<TType>::m_Container.emplace_front(std::forward<TType>(func(i)));
		}
	}

	virtual TType& push() override {
		TDeque<TType>::m_Container.emplace_front();
		return TDeque<TType>::top();
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			TDeque<TType>::m_Container.emplace_front(obj);
			return 0;
		} else {
			throw std::runtime_error("Type is not copyable");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			TDeque<TType>::m_Container.emplace_front(std::move(obj));
			return 0;
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
		TDeque<TType>::push(index, std::move(obj));
	}

	virtual void pop(const size_t index) override {
		TDeque<TType>::pop(index);
	}
};