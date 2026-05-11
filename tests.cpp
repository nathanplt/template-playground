#include "metafunctions.hpp"

#include <iostream>
#include <tuple>
#include <type_traits>

int main() {
    /* conditional_t tests */
    {
        using A1 = int;
        using B1 = char;

        static_assert(is_same_v<conditional_t<true, A1, B1>, A1>);

        using A2 = double;
        using B2 = bool;

        static_assert(is_same_v<conditional_t<false, A2, B2>, B2>);
    }

    /* push_back_t tests */
    {
        using L1 = meta_list<int, double>;
        using R1 = push_back_t<L1, char>;

        static_assert(is_same_v<R1, meta_list<int, double, char>>);

        using L2 = std::tuple<int, char>;
        using R2 = push_back_t<L2, double>;

        static_assert(is_same_v<R2, std::tuple<int, char, double>>);
    }

    /* push_front_t tests */
    {
        using L1 = meta_list<int, double>;
        using R1 = push_front_t<L1, char>;

        static_assert(is_same_v<R1, meta_list<char, int, double>>);

        using L2 = std::tuple<int, char>;
        using R2 = push_front_t<L2, double>;

        static_assert(is_same_v<R2, std::tuple<double, int, char>>);
    }

    /* append_t tests */
    {
        using L1 = std::tuple<int, int, double>;
        using R1 = append_t<L1, meta_list<int, char, double>>;

        static_assert(is_same_v<R1, std::tuple<int, int, double, int, char, double>>);

        using L2 = meta_list<double>;
        using R2 = append_t<L2, std::tuple<int, char, char>>;

        static_assert(is_same_v<R2, meta_list<double, int, char, char>>);

        using L3 = meta_list<int>;
        using R3 = append_t<L3, meta_list<>>;

        static_assert(is_same_v<R3, meta_list<int>>);
    }

    /* c_sive_v tests */
    {
        using L1 = meta_list<int, double, char, int, double>;

        static_assert(c_size_v<L1> == 5);

        using L2 = meta_list<>;

        static_assert(c_size_v<L2> == 0);
    }

    /* lambda_t tests */
    {
        using L1 = std::tuple<int, int, double>;
        using R1 = lambda_t<append>::apply<L1, meta_list<int, char, double>>::type;

        static_assert(is_same_v<R1, std::tuple<int, int, double, int, char, double>>);

        using L2 = meta_list<int, double>;
        using R2 = lambda_t<push_back>::apply<L2, char>::type;

        static_assert(is_same_v<R2, meta_list<int, double, char>>);
    }

    /* transform_t tests */
    {
        using L1 = std::tuple<int*, double*, char*>;
        using R1 = transform_t<L1, lambda_t<remove_pointer>>;

        static_assert(is_same_v<R1, std::tuple<int, double, char>>);

        using L2 = meta_list<int*, double, char**, int***>;
        using R2 = transform_t<L2, lambda_t<add_pointer>>;

        static_assert(is_same_v<R2, meta_list<int**, double*, char***, int****>>);
    }

    /* filter_t tests */
    {
        using L1 = std::tuple<int, double, char>;
        using R1 = filter_t<L1, std::is_integral>;

        static_assert(is_same_v<R1, std::tuple<int, char>>);

        using L2 = meta_list<int*, double, char*, int, int*>;
        using R2 = filter_t<L2, std::is_arithmetic>;

        static_assert(is_same_v<R2, meta_list<double, int>>);
    }

    std::cout << "ALL TESTS PASSING!" << '\n';
}
