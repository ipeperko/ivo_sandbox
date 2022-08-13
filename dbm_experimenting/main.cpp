#include "model.hpp"
#include <iostream>


namespace dbm {
using key = kind::key;
using tag = kind::tag;
//using primary = kind::primary;

//template<class... ContainerTypes>
//using model = kind::model<ContainerTypes...>;
}

auto get_model(std::string& name, int& my_int, std::function<bool(int)> id_validator)
{
    using namespace dbm;

    auto m = make_model (
        "my_table",

        make_columns (
                    local(-1, std::move(id_validator))
                            .key("id")      .tag("ID")      .required(true)
                                    // current state
                            .defined(true) .is_null(false)
                                    // db settings
                            .primary(true) .not_null(true) .auto_increment(true),

                    binding(name, [] (auto const& name) { return !name.empty(); })
                            .key("name")    .tag("NAME")    .required(true),

                    binding(my_int)
                            .key("age")     .tag("AGE"),

                    local<double>()
                            .key("weight")  .taggable(false)
        )
    );

    return m;
}

int main() {
    using namespace dbm;

    std::cout << "is_reference int  : " << dbm::kind::detail::model_item_traits<int>::is_reference << "\n";
    std::cout << "is_reference int& : " << dbm::kind::detail::model_item_traits<int&>::is_reference << "\n";

//    model_item<int> mi_int;
//    std::cout << "is_reference mi_int  : " << decltype(mi_int)::traits::is_reference << "\n";
//
//    model_item mi_int_val(1);
//    std::cout << "is_reference mi_int_val  : " << decltype(mi_int_val)::traits::is_reference << "\n";

    int my_int = 1;
    int& my_int_ref = my_int;
    int const& my_int_cref = my_int;
//    model_item mi_int_ref(my_int);
//    std::cout << "is_reference mi_int_ref  : " << decltype(mi_int_ref)::traits::is_reference << "\n";


    auto print = [] (std::string_view name, const auto& container) {
        std::cout << name << " : " << container.key() << " | " << container.tag() << "\n  is_reference loc  : " << std::decay_t<decltype(container)>::traits::is_reference << "\n";
    };
    auto print_val = [] (const auto& container) {
        std::cout << "  val : " << container.value() << "\n";
    };

    auto loc = local<int>(0);
//    std::cout << "is_reference loc  : " << decltype(loc)::traits::is_reference << "\n";
    print("loc", loc);
    print_val(loc);

//    return 0;

    auto loc_val_direct = local(42);
    print("loc_val_direct", loc_val_direct);
    print_val(loc_val_direct);

    auto loc_val_copy = local(my_int);
    print("loc_val_copy", loc_val_copy);
    print_val(loc_val_copy);

    auto loc_val_ref = local(my_int_ref);
    print("loc_val_ref", loc_val_ref);
    print_val(loc_val_ref);

    auto loc_val_cref = local(my_int);
    print("loc_val_cref", loc_val_cref);
    print_val(loc_val_cref);

    auto binding_val = binding(my_int);
    print("binding_val", binding_val);
    print_val(binding_val);
    my_int = 2;
    print_val(binding_val);

    std::string name = "ivo";

    // takole bi lahko definirali vse parametre (tudi stringe) v compile time-u (razen validatorja?):
    // local(13,        key("id"),      tag("ID"),      primary(true))
    // binding(name,    key("name"),    tag("NAME")



    {
        auto id_validator = [](int const& id) {
            return id >= 1;
        };
        auto name_validator = [](const auto& val) {
            return !val.empty();
        };

//        local<int, decltype(id_validator)>();
        local<int>(1, id_validator);
        local(1, id_validator);


        model m {
                "my_table",

                make_columns
                        (
                    local(-1, id_validator)
                            .key("id")      .tag("ID")      .required(true)
                            // current state
                            .defined(true) .is_null(false)
                            // db settings
                            .primary(true) .not_null(true) .auto_increment(true),

                    binding(name, [] (auto const& name) { return !name.empty(); })
                            .key("name")    .tag("NAME")    .required(true),

                    binding(my_int)
                            .key("age")     .tag("AGE"),

                    local<double>()
                            .key("weight")  .taggable(false)
                        )
        };

        auto m2 = get_model(name, my_int, id_validator);

        auto& items = m2.items();

        print("tuple element ", std::get<0>(items));
        print_val(std::get<0>(items));

        print("tuple element ", std::get<1>(items));
        print_val(std::get<1>(items));

        print("tuple element ", std::get<2>(items));
        print_val(std::get<2>(items));

        std::cout << m.get("id").key() << "\n";
        std::cout << m.get("name").key() << "\n";
        std::cout << m.get("age").key() << "\n";
//
//        std::cout << "model size : " << m.length << "\n";
    }

    return 0;
}
