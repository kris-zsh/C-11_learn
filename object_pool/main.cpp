#include "my_object_pool.h"
#include <iostream>
using namespace std;

class Test {
public:
    Test() = default;
    Test(int a, int b) {}
    Test(int a, double b) {}
    void show(int num) {
        cout << num << endl;
    }
};
class BigObject {
public:
    BigObject() {}

    BigObject(int a) {}

    BigObject(const int &a, const int &b) {
    }
    ~BigObject(){ cout << "disconstruct BigObject" << endl;}

    void Print(const string &str) {
        cout << str << endl;
    }
};

void Print(const shared_ptr<BigObject>& p, const string &str) {
    if (p != nullptr) {
        p->Print(str);
        return;
    }
    cout << "this shared_ptr is null str: " << str <<endl;

}
int main() {
    ObjectPool<BigObject> pool;
    pool.init(2);

    {
        auto p = pool.get();
        Print(p, "p");

        auto p2 = pool.get();
        Print(p2, "p2");
        cout << "pool size: "<< pool.size() <<endl;
    }

    cout << "------------------" << endl;
    cout << "pool size: "<< pool.size() <<endl;
    auto p = pool.get();
    Print(p, "p");

    auto p2 = pool.get();
    Print(p2, "p2");

    auto p3 = pool.get();
    Print(p3, "p3");

    cout << "------------------" << endl;
    cout << "pool size: "<< pool.size() <<endl;

    pool.init(2, 1);

    cout << "pool size: "<< pool.size() <<endl;

    auto p4 = pool.get<int>();
    cout << "pool size: "<< pool.size() <<endl;
    Print(p4, "p4");
    return 0;
}
