//
// Created by zhaoshuhao on 2023/3/15.
//

#ifndef IOC_IOCONTAINER_H
#define IOC_IOCONTAINER_H

#include <map>
#include <functional>
#include <string>
#include <stdexcept>
#include <memory>
#include <cassert>

class NoCopyable {
public:
    NoCopyable() = default;
    virtual ~NoCopyable() = default;
    NoCopyable(const NoCopyable&) = delete;
    NoCopyable& operator=(const NoCopyable&) = delete;
};

template<typename T>
class IOContainer : public NoCopyable{
public:
    using create_object = std::function<T*()>;
    IOContainer() =default;
    ~IOContainer() override = default;

    template<typename Dervied>
    void register_type(const std::string& key){
        create_object ob = [](){return new Dervied();};
        register_type(key,ob);
    }

    T* resolve_ptr(const std::string& key){
        auto it = create_maps.find(key);
        return it->second();
    }
    std::shared_ptr<T> resolve_smart_ptr(const std::string& key){
        auto it = create_maps.find(key);
        assert(it != create_maps.end());
        T* t = it->second();
        return std::shared_ptr<T>(t);
    }

private:
    void register_type(const std::string& key, create_object ob){
        if(create_maps.find(key) != create_maps.end())
            throw std::invalid_argument("this key already exist");
        create_maps[key] = ob;
    }

    std::map<std::string,create_object >create_maps;
};

#endif//IOC_IOCONTAINER_H
