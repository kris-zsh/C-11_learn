#include <iostream>
#include <any>
#include "IOContainer.h"
using namespace std;


struct test{
    void show(){
        cout << "test show" << endl;
    }
};

class ICar{
public:
    virtual ~ICar() = default;
    virtual void show(){};
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

class BBB:public ICar{
public:
    BBB(int a, int b): a_(a), b_(b){}
    ~BBB() override = default;
    void show() override{
        cout << " a_ " << a_ << " b_ " << b_ << '\n';
    }
private:
    int a_;
    int b_;
};

class A{
public:
    A(ICar* b): ptr_(b){}
    void show(){
        ptr_->show();
    }
private:
    ICar* ptr_;
};

int main() {
    IOContainer IOC;
    IOC.register_type<A,Bus>("Bus");
    IOC.register_type<A,Car>("Car");
    IOC.register_type<ICar,Car>("ICar");

    IOC.register_simple<test>("test");
    IOC.register_simple<BBB,int,int>("BBB");

    auto base1 = IOC.resolve_smart_ptr<A>("Bus");
    auto base2 = IOC.resolve_smart_ptr<A>("Car");
    auto base3 = IOC.resolve_smart_ptr<test>("test");
    auto base4 = IOC.resolve_smart_ptr<BBB>("BBB",10,20);

    base1->show();
    base2->show();
    base3->show();
    base4->show();
    return 0;
}
