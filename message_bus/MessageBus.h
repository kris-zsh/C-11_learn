//
// Created by zhaoshuhao on 2023/3/22.
//

#ifndef MESSAGE_BUS_MESSAGEBUS_H
#define MESSAGE_BUS_MESSAGEBUS_H

#include "function_traits.h"
#include <any>
#include <map>
#include <string>
class NoCopyable {
protected:
    NoCopyable() = default;
    virtual ~NoCopyable() = default;
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable &operator=(const NoCopyable &) = delete;
};

class MessageBus : private NoCopyable {
public:
    template<typename F>
    void attach(F &&f, const std::string &topic = "") {
        auto fun = to_function(std::forward<F>(f));
        add(topic, std::move(fun));
    }
    template<typename R>
    void send_req(const std::string &topic = "") {
        using function_type = std::function<R()>;
        std::string msg_type = topic + typeid(function_type).name();
        auto range = maps.equal_range(msg_type);
        for (auto it = range.first; it != range.second; it++) {
            auto f = std::any_cast<function_type>(it->second);
            f();
        }
    }

    template<typename R, typename... Args>
    void send_req(Args &&...args, const std::string &topic = "") {
        using function_type = std::function<R(Args...)>;
        std::string msg_type = topic + typeid(function_type).name();
        auto range = maps.equal_range(msg_type);
        for (auto it = range.first; it != range.second; it++) {
            auto f = std::any_cast<function_type>(it->second);
            f(std::forward<Args>(args)...);
        }
    }

    template<typename R,typename ...Args>
    void remove(const std::string& topic = ""){
        using function_type = std::function<R(Args...)>;
        std::string msg_type = topic + typeid(function_type).name();
        auto range = maps.equal_range(msg_type);
        maps.erase(range.first,range.second);
    }
private:
    template<typename F>
    void add(const std::string &topic, F &&f) {
        std::string msg_type = topic + typeid(F).name();
        maps.emplace(msg_type, std::forward<F>(f));
    }
    std::multimap<std::string, std::any> maps;
};
#endif//MESSAGE_BUS_MESSAGEBUS_H
