# ⚙️ Custom Data Structures for Modern C++

### 🧩 Reimagining the STL — adaptive, hybrid, and high-performance containers in C++

![C++](https://img.shields.io/badge/language-C%2B%2B17-blue)
![License: MIT](https://img.shields.io/badge/license-MIT-green)
![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-orange)

---

## 🚀 Overview

**Custom Data Structures** is an experimental collection of C++ containers designed to explore how standard data structures could evolve beyond fixed models like `std::vector` or `std::unordered_map`.

Each container focuses on:
- ⚡ **Performance** — cache-friendly and low-overhead  
- 🔁 **Adaptivity** — dynamically adjusts its behavior based on data size or access pattern  
- 🧱 **Modern C++ Design** — templates, move semantics, smart pointers, and exception safety  

| Structure | Description | Key Strength |
|------------|--------------|---------------|
| **AdaptiveContainer** | Dynamically switches between stack and heap storage | Stack-fast + heap-flexible |
| **HashTree** | Hybrid of hash map and tree hierarchy | Fast lookups *and* efficient range queries |

---

## 🧠 1️⃣ AdaptiveContainer

### Overview
A high-efficiency container that intelligently adapts its storage model.

> 🧩 Inspired by `std::string`’s **Small Buffer Optimization**, but extended to *any* container type.

- For **small sizes**, elements live directly on the **stack**, enabling *ultra-fast access*.  
- When size exceeds a compile-time threshold, it **promotes** to **heap storage**.  
- If elements are removed and size drops, it **demotes** back to stack storage automatically.

### ✨ Features
- 🧠 **Hybrid Storage:** Stack for speed, heap for capacity  
- 🔁 **Automatic Promotion/Demotion:** Switches modes seamlessly  
- 🧩 **STL-like Interface:** `push_back()`, `pop_back()`, `operator[]`, `iterators`, `reserve()`  
- 🛡️ **Exception-safe Copy/Move Semantics**  
- 🧮 **Manual Optimization:** `optimize()` lets you force a demotion when small  
- 🧰 **Debug Mode:** `printStatus()` shows current storage mode and size  

### 📘 Example
```cpp
#include "AdaptiveContainer.h"
#include <iostream>

int main() {
    AdaptiveContainer<int, 8> container;  // stack capacity threshold = 8

    for (int i = 0; i < 10; ++i)
        container.push_back(i);

    container.printStatus();  // should show "Heap mode"

    while (container.size() > 5)
        container.pop_back();

    container.printStatus();  // should show "Stack mode"
}

⚙️ Use Cases
	•	Small, frequently resized buffers
	•	Embedded or real-time systems where heap usage must be minimized
	•	Performance-critical algorithms with varying input sizes

⸻
```
🌳 2️⃣ HashTree

Overview

A hybrid data structure that merges hash-based indexing with tree-like hierarchy, delivering both lightning-fast lookups and efficient range queries.

Think of it as an unordered_map that can also do lower_bound() efficiently.

Keys are divided into segments via hashing, forming a multi-level hierarchy:
	•	O(depth) average-time complexity for insert, search, and delete
	•	Efficient range queries across ordered key intervals
	•	Memory-safe with std::unique_ptr-managed nodes

✨ Features
	•	⚡ Fast Lookup: O(1) average child access per level
	•	🌿 Range Queries: Retrieve key intervals efficiently
	•	🧩 Memory Safety: Smart pointer–based node management
	•	🛠️ Configurable Depth: Tune for your key distribution and memory budget

📘 Example
```cpp
#include "HashTree.hpp"
#include <iostream>
int main() {
    HashTree<int, std::string> tree(4); // 4-level tree depth

    tree.insert(42, "Answer to everything");
    tree.insert(7, "Lucky number");
    tree.insert(100, "Century");

    if (auto res = tree.search(42))
        std::cout << "Found: " << res.value() << "\n";

    auto results = tree.rangeQuery(5, 50);
    for (const auto& [key, val] : results)
        std::cout << key << ": " << val << "\n";

    tree.remove(42);
}

⚙️ Use Cases
	•	Key-value stores needing both hash-speed and ordered traversal
	•	Database indexing or caching layers
	•	Real-time analytics systems

⸻
```
🧪 Future Directions
	•	🔹 SmartDeque: Adaptive deque with dynamic segmentation
	•	🔹 ParallelHashTree: Multi-threaded version using lock-free techniques
	•	🔹 SmallVector<T>: STL-compatible vector with embedded small buffer optimization

💬 Contributions and ideas are welcome!
If you’ve ever wondered “what if STL containers were smarter?”, this project is for you.

⸻

🧾 License

Released under the MIT License.

⸻

⭐ Get Involved
	•	Fork the repo and try the containers in your projects
	•	Suggest a new hybrid data structure idea
	•	Share benchmarks or real-world performance results

🧩 Together, let’s imagine the next generation of C++ containers.

---
