#pragma once

#include <algorithm>
#include "Container.h"

// Priority Vector is a vector that makes itself into a max heap, it is guaranteed the top value is always the largest
template <typename TType>
struct TMinHeap : TVector<TType> {

	virtual void resize(size_t amt) override {
		TVector<TType>::resize(amt);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
	}

	virtual void resize(size_t amt, std::function<TType(size_t)> func) override {
		TVector<TType>::resize(amt, func);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
	}

	virtual TType& push() override {
		TVector<TType>::push();
		std::push_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
		return TVector<TType>::get(TVector<TType>::getSize() - 1);
	}

	virtual size_t push(const TType& obj) override {
		TVector<TType>::push(obj);
		std::push_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
		return TVector<TType>::getSize() - 1;
	}

	virtual size_t push(TType&& obj) override {
		TVector<TType>::push(std::forward<TType>(obj));
		std::push_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
		return TVector<TType>::getSize() - 1;
	}

	virtual void pop() override {
		std::pop_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
		TVector<TType>::pop();
	}

	virtual void pop(const TType& obj) override {
		TVector<TType>::pop(obj);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
	}

protected:

	virtual void push(const size_t index, const TType& obj) override {
		push(obj);
	}

	virtual void push(const size_t index, TType&& obj) override {
		push(std::forward<TType>(obj));
	}

	virtual void pop(const size_t index) override {
		TVector<TType>::pop(index);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), MinCmp{});
	}

	struct MinCmp {
		bool operator()(const TType& a, const TType& b) const {
			return b < a;
		}
	};
};