//
// Created by zhaoshuhao on 2023/3/13.
//

#ifndef OBJECT_POOL_OBJECT_POOL_H
#define OBJECT_POOL_OBJECT_POOL_H

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

const int MaxObjectNum = 10;

template<typename T>
class ObjectPool {
    template<typename... Args>
    using construct = std::function<std::shared_ptr<T>(Args...)>;

public:
    ObjectPool() = default;
    ~ObjectPool() {
        is_need_clean = true;
    }
    template<typename... Args>
    void init(size_t num, Args&&... args) {
        if (num <= 0 || num >= MaxObjectNum)
            throw std::logic_error("object num ou of range");
        std::string construct_name = typeid(construct<Args...>).name();
        std::cout << "construct_name  " << construct_name << std::endl;
        object_maps_.emplace(construct_name, std::shared_ptr<T>(new T(std::forward<Args>(args)...),
                                         [this, construct_name](T* p){
            return create_ptr<T>(construct_name, args...);
                                         }));
    }
    template<typename... Args>
    std::shared_ptr<T> create_ptr(const std::string &name, Args... args) {
        return std::make_shared<T>(name, args..., [name, this](T *t) {
            if (is_need_clean)
                delete t;
            else
                object_maps_.emplace(name, std::shared_ptr<T>(t));
        });
    }
    template<typename... Args>
    std::shared_ptr<T> get() {
        std::string construct_name = typeid(construct<Args...>).name();
        auto range = object_maps_.equal_range(construct_name);
        for (auto it = range.first; it != range.seond; it++) {
            auto ptr = it->second;
            object_maps_.erase(it);
            return ptr;
        }
        return nullptr;
    }

private:
    bool is_need_clean = false;
    std::multimap<std::string, std::shared_ptr<T>> object_maps_;
};
#endif//OBJECT_POOL_OBJECT_POOL_H
