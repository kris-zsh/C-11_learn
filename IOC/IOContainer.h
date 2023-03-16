//
// Created by zhaoshuhao on 2023/3/15.
//

#ifndef IOC_IOCONTAINER_H
#define IOC_IOCONTAINER_H

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

class NoCopyable {
public:
    NoCopyable() = default;
    virtual ~NoCopyable() = default;
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable &operator=(const NoCopyable &) = delete;
};

class IOContainer : public NoCopyable {
public:
    IOContainer() = default;
    ~IOContainer() override = default;

    template<typename T, typename Depend, typename... Args>
    typename std::enable_if<!std::is_base_of_v<T, Depend>>::type register_type(const std::string &key) {
        using create_object = std::function<T *(Args&&...)>;
        create_object ob = [](Args&&... args) {
            return new T(new Depend(std::forward<Args>(args)...));
        };
        register_type(key, ob);
    }

    template<typename T, typename Depend, typename... Args>
    typename std::enable_if<std::is_base_of_v<T, Depend>>::type register_type(const std::string &key) {
        using create_object = std::function<T *(Args&&...)>;

        create_object ob = [](Args&&... args) {
            return new Depend(std::forward<Args>(args)...);
        };
        register_type(key, ob);
    }

    template<typename T, typename... Args>
    void register_simple(const std::string &key) {
        using create_object = std::function<T *(Args&&...)>;
        create_object ob = [](Args&&... args) {
            return new T(std::forward<Args>(args)...);
        };
        register_type(key, ob);
    }

    template<typename T, typename... Args>
    T *resolve_ptr(const std::string &key, Args&&... args) {
        using create_object = std::function<T *(Args&&...)>;
        auto it = create_maps.find(key);
        auto fun = std::any_cast<create_object>(it->second);
        return fun(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> resolve_smart_ptr(const std::string &key, Args&&... args) {
        T *t = resolve_ptr<T>(key, std::forward<Args>(args)...);
        return std::shared_ptr<T>(t);
    }

private:
    void register_type(const std::string &key, const std::any& any) {
        if (create_maps.find(key) != create_maps.end())
            throw std::invalid_argument("this key already exist");
        create_maps[key] = any;
    }

    std::map<std::string, std::any> create_maps;
};

#endif//IOC_IOCONTAINER_H
