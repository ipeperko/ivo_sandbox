#ifndef DBM_COMMON_HPP
#define DBM_COMMON_HPP

#include <variant>
#include <tuple>
#include <functional>
#include <string>
#include <stdexcept>

namespace dbm {

template <
    size_t Index = 0, // start iteration at 0 index
    typename TTuple,  // the tuple type
    size_t Size =
        std::tuple_size_v<
            std::remove_reference_t<TTuple>>, // tuple size
    typename TCallable, // the callable to bo invoked for each tuple item
    typename... TArgs   // other arguments to be passed to the callable
    >
void for_each(TTuple&& tuple, TCallable&& callable, TArgs&&... args)
{
    if constexpr (Index < Size)
    {
        if constexpr (std::is_assignable_v<bool&, std::invoke_result_t<TCallable&&, TArgs&&..., decltype(std::get<Index>(tuple))>>)
        {
            if (!std::invoke(callable, args..., std::get<Index>(tuple)))
                return;
        }
        else
        {
            std::invoke(callable, args..., std::get<Index>(tuple));
        }

        if constexpr (Index + 1 < Size)
            for_each<Index + 1>(
                std::forward<TTuple>(tuple),
                std::forward<TCallable>(callable),
                std::forward<TArgs>(args)...);
    }
}

using variant = std::variant<std::nullptr_t,
                             bool,
                             int32_t,
                             int16_t,
                             int64_t,
                             uint32_t,
                             uint16_t,
                             uint64_t,
                             //        detail::timestamp2u_converter,
                             double,
                             std::string,
                             const char*,
                             std::string_view
#ifdef DBM_EXPERIMENTAL_BLOB
                             , kind::blob
#endif
                             >;


} // namespace dbm

#endif //DBM_COMMON_HPP
