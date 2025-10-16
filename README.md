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
}```


2)HashTree

A hybrid data structure combining hash-based indexing and tree-like hierarchy for efficient key-value storage, search, and range queries.

## Overview

**HashTree** offers the speed of hash maps for fast lookups while supporting hierarchical structure for efficient range queries. It splits keys into segments via hashing to organize data in a multi-level tree, allowing:

- O(depth) average time complexity for insert, search, and delete operations.
- Efficient range queries to retrieve all key-value pairs within a specified key range.
- Dynamic, memory-friendly nodes using `std::unordered_map` and smart pointers.

This structure is especially useful when you need quick access and partial key range searches, which traditional hash maps alone cannot efficiently provide.

## Features

- **Fast Lookup:** Uses hash functions at each tree level for O(1) child access.
- **Range Queries:** Retrieve elements in key intervals efficiently.
- **Memory Safety:** Uses `std::unique_ptr` to manage node lifetimes automatically.
- **Configurable Depth:** Customize tree depth to balance between speed and memory.

## Usage

```cpp
#include "HashTree.hpp" // or include the class definition directly

int main() {
    HashTree<int, std::string> tree(4); // 4-level tree depth

    // Insert key-value pairs
    tree.insert(42, "Answer to everything");
    tree.insert(7, "Lucky number");
    tree.insert(100, "Century");

    // Search for keys
    auto res = tree.search(42);
    if (res) {
        std::cout << "Found: " << res.value() << "\n";
    }

    // Range query example
    auto results = tree.rangeQuery(5, 50);
    for (const auto& [key, val] : results) {
        std::cout << key << ": " << val << "\n";
    }

    // Remove a key
    tree.remove(42);
}
