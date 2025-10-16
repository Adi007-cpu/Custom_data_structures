#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <string>

template<typename Key, typename Value>
class HashTree {
private:
    struct Node {
        // Map from hashed key parts to child nodes — allows O(1) lookup at each level
        std::unordered_map<Key, std::unique_ptr<Node>> children;
        std::optional<Value> value;  // Only leaf nodes store values
        bool isLeaf = false;
    };

    std::unique_ptr<Node> root;
    size_t depth; // Number of levels (tree depth)

    // Extracts 4 bits from hash at given level for branching (0-15)
    size_t hashAtLevel(const Key& key, size_t level) const {
        std::hash<Key> hasher;
        return (hasher(key) >> (level * 4)) & 0xF;
    }

    // Converts hashed part to child key for lookup
    Key getChildKey(const Key& key, size_t level) const {
        return static_cast<Key>(hashAtLevel(key, level));
    }

    // Recursive helper for range query
    void rangeQueryHelper(const Node* node, size_t level, const Key& minKey, const Key& maxKey,
                          std::vector<std::pair<Key, Value>>& results) const {
        if (!node) return;

        if (level == depth - 1) {
            // Leaf level: check keys directly in this node's children
            for (const auto& [key, child] : node->children) {
                if (child->isLeaf && key >= minKey && key <= maxKey) {
                    results.emplace_back(key, child->value.value());
                }
            }
        } else {
            // Internal node: recurse into all children
            for (const auto& [_, child] : node->children) {
                rangeQueryHelper(child.get(), level + 1, minKey, maxKey, results);
            }
        }
    }

public:
    explicit HashTree(size_t maxDepth = 4) : root(std::make_unique<Node>()), depth(maxDepth) {}

    // Insert key-value pair (O(depth) with O(1) hash lookup at each level)
    void insert(const Key& key, const Value& val) {
        Node* current = root.get();
        for (size_t level = 0; level < depth - 1; ++level) {
            Key childKey = getChildKey(key, level);
            if (!current->children.count(childKey)) {
                current->children[childKey] = std::make_unique<Node>();
            }
            current = current->children[childKey].get();
        }
        // Final level stores actual key
        if (!current->children.count(key)) {
            current->children[key] = std::make_unique<Node>();
        }
        current->children[key]->value = val;
        current->children[key]->isLeaf = true;
    }

    // Search for key, returns std::optional<Value>
    std::optional<Value> search(const Key& key) const {
        const Node* current = root.get();
        for (size_t level = 0; level < depth - 1; ++level) {
            Key childKey = getChildKey(key, level);
            auto it = current->children.find(childKey);
            if (it == current->children.end()) return std::nullopt;
            current = it->second.get();
        }
        auto it = current->children.find(key);
        if (it != current->children.end() && it->second->isLeaf) {
            return it->second->value;
        }
        return std::nullopt;
    }

    // Remove key, returns true if removed
    bool remove(const Key& key) {
        std::vector<Node*> path;
        std::vector<Key> keys;
        Node* current = root.get();

        for (size_t level = 0; level < depth - 1; ++level) {
            Key childKey = getChildKey(key, level);
            keys.push_back(childKey);
            auto it = current->children.find(childKey);
            if (it == current->children.end()) return false;
            path.push_back(current);
            current = it->second.get();
        }

        auto it = current->children.find(key);
        if (it == current->children.end() || !it->second->isLeaf) return false;

        current->children.erase(it);

        // Clean up empty nodes bottom-up
        for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
            if (current->children.empty()) {
                path[i]->children.erase(keys[i]);
                current = path[i];
            } else {
                break;
            }
        }
        return true;
    }

    // Range query [minKey, maxKey]
    std::vector<std::pair<Key, Value>> rangeQuery(const Key& minKey, const Key& maxKey) const {
        std::vector<std::pair<Key, Value>> results;
        rangeQueryHelper(root.get(), 0, minKey, maxKey, results);
        return results;
    }
};

// Demo usage
int main() {
    HashTree<int, std::string> tree(4);

    std::cout << "=== HashTree Demo ===\n\n";

    // Insert
    tree.insert(42, "Answer to everything");
    tree.insert(7, "Lucky number");
    tree.insert(100, "Century");
    tree.insert(15, "Fifteen");
    tree.insert(8, "Eight");
    tree.insert(99, "Almost 100");

    // Search
    std::cout << "Search results:\n";
    for (int key : {42, 7, 999}) {
        auto res = tree.search(key);
        std::cout << "Key " << key << ": " << (res ? res.value() : "Not found") << "\n";
    }

    // Range query
    std::cout << "\nRange query [5, 50]:\n";
    auto results = tree.rangeQuery(5, 50);
    for (const auto& [key, val] : results) {
        std::cout << key << " -> " << val << "\n";
    }

    // Remove
    std::cout << "\nRemoving key 42...\n";
    tree.remove(42);
    auto res = tree.search(42);
    std::cout << "After removal, key 42: " << (res ? res.value() : "Not found") << "\n";

    return 0;
}
