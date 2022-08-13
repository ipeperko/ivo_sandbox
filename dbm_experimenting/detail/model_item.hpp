#ifndef DBM_MODEL_ITEM_HPP
#define DBM_MODEL_ITEM_HPP

#include "model_item_base.hpp"

namespace dbm {

namespace kind {

namespace detail {

template<typename T>
struct dummy_validator
{
    bool operator()(T const& val [[maybe_unused]])
    {
        return true;
    }
};

template<typename T>
struct model_item_traits
{
    using value_type = T;
    using dereferenced_type = std::remove_reference_t<T>;
    using reference_type = std::add_rvalue_reference_t<dereferenced_type>;

    static constexpr bool is_reference = std::is_reference_v<T>;
};

} // namespace detail

template<
        typename T,
        typename Validator = detail::dummy_validator<std::decay_t<T>>,
        typename Traits = detail::model_item_traits<T>
        >
class model_item : public model_item_base
{
public:
    using traits = Traits;
    using dereferenced_type = typename traits::dereferenced_type;
    using reference_type = typename traits::reference_type;
    using const_reference_type = const reference_type;
    using validator_type = Validator;

    model_item() = delete;

    template <
            typename Dummy = bool,
            std::enable_if_t<not traits::is_reference, Dummy> = false
//                    ,
            //typename... Parms
            >
    constexpr model_item(T&& val, validator_type validator = {} /*, std::tuple<Parms...>&& parms */)
        : val_(std::forward<T>(val))
        , validator_(validator)
    {
        // set_helper(parms);
    }

    template <
            typename Dummy = bool,
            std::enable_if_t<traits::is_reference, Dummy> = false
//                    ,
//            typename... Parms
            >
    constexpr model_item(T val, validator_type validator = {} /*, std::tuple<Parms...>&& parms */)
        : val_(val)
        , validator_(validator)
    {
        // set_helper(parms);
    }

    // Get value
    const dereferenced_type& value() const { return val_; }

    // Set value
    void value(dereferenced_type&& val) { val_ = val; }

    // Get variant value
    variant get() const override
    {
        return {val_};
    }

    // Set value from variant
    void set(variant& var) override
    {
        val_ = std::move(std::get<dereferenced_type>(var));
    }

    // Setup methods (named types)
//    void set(::dbm::kind::key const& v)
//    {
//        key_ = v.value();
//    }
//
//    void set(::dbm::kind::tag const& v)
//    {
//        tag_ = v.value();
//    }

//    void set(::dbm::kind::taggable const& v)
//    {
//        taggable_ = v.value();
//    }
//
//    void set(::dbm::kind::primary const& v)
//    {
//        primary_ = v.value();
//    }
//
//    void set(::dbm::kind::defined const& v)
//    {
//        defined_ = v.value();
//    }
//
//    void set(::dbm::kind::null const& v)
//    {
//        is_null_ = v.value();
//    }

    // Alternative setup methods (chaining)
    using model_item_base::key;
    using model_item_base::tag;
    using model_item_base::required;
    using model_item_base::taggable;
    using model_item_base::primary;
    using model_item_base::not_null;
    using model_item_base::auto_increment;
    using model_item_base::defined;
    using model_item_base::is_null;

    constexpr auto& key(std::string_view val)
    {
        key_ = val;
        return *this;
    }

    constexpr auto& tag(std::string_view val)
    {
        tag_ = val;
        return *this;
    }

    constexpr auto& required(bool val)
    {
        required_ = val;
        return *this;
    }

    constexpr auto& taggable(bool val)
    {
        taggable_ = val;
        return *this;
    }

    constexpr auto& primary(bool val)
    {
        primary_ = val;
        return *this;
    }

    constexpr auto& not_null(bool val)
    {
        not_null_ = val;
        return *this;
    }

    constexpr auto& auto_increment(bool val)
    {
        auto_increment_ = val;
        return *this;
    }

    constexpr auto& defined(bool val)
    {
        defined_ = val;
        return *this;
    }

    constexpr auto& is_null(bool val)
    {
        is_null_ = val;
        return *this;
    }

    constexpr auto& validator(std::function<bool(dereferenced_type const&)>&& validator)
    {
        validator2_ = std::move(validator);
        return *this;
    }

private:

//    template<size_t Index = 0, typename... Args>
//    void set_helper(std::tuple<Args...> const& tup)
//    {
//        if constexpr(Index < sizeof...(Args)) {
//            set(std::get<Index>(tup));
//            set_helper<Index + 1>(tup);
//        }
//    }

    T val_;
    validator_type validator_;
    std::function<bool(dereferenced_type const&)> validator2_;
};

} // namespace kind

// Helper function for creating a model item with local container
template<
        typename T
//        ,
//        typename... Args
        , typename Validator = kind::detail::dummy_validator<std::decay_t<T>>
        >
auto local(T&& val = {}, Validator validator = {} /*, Args&&... args*/)
{
    using value_type = std::decay_t<T>;

    return kind::model_item<
            value_type,
            Validator>
            (std::forward<value_type>(val), validator /*, std::make_tuple(std::forward<Args>(args)...) */);
}

// Helper function for creating a model item with binding
template<
        typename T
        , typename Validator = kind::detail::dummy_validator<T>
//        ,
//        typename... Args
        >
auto binding(T& val, Validator validator = {} /*, Args&&... args */)
{
    return kind::model_item<
            T&,
            Validator>
            (val, validator /*, std::make_tuple(std::forward<Args>(args)...) */);
}

} // namespace dbm

#endif //DBM_MODEL_ITEM_HPP
