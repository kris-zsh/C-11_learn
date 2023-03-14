//
// Created by zhaoshuhao on 2023/3/14.
//

#ifndef OBJECT_POOL_MY_OBJECT_POOL_H
#define OBJECT_POOL_MY_OBJECT_POOL_H
#include <functional>
#include <iostream>
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

const size_t MaxObjectNum = 10;
template<typename T>
class ObjectPool : public NoCopyable {
public:
    template<typename... Args>
    using construct = std::function<std::shared_ptr<T>(Args...)>;
    ~ObjectPool(){is_clean = true;}

    template<typename... Args>
    void init(size_t num, Args&&... args) {
        if (num <= 0 || num > MaxObjectNum)
            throw std::logic_error("object pool num is error");

        std::string construct_name = typeid(construct<Args...>).name();
        std::cout << "construct_name " << construct_name << std::endl;

        for (size_t i = 0; i < num; i++) {
            object_maps_.emplace(construct_name, std::shared_ptr<T>(new T(std::forward<Args>(args)...), [construct_name, this](T *p) {
                                     if(is_clean)
                                         delete p;
                                     else
                                        object_maps_.emplace(construct_name, std::shared_ptr<T>(p));
                                 }));
        }
    }

    template<typename... Args>
    std::shared_ptr<T> get() {
        std::string construct_name = typeid(construct<Args...>).name();
        auto range = object_maps_.equal_range(construct_name);
        for (auto it = range.first; it != range.second; it++) {
            auto ptr = it->second;
            object_maps_.erase(it);
            return ptr;
        }
        return nullptr;
    }

    size_t size() const{
        return object_maps_.size();
    }

private:
    bool is_clean = false;
    std::multimap<std::string, std::shared_ptr<T>> object_maps_;
};
#endif//OBJECT_POOL_MY_OBJECT_POOL_H
