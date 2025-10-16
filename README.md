# Custom_data_structures
A collection of custom data structures in C++.

1)Adaptive container
# AdaptiveContainer

An efficient, adaptive container that dynamically switches between stack-allocated storage and heap-allocated storage based on its size. 

- For small sizes (up to a compile-time `THRESHOLD`), data is stored on the stack for ultra-fast access and minimal overhead.
- Once the size exceeds the threshold, it promotes to heap storage to handle larger datasets efficiently.
- When elements are removed and the size falls back below the threshold, it automatically demotes back to stack storage, saving memory.

---

## Features

- **Hybrid Storage:** Combines the speed of stack allocation with the flexibility of dynamic heap allocation.
- **Automatic Promotion/Demotion:** Seamlessly switches between stack and heap as container size changes.
- **Exception-safe Copy/Move semantics:** Implements copy/move constructors and assignment operators.
- **Standard Container-like API:** Supports `push_back()`, `pop_back()`, `operator[]`, iterators, `reserve()`, and more.
- **Manual Optimization:** Call `optimize()` to force demotion to stack mode if applicable.
- **Debug-friendly:** Built-in status printing with `printStatus()`.

---

## Usage

```cpp
#include "AdaptiveContainer.h"
#include <iostream>

int main() {
    AdaptiveContainer<int, 8> container;

    // Add elements
    for (int i = 0; i < 10; ++i)
        container.push_back(i);

    container.printStatus();

    // Remove some elements to trigger demotion
    while (container.size() > 5)
        container.pop_back();

    container.printStatus();

    return 0;
}
