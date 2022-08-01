#include <iostream>
#include "mysql/mysql.h"
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

template<typename T>
struct is_string_type : public std::integral_constant<bool, std::is_constructible<std::string, T>::value>
{
};

template <typename T>
inline constexpr bool is_string_type_v = is_string_type<T>::value;

template<typename T>
struct is_string_constructible_but_not_std_string :
        public std::integral_constant<
            bool,
            std::is_constructible_v<std::string, T> and
            not std::is_same_v<T, std::string>
            >
{
};

template <typename T>
inline constexpr bool is_string_constructible_but_not_std_string_v = is_string_constructible_but_not_std_string<T>::value;

class A
{
public:
    A() = default;

//    template<typename T, typename = typename std::enable_if<std::is_same_v<T, std::string>>::type>
//    explicit A(T const& v)
    explicit A(std::string const& s)
    {
        std::cout << "std::string ctor " << s <<'\n';
    }

//    template<typename T, typename = typename std::enable_if_t<is_string_constructible_but_not_std_string_v<T>>>
    template<typename T, typename = typename std::enable_if_t<is_string_type_v<T> and not std::is_same_v<T, std::string>>>
    explicit A(T const& v)
    {
        std::cout << "not std::string ctor " << v << "\n";
    }

//    template<typename T, typename = std::is_same<T, std::string>::type>
//    explicit A(T const& v)
//    {
//        std::cout << "not std::string ctor\n";
//    }

//    A(int val)
//        : a_(val)
//    {}

    A(const A& oth)
    {
        a_ = oth.a_;
    }

private:
    int a_;
};

template <template <typename...> class C, typename...Ts>
std::true_type is_base_of_template_impl(const C<Ts...>*);

template <template <typename...> class C>
std::false_type is_base_of_template_impl(...);

template <typename T, template <typename...> class C>
using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T*>()));

template <class D>
struct B {

    ~B() {
        std::cout << "B<D> dtor\n";
        static_cast<D*>(this)->~D();
    }
};

struct D : public B<D>
{
    ~D() {
        std::cout << "D dtor\n";
    }
};




#include <boost/stacktrace.hpp>

void foo(int bar) {
    std::cout << boost::stacktrace::stacktrace();
}

void bar() {
    foo(1);
}


int main()
{
    {
        bar();
        return 0;
    }

    {
        std::chrono::milliseconds wait_stat_step(100);
        std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(101ms);
        std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();
        auto const dur = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
        long rem = dur % wait_stat_step.count();
        long range = dur / wait_stat_step.count();
        range *= wait_stat_step.count();
        if (rem)
            range += wait_stat_step.count();
        std::cout << dur << " range " << range << " rem " << rem << "\n";
    }
    return 0;

    {
        B<D> inv;
//        B<D>* a = new D;
//        delete a;
//        D d;
    }


    return 0;


    std::string s = "string";
    const char* cc = "neki";
    std::string_view sv = "sview";

    A a(s);
    A(std::string("1"));
    A("1");
    A {cc};
    A {sv};


    std::cout << is_string_constructible_but_not_std_string<std::string>::value << "\n";
    std::cout << is_string_constructible_but_not_std_string<const char*>::value << "\n";

    return 0;

#ifdef TEST_OPTION
    std::cout << "[ok] Test option enabled\n";
    return 0;
#else
    std::cout << "[error] Test option disabled\n";
    return 1;
#endif
}