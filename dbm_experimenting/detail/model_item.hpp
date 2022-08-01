#ifndef DBM_MODEL_ITEM_HPP
#define DBM_MODEL_ITEM_HPP

#include "model_item_base.hpp"

namespace dbm {

namespace kind {

namespace detail {

template<typename T>
struct model_item_traits
{
    using value_type = T;
    using dereferenced_type = std::remove_reference_t<T>;
    using reference_type = std::add_rvalue_reference_t<dereferenced_type>;

    static constexpr bool is_reference = std::is_reference_v<T>;
};

} // namespace detail

template<typename T, typename Traits = detail::model_item_traits<T>>
class model_item : public model_item_base
{
public:
    using traits = Traits;
    using dereferenced_type = typename traits::dereferenced_type;
    using reference_type = typename traits::reference_type;
    using const_reference_type = const reference_type;

    model_item() = delete;

    template <typename Dummy = bool, std::enable_if_t<not traits::is_reference, Dummy> = false, typename... Parms>
    constexpr model_item(T&& val, std::tuple<Parms...>&& parms)
        : val_(std::forward<T>(val))
    {
        set_helper(parms);
    }

    template <typename Dummy = bool, std::enable_if_t<traits::is_reference, Dummy> = false, typename... Parms>
    constexpr model_item(T val, std::tuple<Parms...>&& parms)
        : val_(val)
    {
        set_helper(parms);
    }

    const dereferenced_type& value() const { return val_; }
    void value(dereferenced_type&& val) { val_ = val; }

    void set(::dbm::kind::key const& v)
    {
        key_ = v.value();
    }

    void set(::dbm::kind::tag const& v)
    {
        tag_ = v.value();
    }

    void set(::dbm::kind::taggable const& v)
    {
        taggable_ = v.value();
    }

    void set(::dbm::kind::primary const& v)
    {
        primary_ = v.value();
    }

    void set(::dbm::kind::defined const& v)
    {
        defined_ = v.value();
    }

    void set(::dbm::kind::null const& v)
    {
        null_ = v.value();
    }

    using model_item_base::key;
    using model_item_base::tag;
    using model_item_base::primary;

    auto& key(std::string_view val)
    {
        key_ = val;
        return *this;
    }

    auto& tag(std::string_view val)
    {
        tag_ = val;
        return *this;
    }

    auto& primary(bool val)
    {
        primary_ = val;
        return *this;
    }

    variant get() const override
    {
        return {val_};
    }

    void set(variant& var) override
    {
        val_ = std::move(std::get<dereferenced_type>(var));
    }

private:

    template<size_t Index = 0, typename... Args>
    void set_helper(std::tuple<Args...> const& tup)
    {
        if constexpr(Index < sizeof...(Args)) {
            set(std::get<Index>(tup));
            set_helper<Index + 1>(tup);
        }
    }

    T val_;
};

} // namespace kind


/*, typename std::enable_if_t<not std::is_reference_v<T>>*/
template<typename T, typename... Args>
auto local(T&& val = {}, Args&&... args)
{
    using value_type = std::decay_t<T>;
    return kind::model_item(std::forward<value_type>(val), std::tuple<Args...>{std::forward<Args>(args)...});
}

template<typename T, typename... Args/*, typename std::enable_if_t<std::is_reference_v<T>>*/>
auto binding(T& val, Args&&... args)
{
    return kind::model_item<T&>(val, std::tuple<Args...>{std::forward<Args>(args)...});
}

//template<typename T>
//auto local(T&& val = {})
//{
//    using value_type = std::decay_t<T>;
//    return kind::model_item(std::forward<value_type>(val), std::tuple<>{});
//}

} // namespace dbm



#endif //DBM_MODEL_ITEM_HPP
