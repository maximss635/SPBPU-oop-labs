//
// Created by maxim on 12.11.2020.
//

#ifndef EVENTS_TREE_H
#define EVENTS_TREE_H

#include <iostream>
#include <list>
#include <functional>

template <typename Value, typename Key>
class Tree {
private:
    class TreeNode;
    TreeNode* root;

public:
    Tree() : root(nullptr) { }

    ~Tree() {
        // remove all nodes
        remove_all();
    }

    void remove_all() {
        // Not very optimal implement
        // it will be better to iterate over all node (for example with dfs()) and delete every

        while(!empty()) {
            remove(root);
        }
    }

    // with recursion insert node with value VALUE and key KEY to tree
    void insert(Value value, Key key) {
        if (root) {
            insert(root, value, key);
        }
        else {
            root = new TreeNode(key, value, nullptr);
        }
    }

    // look for node with key KEY and delete it from tree
    void remove(Key& key) {
        // firstly look for
        TreeNode* deleted_node = search_node(key);
        if (deleted_node == nullptr) {
            // no such node
            return;
        }

        remove(deleted_node);
    }

    // recursive traversal over all nodes and do FUNC(node) with every node
    void dfs(std::function<void(Value&)> func) {
        // start iterating from root
        dfs(root, func);
    }

    inline bool empty() const {return root == nullptr; }

private:
    /** recursive traversal over all nodes and look for place to
     * inserting new node with key KEY and value VALUE
     * */
    void insert(TreeNode* start, Value& value, Key& key) {
        if (start->key > key) {
            if (start->child_right) {
                insert(start->child_right, value, key);
            }
            else {
                start->child_right = new TreeNode(key, value, start);
            }
        }
        else {
            if (start->child_left) {
                insert(start->child_left, value, key);
            }
            else {
                start->child_left = new TreeNode(key, value, start);
            }
        }
    }

    // recursive traversal over all nodes and do FUNC(node) with every node
    void dfs(TreeNode* start, std::function<void(Value&)> func) {
        if (start == nullptr) {
            return;
        }

        dfs(start->child_right, func);
        func(start->value);
        dfs(start->child_left, func);
    }

    // binary search node with key KEY
    TreeNode* search_node(Key& key) {
        if (empty()) {
            return nullptr;
        }

        TreeNode* node = root;
        while (node && node->key != key) {
            if (node->key > key) {
                node = node->child_right;
            }
            else {
                node = node->child_left;
            }
        }

        return node;
    }

    // remove node DELETED_NODE from the tree
    void remove(TreeNode* deleted_node) {
        if (deleted_node == nullptr) {
            return;
        }

        int child_number_ = deleted_node->child_number();

        if (child_number_ == 0) {
            deleted_node->tear_away_from_parent();

            if (deleted_node == root) {
                delete deleted_node;
                root = nullptr;
            }
            else {
                delete deleted_node;
            }
        }

        else if (child_number_ == 1) {
            TreeNode* grandfather = deleted_node->parent;
            TreeNode* child = deleted_node->child_right;
            if (child == nullptr) child = deleted_node->child_left;

            deleted_node->tear_away_from_parent();

            if (grandfather) {
                if (deleted_node == grandfather->child_right) {
                    grandfather->child_right = child;
                } else {
                    grandfather->child_right = child;
                }
            }

            if (deleted_node == root) {
                root = child;
            }

            delete deleted_node;
        }

        else {
            // 2 childrens -  instead of DELETED_NODE should stay the leftest of right under-tree
            TreeNode* moved_node = deleted_node->child_right;
            while (moved_node->child_left) {
                moved_node = moved_node->child_left;
            }

            moved_node->tear_away_from_parent();
            deleted_node->value = moved_node->value;

            delete moved_node;
        }
    }

private:
    class TreeNode {
    public:
        TreeNode(const Key& key, const Value& value, TreeNode* parent) :
            key(key),
            value(value),
            parent(parent),
            child_right(nullptr),
            child_left(nullptr) { }

        // pointers to parent node and two children's node
        TreeNode* child_left;
        TreeNode* child_right;
        TreeNode* parent;

        // element stored in node with key
        Key   key;
        Value value;

        int child_number() const {
            if ((child_right == nullptr) && (child_left == nullptr)) {
                return 0;
            }
            if ((child_right != nullptr) && (child_left != nullptr)) {
                return 2;
            }

            return 1;
        }

        inline bool has_parent() const {
            return parent != nullptr;
        }

        void tear_away_from_parent() {
            if (has_parent()) {
                if (this == parent->child_left) {
                    parent->child_left = nullptr;
                } else {
                    parent->child_right = nullptr;
                }
            }
        }
    };

};



#endif //EVENTS_TREE_H
