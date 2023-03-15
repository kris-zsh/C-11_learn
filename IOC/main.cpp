#include <iostream>
#include <any>
#include "IOContainer.h"
using namespace std;

class ICar{
public:
    virtual ~ICar() = default;
    virtual void show() = 0;
};

class Bus:public ICar{
public:
    ~Bus() override =default;
    void show() override{
        cout << " Bus:: show\n";
    }
};

class Car:public ICar{
public:
    ~Car() override = default;
    void show() override{
        cout << " Car:: show\n";
    }
};


int main() {
    IOContainer<ICar>IOC;
    IOC.register_type<Bus>("Bus");
    IOC.register_type<Car>("Car");

    auto base1 = IOC.resolve_smart_ptr("Bus");
    auto base2 = IOC.resolve_smart_ptr("Car");

    base1->show();
    base2->show();
    return 0;
}
