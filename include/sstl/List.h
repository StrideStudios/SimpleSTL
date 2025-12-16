#pragma once

#include <list>
#include "Container.h"

template <typename TType>
struct TList : TSequenceContainer<TType> {

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

	virtual bool contains(const TType& obj) const override {
		return CONTAINS(m_Container, obj);
	}

	virtual size_t find(const TType& obj) const override {
		return DISTANCE(m_Container, obj);
	}

	virtual TType& get(size_t index) override {
		auto itr = m_Container.begin();
		std::advance(itr, index);
		return *itr;
	}

	virtual const TType& get(size_t index) const override {
		auto itr = m_Container.begin();
		std::advance(itr, index);
		return *itr;
	}

	virtual void resize(size_t amt) override {
		m_Container.resize(amt);
	}

	virtual void resize(const size_t amt, std::function<void(TType&, size_t)> func) override {
		const size_t previousSize = getSize();
		resize(amt);
		for (size_t i = previousSize; i < getSize(); ++i) {
			func(get(i), i);
		}
	}

	virtual TType& push() override {
		m_Container.emplace_back();
		return get(getSize() - 1);
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			m_Container.emplace_back(obj);
			return getSize() - 1;
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			m_Container.emplace_back(std::move(obj));
			return getSize() - 1;
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void push(const size_t index, const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			auto itr = m_Container.begin();
			std::advance(itr, index);
			m_Container.insert(itr, obj);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void push(const size_t index, TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			auto itr = m_Container.begin();
			std::advance(itr, index);
			m_Container.insert(itr, std::move(obj));
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void replace(const size_t index, const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			pop(index);
			push(index, obj);
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void replace(const size_t index, TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			pop(index);
			push(index, std::move(obj));
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void clear() override {
		m_Container.clear();
	}

	virtual void pop() override {
		m_Container.erase(m_Container.begin());
	}

	virtual void pop(const size_t index) override {
		auto itr = m_Container.begin();
		std::advance(itr, index);
		m_Container.erase(itr);
	}

	virtual void pop(const TType& obj) override {
		ERASE(m_Container, obj);
	}

	// List transfer can use splicing
	virtual void transfer(TSequenceContainer<TType>& otr, const size_t index) override {
		if (auto otrList = dynamic_cast<TList*>(&otr)) {
			auto itr = m_Container.begin();
			std::advance(itr, index);
			otrList->m_Container.splice(otrList->m_Container.begin(), m_Container, itr);
			return;
		}
		TSequenceContainer<TType>::transfer(otr, index);
	}

	virtual void forEach(const std::function<void(size_t, TType&)>& func) override {
		size_t i = 0;
		for (auto itr = m_Container.begin(); itr != m_Container.end(); std::advance(itr, 1), ++i) {
			func(i, *itr);
		}
	}

	virtual void forEach(const std::function<void(size_t, const TType&)>& func) const override {
		size_t i = 0;
		for (auto itr = m_Container.begin(); itr != m_Container.end(); std::advance(itr, 1), ++i) {
			func(i, *itr);
		}
	}

	virtual void forEachReverse(const std::function<void(size_t, TType&)>& func) override {
		size_t i = getSize() - 1;
		for (auto itr = m_Container.rbegin(); itr != m_Container.rend(); std::advance(itr, 1), --i) {
			func(i, *itr);
		}
	}

	virtual void forEachReverse(const std::function<void(size_t, const TType&)>& func) const override {
		size_t i = getSize() - 1;
		for (auto itr = m_Container.rbegin(); itr != m_Container.rend(); std::advance(itr, 1), --i) {
			func(i, *itr);
		}
	}

protected:

	std::list<TType> m_Container;
};