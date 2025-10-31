#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
 
int a = 0;
std::mutex mtx;

void func_inc() {
    for (size_t i = 0; i < 1000000; ++i){
        mtx.lock();
        a++;
        mtx.unlock();
    }
}
 
void func_dec() {
    for (size_t i = 0; i < 1000000; ++i){
    mtx.lock();
        a--;
    mtx.unlock();
    }
}
 
int main()
{
    std::vector<std::thread> threads;
 
    // Launch multiple increment and decrement threads
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(func_inc);
        threads.emplace_back(func_dec);
    }
 
    for (auto& t : threads)
        t.join();
    
    if (a == 0) {
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "Failure! Expected a = 0, received a = " << a << std::endl;
    }
    
 
    return 0;
}