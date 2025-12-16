#pragma once

#include <forward_list>
#include "Container.h"

template <typename TType>
struct TForwardList : TSequenceContainer<TType> {

	[[nodiscard]] virtual size_t getSize() const override {
		return m_Size;
	}

	virtual TType& top() override {
		return m_Container.front();
	}

	virtual const TType& top() const override {
		return m_Container.front();
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
		m_Size = amt;
	}

	virtual void resize(const size_t amt, std::function<void(TType&, size_t)> func) override {
		const size_t previousSize = getSize();
		resize(amt);
		for (size_t i = previousSize; i < getSize(); ++i) {
			func(get(i), i);
		}
	}

	virtual TType& push() override {
		m_Container.emplace_front();
		m_Size++;
		return get(getSize() - 1);
	}

	virtual size_t push(const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			m_Container.emplace_front(obj);
			m_Size++;
			return getSize() - 1;
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual size_t push(TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			m_Container.emplace_front(std::move(obj));
			m_Size++;
			return getSize() - 1;
		} else {
			throw std::runtime_error("Type is not moveable!");
		}
	}

	virtual void push(const size_t index, const TType& obj) override {
		if constexpr (std::is_copy_constructible_v<TType>) {
			auto itr = m_Container.before_begin();
			std::advance(itr, index);
			m_Container.insert_after(itr, obj);
			m_Size++;
		} else {
			throw std::runtime_error("Type is not copyable!");
		}
	}

	virtual void push(const size_t index, TType&& obj) override {
		if constexpr (std::is_move_constructible_v<TType>) {
			auto itr = m_Container.before_begin();
			std::advance(itr, index);
			m_Container.insert_after(itr, std::move(obj));
			m_Size++;
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
		m_Size = 0;
	}

	virtual void pop() override {
		m_Container.erase_after(m_Container.before_begin());
		m_Size--;
	}

	virtual void pop(const size_t index) override {
		auto itr = m_Container.before_begin();
		std::advance(itr, index);
		m_Container.erase_after(itr);
		m_Size--;
	}

	virtual void pop(const TType& obj) override {
		m_Container.erase_after(std::remove(m_Container.before_begin(), m_Container.end(), obj), m_Container.end());
		m_Size--;
	}

	virtual void transfer(TSequenceContainer<TType>& otr, const size_t index) override {
		// Forward List transfer can use splicing
		if (auto otrList = dynamic_cast<TForwardList*>(&otr)) {
			auto itr = m_Container.before_begin();
			std::advance(itr, index);
			otrList->m_Container.splice_after(otrList->m_Container.before_begin(), m_Container, itr);
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

protected:

	std::forward_list<TType> m_Container;
	size_t m_Size = 0;
};
