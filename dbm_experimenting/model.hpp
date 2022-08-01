#ifndef DBM_MODEL_HPP
#define DBM_MODEL_HPP

#include "detail/model_item.hpp"

namespace dbm /* ::kind */ {
template<class... ContainerTypes>
class model
{
public:

    using tuple_type = std::tuple<ContainerTypes...>;
    using model_item_base = ::dbm::kind::model_item_base;

    static constexpr size_t length = sizeof...(ContainerTypes);

    constexpr model(ContainerTypes... items)
        : items_(std::forward<ContainerTypes>(items)...)
    {
        validate_containers2();
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
    constexpr void validate_containers2()
    {
        if constexpr(idx < length) {
            static_assert(std::is_base_of_v<model_item_base, std::tuple_element_t<idx, tuple_type>>,
                          "Model parameter should be derived from model_item_base");
            validate_containers2<idx+1>();
        }
    }

    tuple_type items_;
};


} // namespace dbm::kind

#endif //DBM_MODEL_HPP
