#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> a = 0;
 
void func_inc() {
    for (size_t i = 0; i < 1000000; ++i)
        a++;
}
 
void func_dec() {
    for (size_t i = 0; i < 1000000; ++i)
        a--;
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