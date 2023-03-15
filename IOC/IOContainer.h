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

    template<typename T, typename Depend = void>
    void register_type(const std::string &key) {
        using create_object = std::function<T *()>;
        create_object ob = [] {
            return new T(new Depend());
        };
        register_type(key, ob);
    }

    template<typename T>
    void register_type(const std::string &key) {
        using create_object = std::function<T *()>;
        create_object ob = [] {
            return new T();
        };
        register_type(key, ob);
    }

    template<typename T>
    T *resolve_ptr(const std::string &key) {
        using create_object = std::function<T *()>;
        auto it = create_maps.find(key);
        auto fun = std::any_cast<create_object>(it->second);
        return fun();
    }

    template<typename T>
    std::shared_ptr<T> resolve_smart_ptr(const std::string &key) {
        T *t = resolve_ptr<T>(key);
        return std::shared_ptr<T>(t);
    }

private:
    void register_type(const std::string &key, std::any any) {
        if (create_maps.find(key) != create_maps.end())
            throw std::invalid_argument("this key already exist");
        create_maps[key] = any;
    }

    std::map<std::string, std::any> create_maps;
};

#endif//IOC_IOCONTAINER_H
