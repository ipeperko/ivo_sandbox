#ifndef DBM_MODEL_HPP
#define DBM_MODEL_HPP

#include "detail/model_item.hpp"

namespace dbm {

//template<class... ContainerTypes>
//class columns
//{
//public:
//    using tuple_type = std::tuple<ContainerTypes...>;
//
//    constexpr explicit columns(ContainerTypes&&... item)
//        : items_(std::forward<ContainerTypes>(item)...)
//    {
//
//    }
//
////    constexpr explicit items(tuple_type&& tup)
////        : items_(std::forward<tuple_type>(tup))
////    {
////
////    }
//
//    tuple_type items_;
//};


template<
        class... ContainerTypes
        >
class model
{
public:

    using tuple_type = std::tuple<ContainerTypes...>;
    using model_item_base = ::dbm::kind::model_item_base;

    static constexpr size_t length = sizeof...(ContainerTypes);

//    constexpr
//    model(std::string_view table_name, columns<ContainerTypes...>&& columns)
//        : items_(std::forward<tuple_type>(columns.items_))
//        , table_name_(table_name)
//    {
//        validate_containers();
//    }

//    constexpr model(std::string_view table_name, ContainerTypes... items)
//        : items_(std::forward<ContainerTypes>(items)...)
//        , table_name_(table_name)
//    {
//        validate_containers();
//    }

    constexpr
    model(std::string_view table_name, tuple_type&& items)
        : items_(std::forward<tuple_type>(items))
        , table_name_(table_name)
    {
        validate_containers();
    }

    //    constexpr size_t size() const { return sizeof...(ContainerTypes); }

    constexpr auto& items() { return items_; }
    constexpr auto const& items() const { return items_; }

    model_item_base* find(std::string_view key)
    {
        model_item_base* t = nullptr;

        for_each(items_, [&](auto& item) {
            if (item.key() == key) {
                t = &item;
                return false;
            }
            return true;
        });

        return t;
    }

    model_item_base const* find(std::string_view key) const
    {
        return const_cast<model const*>(this)->find(key);
    }

    model_item_base* find(::dbm::kind::key key)
    {
        return find(key.value());
    }

    model_item_base const* find(::dbm::kind::key key) const
    {
        return find(key.value());
    }

    model_item_base* find(::dbm::kind::tag tag)
    {
        model_item_base* t = nullptr;

        for_each(items_, [&](auto& item) {
            if (item.tag() == tag) {
                t = &item;
                return false;
            }
            return true;
        });

        return t;
    }

    model_item_base const* find(::dbm::kind::tag tag) const
    {
        return const_cast<model const*>(this)->find(tag);
    }

    model_item_base& get(std::string_view key)
    {
        auto* t = find(key);
        if (!t)
            throw std::domain_error("No such element with key=" + std::string(key));
        return *t;
    }

    model_item_base const& get(std::string_view key) const
    {
        return const_cast<model const*>(this)->get(key);
    }

    model_item_base& get(::dbm::kind::key key)
    {
        return get(key.value());
    }

    model_item_base const& get(::dbm::kind::key key) const
    {
        return get(key.value());
    }

    model_item_base& get(::dbm::kind::tag tag)
    {
        auto* t = find(tag);
        if (!t)
            throw std::domain_error("No such element with tag=" + std::string(tag.value()));
        return *t;
    }

    model_item_base const& get(::dbm::kind::tag tag) const
    {
        return const_cast<model const*>(this)->get(tag);
    }

private:
    template<size_t idx = 0>
    constexpr void validate_containers()
    {
        if constexpr(idx < length) {
            static_assert(std::is_base_of_v<model_item_base, std::tuple_element_t<idx, tuple_type>>,
                          "Model parameter should be derived from model_item_base");
            validate_containers<idx + 1>();
        }
    }

    tuple_type items_;
    std::string_view table_name_;
};

template<class... ContainerTypes>
inline auto make_columns(ContainerTypes&&... item)
{
    return std::make_tuple(std::forward<ContainerTypes>(item)...);
}

template<class... ContainerTypes>
inline auto make_model(std::string_view table_name, std::tuple<ContainerTypes...>&& items)
{
    model m(table_name, std::forward<std::tuple<ContainerTypes...>>(items));
    return m;
}



} // namespace dbm

#endif //DBM_MODEL_HPP
