Multi-dimesion array ( array-vector hybrid)
Idea: See 
when we use an array, we have fast search, but fixed size
when we use vector, we have no space wastage, but lose the O(1) search
So my idea was to create something that had best of both
See, lets say until a fixed size (like 10 elements), we have array like behaviour
but after that, it behaves like a vector, that dynamically resizes based on input given in subequent blocks, 
and after the insertion is done, it turns back to array, basically best of both.

#include <iostream>
#include <stdexcept>
#include <algorithm>

template<typename T, size_t THRESHOLD = 10>
class AdaptiveContainer {
private:
    size_t sz = 0;        // current size
    size_t cap = THRESHOLD; // current capacity
    bool usingHeap = false;

    T stackArray[THRESHOLD];
    T* heapArray = nullptr;

    void promoteToHeap() {
        heapArray = new T[cap * 2];
        for (size_t i = 0; i < sz; ++i) {
            heapArray[i] = std::move(stackArray[i]);
        }
        cap *= 2;
        usingHeap = true;
        std::cout << "[AdaptiveContainer] Promoted to HEAP mode (capacity: " << cap << ")\n";
    }

    void grow() {
        size_t newCap = cap * 2;
        T* newHeap = new T[newCap];
        for (size_t i = 0; i < sz; ++i) {
            newHeap[i] = std::move(heapArray[i]);
        }
        delete[] heapArray;
        heapArray = newHeap;
        cap = newCap;
        std::cout << "[AdaptiveContainer] Grew heap (new capacity: " << cap << ")\n";
    }

    void demoteToStack() {
        if (!usingHeap) return;
        
        for (size_t i = 0; i < sz; ++i) {
            stackArray[i] = std::move(heapArray[i]);
        }
        delete[] heapArray;
        heapArray = nullptr;
        cap = THRESHOLD;
        usingHeap = false;
        std::cout << "[AdaptiveContainer] Demoted back to STACK mode\n";
    }

public:
    static_assert(THRESHOLD > 0, "THRESHOLD must be greater than zero");

    AdaptiveContainer() = default;

    // Copy constructor
    AdaptiveContainer(const AdaptiveContainer& other) 
        : sz(other.sz), cap(other.cap), usingHeap(other.usingHeap) {
        if (usingHeap) {
            heapArray = new T[cap];
            for (size_t i = 0; i < sz; ++i)
                heapArray[i] = other.heapArray[i];
        } else {
            for (size_t i = 0; i < sz; ++i)
                stackArray[i] = other.stackArray[i];
            heapArray = nullptr;
        }
    }

    // Copy assignment operator
    AdaptiveContainer& operator=(const AdaptiveContainer& other) {
        if (this != &other) {
            // Clean up current heap data if any
            if (usingHeap && heapArray) {
                delete[] heapArray;
                heapArray = nullptr;
            }

            sz = other.sz;
            cap = other.cap;
            usingHeap = other.usingHeap;

            if (usingHeap) {
                heapArray = new T[cap];
                for (size_t i = 0; i < sz; ++i)
                    heapArray[i] = other.heapArray[i];
            } else {
                for (size_t i = 0; i < sz; ++i)
                    stackArray[i] = other.stackArray[i];
                heapArray = nullptr;
            }
        }
        return *this;
    }

    // Move constructor
    AdaptiveContainer(AdaptiveContainer&& other) noexcept
        : sz(other.sz), cap(other.cap), usingHeap(other.usingHeap) {
        if (usingHeap) {
            heapArray = other.heapArray;
            other.heapArray = nullptr;
        } else {
            for (size_t i = 0; i < sz; ++i)
                stackArray[i] = std::move(other.stackArray[i]);
            heapArray = nullptr;
        }
        other.sz = 0;
        other.cap = THRESHOLD;
        other.usingHeap = false;
    }

    // Move assignment operator
    AdaptiveContainer& operator=(AdaptiveContainer&& other) noexcept {
        if (this != &other) {
            if (usingHeap && heapArray) {
                delete[] heapArray;
            }
            sz = other.sz;
            cap = other.cap;
            usingHeap = other.usingHeap;
            if (usingHeap) {
                heapArray = other.heapArray;
                other.heapArray = nullptr;
            } else {
                for (size_t i = 0; i < sz; ++i)
                    stackArray[i] = std::move(other.stackArray[i]);
                heapArray = nullptr;
            }
            other.sz = 0;
            other.cap = THRESHOLD;
            other.usingHeap = false;
        }
        return *this;
    }

    ~AdaptiveContainer() {
        if (usingHeap) {
            delete[] heapArray;
        }
    }

    void push_back(const T& val) {
        if (usingHeap) {
            if (sz == cap) {
                grow();
            }
            heapArray[sz++] = val;
        } else {
            if (sz == THRESHOLD) {
                promoteToHeap();
                heapArray[sz++] = val;
            } else {
                stackArray[sz++] = val;
            }
        }
    }

    // push_back with move semantics
    void push_back(T&& val) {
        if (usingHeap) {
            if (sz == cap) {
                grow();
            }
            heapArray[sz++] = std::move(val);
        } else {
            if (sz == THRESHOLD) {
                promoteToHeap();
                heapArray[sz++] = std::move(val);
            } else {
                stackArray[sz++] = std::move(val);
            }
        }
    }

    void pop_back() {
        if (sz == 0) {
            throw std::out_of_range("pop_back on empty container");
        }
        --sz;
        
        // KEY FEATURE: Auto-shrink back to stack when size fits
        if (usingHeap && sz <= THRESHOLD) {
            demoteToStack();
        }
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void clear() {
        sz = 0;
        if (usingHeap) {
            delete[] heapArray;
            heapArray = nullptr;
            cap = THRESHOLD;
            usingHeap = false;
        }
    }

    T& operator[](size_t idx) {
        if (idx >= sz) throw std::out_of_range("Index out of bounds");
        return usingHeap ? heapArray[idx] : stackArray[idx];
    }

    const T& operator[](size_t idx) const {
        if (idx >= sz) throw std::out_of_range("Index out of bounds");
        return usingHeap ? heapArray[idx] : stackArray[idx];
    }

    // Provide begin/end iterators for range-based loops
    T* begin() {
        return usingHeap ? heapArray : stackArray;
    }
    const T* begin() const {
        return usingHeap ? heapArray : stackArray;
    }
    T* end() {
        return (usingHeap ? heapArray : stackArray) + sz;
    }
    const T* end() const {
        return (usingHeap ? heapArray : stackArray) + sz;
    }

    // Reserve capacity to reduce reallocations
    void reserve(size_t newCap) {
        if (newCap <= cap) return;
        if (!usingHeap) {
            promoteToHeap();
        }
        while (cap < newCap) {
            grow();
        }
    }

    // Manual optimization trigger (simulates scope exit optimization)
    void optimize() {
        if (usingHeap && sz <= THRESHOLD) {
            demoteToStack();
        }
    }

    // Check current mode
    bool isStackMode() const {
        return !usingHeap;
    }

    size_t capacity() const {
        return cap;
    }

    // Convenience accessors
    T& front() {
        if (empty()) throw std::out_of_range("front() on empty container");
        return (*this)[0];
    }
    const T& front() const {
        if (empty()) throw std::out_of_range("front() on empty container");
        return (*this)[0];
    }
    T& back() {
        if (empty()) throw std::out_of_range("back() on empty container");
        return (*this)[sz - 1];
    }
    const T& back() const {
        if (empty()) throw std::out_of_range("back() on empty container");
        return (*this)[sz - 1];
    }

    // Debug status printing
    void printStatus() const {
        std::cout << "[AdaptiveContainer] Size: " << sz 
                  << ", Capacity: " << cap
                  << ", Mode: " << (isStackMode() ? "STACK" : "HEAP")
                  << std::endl;
    }
};

// Example usage and demonstration
int main() {
    std::cout << "=== AdaptiveContainer Demo ===" << std::endl;
    
    AdaptiveContainer<int, 5> container;
    
    std::cout << "\n1. Adding elements within stack capacity:" << std::endl;
    for (int i = 1; i <= 4; ++i) {
        container.push_back(i);
        container.printStatus();
    }
    
    std::cout << "\n2. Exceeding stack capacity (triggers heap promotion):" << std::endl;
    container.push_back(5);
    container.printStatus();
    container.push_back(6);
    container.push_back(7);
    container.push_back(8);
    container.printStatus();
    
    std::cout << "\n3. Current elements: ";
    for (const auto& elem : container) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\n4. Removing elements (auto-demotion when size <= threshold):" << std::endl;
    while (container.size() > 3) {
        container.pop_back();
        container.printStatus();
    }
    
    std::cout << "\n5. Testing grow/shrink cycle:" << std::endl;
    for (int i = 10; i <= 20; ++i) {
        container.push_back(i);
    }
    container.printStatus();
    
    std::cout << "\n6. Shrinking back down:" << std::endl;
    while (container.size() > 5) {
        container.pop_back();
    }
    container.printStatus();
    
    container.pop_back(); // This should trigger demotion
    container.printStatus();
    
    std::cout << "\n7. Testing manual optimize():" << std::endl;
    for (int i = 1; i <= 10; ++i) {
        container.push_back(i * 10);
    }
    container.printStatus();
    
    while (container.size() > 4) {
        container.pop_back();
    }
    std::cout << "Before optimize(): ";
    container.printStatus();
    
    container.optimize();
    std::cout << "After optimize(): ";
    container.printStatus();
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}
