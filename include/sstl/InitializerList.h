#pragma once

#include <type_traits>

namespace sstl {
    template <class TType, class... TArgs>
    struct EnforceConvertible {
        static_assert(std::conjunction_v<std::is_convertible<TType, TArgs>...>, "An argument in TArgs is not convertible to TType!");
        using Type = TType;
    };
}

template <class TType>
class TInitializerList {
public:
    constexpr TInitializerList() noexcept : m_fst(nullptr), m_lst(nullptr) {}

    constexpr TInitializerList(const TType* fst, const TType* lst) noexcept
        : m_fst(fst), m_lst(lst) {}

    _NODISCARD constexpr const TType* begin() const noexcept {
        return m_fst;
    }

    _NODISCARD constexpr const TType* end() const noexcept {
        return m_lst;
    }

    _NODISCARD constexpr size_t size() const noexcept {
        return static_cast<size_t>(m_lst - m_fst);
    }

private:
    const TType* m_fst;
    const TType* m_lst;
};

template <typename TType, typename... TArgs>
TInitializerList(TType, TArgs...) -> TInitializerList<typename sstl::EnforceConvertible<TType, TArgs...>::Type>;
