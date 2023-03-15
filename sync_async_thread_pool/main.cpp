#include "threadpool.h"

#include <chrono>
#include <iostream>

using namespace std;

int main() {

    ThreadPool pool(2,5);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread t1([&pool] {
        for (size_t i = 0; i < 10; i++) {
            auto id = std::this_thread::get_id();
            pool.add_task([id] {
                cout << "sync thread1 id : " << id << endl;
            });
        }
    });
    std::thread t2([&pool] {
        for (size_t i = 0; i < 10; i++) {
            auto id = std::this_thread::get_id();
            pool.add_task([id] {
                cout << "sync thread2 id : " << id << endl;
            });
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
//    system("pause");
    pool.stop();

    t1.join();
    t2.join();

    return 0;
}
