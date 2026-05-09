#include <iostream>
#include <tuple>
#include <type_traits>

// -----------------------------------------------------------------------------
// integral_constant
// -----------------------------------------------------------------------------

template <typename T, T V>
struct integral_constant {
    using value_type = T;
    using type = integral_constant<T, V>;
    static constexpr T value = V;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// -----------------------------------------------------------------------------
// lambda
// -----------------------------------------------------------------------------

template <template <typename...> typename MetaFunction>
struct lambda {
    struct type {
        template <typename... Args>
        struct apply {
            using type = typename MetaFunction<Args...>::type;
        };
    };
};

template <template <typename...> typename MetaFunction>
using lambda_t = typename lambda<MetaFunction>::type;

// -----------------------------------------------------------------------------
// meta_list and utilities
// -----------------------------------------------------------------------------

template <typename... Ts>
struct meta_list;

template <typename List, typename E>
struct push_back;

template <template <typename...> typename List, typename... Ts, typename E>
struct push_back<List<Ts...>, E> {
    using type = List<Ts..., E>;
};

template <typename L1, typename L2>
struct append;

template <template <typename...> typename L1,
          typename... T1,
          template <typename...> typename L2,
          typename... T2>
struct append<L1<T1...>, L2<T2...>> {
    using type = L1<T1..., T2...>;
};

template <typename List, typename F>
struct transform;

template <template <typename...> typename List, typename... Ts, typename F>
struct transform<List<Ts...>, F> {
    template <typename T>
    using call = typename F::template apply<T>::type;

    using type = List<call<Ts>...>;
};

template <typename List>
struct c_size;

template <template <typename...> typename List, typename... Ts>
struct c_size<List<Ts...>> : integral_constant<std::size_t, sizeof...(Ts)> {};

template <typename List, typename E>
using push_back_t = typename push_back<List, E>::type;

template <typename L1, typename L2>
using append_t = typename append<L1, L2>::type;

template <typename List, typename F>
using transform_t = typename transform<List, F>::type;

template <typename List>
inline constexpr std::size_t c_size_v = c_size<List>::value;

// -----------------------------------------------------------------------------
// general utilities
// -----------------------------------------------------------------------------

template <typename A, typename B>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

template <typename T>
struct remove_pointer {
    using type = T;
};

template <typename T>
struct remove_pointer<T*> {
    using type = T;
};

template <typename T>
struct add_pointer {
    using type = T*;
};

template <typename A, typename B>
inline constexpr bool is_same_v = is_same<A, B>::value;

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

// -----------------------------------------------------------------------------
// testing
// -----------------------------------------------------------------------------

int main() {
    /* push_back_t tests */
    {
        using L1 = meta_list<int, double>;
        using R1 = push_back_t<L1, char>;

        static_assert(is_same_v<R1, meta_list<int, double, char>>);

        using L2 = std::tuple<int, char>;
        using R2 = push_back_t<L2, double>;

        static_assert(is_same_v<R2, std::tuple<int, char, double>>);
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

    std::cout << "ALL TESTS PASSING!" << '\n';
}
