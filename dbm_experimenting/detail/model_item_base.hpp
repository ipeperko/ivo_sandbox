#ifndef DBM_MODEL_ITEM_BASE_HPP
#define DBM_MODEL_ITEM_BASE_HPP

#include "common.hpp"

namespace dbm {

namespace kind {

namespace detail {
struct model_item_param_base_tag
{
};

template<typename T, typename PhantomParameter>
class model_item_parameter : model_item_param_base_tag
{
public:
    constexpr explicit model_item_parameter(T str)
        : value_ { str }
    {}

    constexpr T value() const { return value_; }

private:
    T value_ {};
};

struct key_phantom_parameter
{
};
struct tag_phantom_parameter
{
};
struct taggable_phantom_parameter
{
};
struct primary_phantom_parameter
{
};
struct defined_phantom_parameter
{
};
struct null_phantom_parameter
{
};

} // namespace detail

using key = detail::model_item_parameter<std::string_view, detail::key_phantom_parameter>;
using tag = detail::model_item_parameter<std::string_view, detail::tag_phantom_parameter>;
using taggable = detail::model_item_parameter<bool, detail::taggable_phantom_parameter>;
using primary = detail::model_item_parameter<bool, detail::primary_phantom_parameter>;
using defined = detail::model_item_parameter<bool, detail::defined_phantom_parameter>;
using null = detail::model_item_parameter<bool, detail::null_phantom_parameter>;

class model_item_base
{
public:
    auto const& key() const { return key_; }

    auto const& tag() const { return tag_; }

    auto taggable() const { return taggable_; }

    auto primary() const { return primary_; }

    auto defined() const { return defined_; }

    auto null() const { return null_; }

    virtual variant get() const = 0;

    virtual void set(variant& var) = 0;

    void set(variant&& var)
    {
        set(var);
    }

protected:
    std::string_view key_;
    std::string_view tag_;
    bool taggable_ { true };
    bool primary_ { false };
    bool defined_ { false };
    bool null_ { false };
};

} // namespace kind

} // namespace dbm

#endif //DBM_MODEL_ITEM_BASE_HPP
