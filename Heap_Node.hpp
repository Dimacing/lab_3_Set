#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include "Heap_Array.hpp"

using namespace std;

template<class T>
struct Node {
    T value;
    Node<T> *parent = nullptr;
    Node<T> *left = nullptr;
    Node<T> *right = nullptr;

    Node(T value1, Node<T> *parent1, Node<T> *left1, Node<T> *right1) : value(value1), parent(parent1), left(left1), right(right1) {}

    /// Поиск элемента
    Node *search(T target) {
        if (target == value)
            return this;
        if (left && target <= left->value) {
            auto searched = left->search(target);
            if (searched) return searched;
        }
        if (right && target <= right->value) {
            auto searched = right->search(target);
            return searched;
        }
        return nullptr;
    }

    /// Вставка элемента
    void insert(T target) {
        if (!left) {
            left = new Node<T>(target, this, nullptr, nullptr);
            if (right && (right->value < left->value)) swap(left, right);
            return;
        }
        if (!right) {
            right = new Node<T>(target, this, nullptr, nullptr);
            if (left && (right->value < left->value)) swap(left, right);
            return;
        }
        if (target <= left->value) {
            left->insert(target);
            return;
        }
        if (target <= right->value) {
            right->insert(target);
            return;
        }
        Node *new_node = new Node<T>(target, this, right, nullptr);
        right->parent = new_node;
        right = new_node;
        if (right->value < left->value) swap(left, right);
    }

    /// Проверка равенства наследников
    bool equalChild(Node<T> &node) const {
        bool result = false;
        if (!left && !right && !node.left && !node.right)
            return true;
        if (left && node.left)
            result = left->equalChild(*(node.left));
        if (!result)
            return false;
        if (right && node.right)
            result = right->equalChild(*(node.right));
        return result;
    }

    /// Вывод
    void print(ostream &out) {
        out << value;
        if (left) {
            out << ", ";
            left->print(out);
        }
        if (right) {
            out << ", ";
            right->print(out);
        }
    };

    /// Обходы
    void NLR(ostream &out) {
        out << value;
        if (parent) cout << "{" << parent->value << "}";
        out << " ";
        if (left) left->NLR(out);
        if (right) right->NLR(out);
    }

    void NRL(ostream &out) {
        out << value;
        if (parent) cout << "{" << parent->value << "}";
        out << " ";
        if (right) right->NRL(out);
        if (left) left->NRL(out);
    }

    void LRN(ostream &out) {
        if (left) left->LRN(out);
        if (right) right->LRN(out);
        out << value;
        if (parent) out << "{" << parent->value << "}";
        out << " ";
    }

    void LNR(ostream &out) {
        if (left) left->LNR(out);
        out << value;
        if (parent) out << "{" << parent->value << "}";
        out << " ";
        if (right) right->LNR(out);
    }

    void RLN(ostream &out) {
        if (right) right->RLN(out);
        if (left) left->RLN(out);
        out << value;
        if (parent) out << "{" << parent->value << "}";
        out << " ";
    }

    void RNL(ostream &out) {
        if (right) right->RNL(out);
        out << value;
        if (parent) out << "{" << parent->value << "}";
        out << " ";
        if (left) left->RNL(out);
    }

    /// Получить значения
    void getValues(vector<T> &values) {
        values.push_back(value);
        if (right) right->getValues(values);
        if (left) left->getValues(values);
    }

    ~Node() {
        if (parent) {
            if (parent->left && parent->left == this) parent->left = nullptr;
            if (parent->right && parent->right == this) parent->right = nullptr;
            parent = nullptr;
        }
        parent = nullptr;
        if (left) {
            if (left->parent == this) left->parent = nullptr;
            left = nullptr;
        }
        if (right) {
            if (right->parent == this) right->parent = nullptr;
            right = nullptr;
        }
    }
};

template<class T>
struct Heap_Node {
    Node<T> *root = nullptr;

    Heap_Node() : root(nullptr) {}

    /// Heap из array
    explicit Heap_Node(const T *list, int size) {
        for (int i = 0; i < size; i++) {
            insert(list[i]);
        }
    }

    /// Heap из vector
    explicit Heap_Node(const vector<T> &vec) {
        for (T item: vec) {
            insert(item);
        }
    }

    /// Копирующий конструктор
    Heap_Node(const Heap_Node<T> &heap) {
        if (heap.root) {
            root = new Node<T>(heap.root->value, heap.root->parent, heap.root->left, heap.root->right);
            if (root->left)
                copyChild(*(heap.root->left), *root, 1);
            if (root->right)
                copyChild(*(heap.root->right), *root, 0);
        }
    }

    /// Копировать наследников
    void copyChild(Node<T> &item, Node<T> &p, int if_left) const {
        auto *node = new Node<T>(item.value, &p, nullptr, nullptr);
        if (item.left) node->left = item.left;
        if (item.right) node->right = item.right;
        if (if_left)
            p.left = node;
        else
            p.right = node;
        if (node->left)
            copyChild(*(item.left), *node, 1);
        if (node->right)
            copyChild(*(item.right), *node, 0);
    }

    /// Поиск поддерева
    [[nodiscard]] bool searchHeap(const Heap_Node<T> &heap) const {
        Node<T> *node = root->search(heap.root->value);
        if (!node)
            return false;
        return node->equalChild(*(heap.root));
    }

    /// Поиск элемента
    [[nodiscard]] bool search(const T &target) const {
        if (root && root->search(target)) return true;
        return false;
    }

    /// Вставка элемента
    void insert(const T &target) {
        if (!root) {
            root = new Node<T>(target, nullptr, nullptr, nullptr);
            return;
        }

        if (target <= root->value) {
            if (!search(target))
                root->insert(target);
        } else {
            auto *new_node = new Node<T>(target, nullptr, root, nullptr);
            root->parent = new_node;
            root = new_node;
        }
    }

    /// Удаление элемента
    void remove(const T &target) {
        if (!root) return;
        if (target > root->value) return;
        Node<T> *node = root->search(target);
        if (!node)
            return;
        while (node->left && node->right) {
            swap(node->value, node->right->value);
            node = node->right;
        }
        if (node->parent) {
            if (node->left) {
                if (node->parent->right && node->parent->right == node) {
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                }
                if (node->parent->left && node->parent->left == node) {
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                }
            }
            if (node->right) {
                if (node->parent->right && node->parent->right == node) {
                    node->parent->right = node->right;
                    node->right->parent = node->parent;
                }
                if (node->parent->left && node->parent->left == node) {
                    node->parent->left = node->right;
                    node->right->parent = node->parent;
                }
            }
        } else {
            if (!node->left && !node->right) {
                root = nullptr;
            }
            if (node->left) {
                root = node->left;
            }
            if (node->right) {
                root = node->right;
            }
        }
        delete node;
    }

    /// Обходы
    void NLR(ostream &out) const {
        if (root) root->NLR(out);
    }

    void NRL(ostream &out) const {
        if (root) root->NRL(out);
    }

    void LRN(ostream &out) const {
        if (root) root->LRN(out);
    }

    void LNR(ostream &out) const {
        if (root) root->LNR(out);
    }

    void RLN(ostream &out) const {
        if (root) root->RLN(out);
    }

    void RNL(ostream &out) const {
        if (root) root->RNL(out);
    }

    /// Получить значения
    [[nodiscard]] vector<T> getValues() const {
        vector<T> vector;
        if (root) root->getValues(vector);
        return vector;
    }

    /// Получение поддерева
    [[nodiscard]] Heap_Node<T> &subHeap(const T &target) const {
        Node<T> *node = root->search(target);
        auto *sub_heap = new Heap_Node<T>();
        if (node) {
            auto *new_root = new Node<T>(node->value, nullptr, node->left, node->right);
            sub_heap->root = new_root;
            if (new_root->left)
                copyChild(*(node->left), *new_root, 1);
            if (new_root->right)
                copyChild(*(node->right), *new_root, 0);
        }
        return *sub_heap;
    }

    friend bool operator==(const Heap_Node<T> &heap, set<T> st) {
        auto values = heap.getValues();
        set<T> heap_set(values.begin(), values.end());
        if (heap_set == st) {
            return true;
        }
        return false;
    }

    friend bool operator!=(const Heap_Node<T> &heap, set<T> st) {
        return !(heap == st);
    }

    ~Heap_Node() = default;
};

/// Heap из string
Heap_Node<int> &heapFromString(const string &str) {
    auto *heap = new Heap_Node<int>();
    int item;
    for (int i = 0; i < str.size(); i++) {
        int j = i;
        while (j < str.size() && str[j] != ' ')
            j++;
        if (str[i] == '-') {
            item = -convert(str.substr(i + 1, j - i - 1));
        } else {
            item = convert(str.substr(i, j - i));
        }
        heap->insert(item);
        i = j;
    }
    return *heap;
}