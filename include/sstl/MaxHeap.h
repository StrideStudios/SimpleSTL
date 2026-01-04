#pragma once

#include <algorithm>
#include "Vector.h"

// Priority Vector is a vector that makes itself into a max heap, it is guaranteed the top value is always the largest
template <typename TType>
struct TMaxHeap : TVector<TType> {

	TMaxHeap() = default;

	template <typename TOtherType = TType,
		std::enable_if_t<std::is_copy_constructible_v<TOtherType>, int> = 0
	>
	TMaxHeap(TInitializerList<TType> init): TVector<TType>(init) {}

	template <typename... TArgs,
		std::enable_if_t<std::conjunction_v<std::is_constructible<TType, TArgs>...>, int> = 0
	>
	explicit TMaxHeap(TArgs&&... args): TVector<TType>(std::forward<TArgs>(args)...) {}

	virtual void resize(size_t amt) override {
		TVector<TType>::resize(amt);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
	}

	virtual void resize(const size_t amt, std::function<TType(size_t)> func) override {
		TVector<TType>::resize(amt, func);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
	}

	virtual TType& push() override {
		TVector<TType>::push();
		std::push_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
		return TVector<TType>::get(TVector<TType>::getSize() - 1);
	}

	virtual size_t push(const TType& obj) override {
		TVector<TType>::push(obj);
		std::push_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
		return TVector<TType>::getSize() - 1;
	}

	virtual size_t push(TType&& obj) override {
		TVector<TType>::push(std::move(obj));
		std::push_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
		return TVector<TType>::getSize() - 1;
	}

	virtual void pop() override {
		std::pop_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
		TVector<TType>::pop();
	}

	virtual void pop(const TType& obj) override {
		TVector<TType>::pop(obj);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
	}

	virtual void pop(typename TUnfurled<TType>::Type* obj) override {
		if constexpr (sstl::is_managed_v<TType>) {
			ERASE(TVector<TType>::m_Container, obj, TUnfurled<TType>::get);
			std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
		} else {
			pop(*obj);
		}
	}

protected:

	virtual void push(const size_t index, const TType& obj) override {
		push(obj);
	}

	virtual void push(const size_t index, TType&& obj) override {
		push(std::move(obj));
	}

	virtual void pop(const size_t index) override {
		TVector<TType>::pop(index);
		std::make_heap(TVector<TType>::m_Container.begin(), TVector<TType>::m_Container.end(), std::less<TType>{});
	}
};

template <typename TType, typename... TArgs>
TMaxHeap(TType, TArgs...) -> TMaxHeap<typename sstl::EnforceConvertible<TType, TArgs...>::Type>;
