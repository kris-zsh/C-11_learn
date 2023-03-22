//
// Created by zhaoshuhao on 2023/3/22.
//

#ifndef MESSAGE_BUS_FUNCTION_TRAITS_H
#define MESSAGE_BUS_FUNCTION_TRAITS_H

//#include <cstdio>
#include <functional>
#include <tuple>

template<typename T>
struct function_traits;

template<typename Ret, typename ...Args>
struct function_traits<Ret(Args...)>{
    static constexpr std::size_t arity = sizeof...(Args);
    using function_type = Ret(Args...);
    using function_ret = Ret;
    using pointer = function_type*;
    using stl_function_type = std::function<function_type>;

    template<std::size_t I>
    struct args{
        static_assert(I < arity, "index is out of range");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

//函数指针
template<typename Ret, typename ...Args>
struct function_traits<Ret(*)(Args...)>:function_traits<Ret(Args...)>{};

//function
template<typename Ret, typename ...Args>
struct function_traits<std::function<Ret(Args...)>>:function_traits<Ret(Args...)>{};

//函数对象
template <typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())>{};

template<typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function& lambda){
    return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}
template<typename Function>
typename function_traits<Function>::stl_function_type to_function(Function&& lambda){
    return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

template<typename Function>
typename function_traits<Function>::pointer to_pointer(const Function& lambda){
    return static_cast<typename function_traits<Function>::pointer>(lambda);
}

//member function
#define FUNCTION_TRAITS(...) \
	template<typename ReturnType, typename ClassType, typename... Args>\
	struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : \
		function_traits<ReturnType(Args...)>{};

FUNCTION_TRAITS();
FUNCTION_TRAITS(const);
FUNCTION_TRAITS(volatile);
FUNCTION_TRAITS(const volatile);
#endif//MESSAGE_BUS_FUNCTION_TRAITS_H
