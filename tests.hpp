#pragma once

#include <chrono>
#include <utility>
#include <cassert>
#include <set>
#include "Heap_Node.hpp"
#include "Heap_Array.hpp"
#include "Set_Array.hpp"
#include "Set_Node.hpp"
#include <random>

#define Max 1000000

pair<int, int> test_create(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-Max, Max);
    vector<int> values(n);
    set<int> target;
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
        target.insert(value);
    }

    auto startTime = chrono::steady_clock::now();
    Set_Node<int> set_node(values);
    auto endTime = chrono::steady_clock::now();
    auto node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    Set_Array<int> set_array(values);
    endTime = chrono::steady_clock::now();
    auto array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    assert(set_node == target);
    assert(set_array == target);

    return {node_time, array_time};
}


pair<int, int> test_insert(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-Max, Max);
    auto set_node = Set_Node<int>();
    auto set_array = Set_Array<int>();
    vector<int> values(n);
    set<int> target;
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
        target.insert(value);
    }

    auto startTime = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        set_node.insert(values[i]);
    auto endTime = chrono::steady_clock::now();
    int node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        set_array.insert(values[i]);
    endTime = chrono::steady_clock::now();
    int array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    assert(set_node == target);
    assert(set_array == target);

    return {node_time, array_time};
}

pair<int, int> test_search(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-Max, Max);
    vector<int> values(n);
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
    }
    Set_Node<int> set_node(values);
    Set_Array<int> set_array(values);
    auto startTime = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        assert(true == set_node.search(values[i]));
    auto endTime = chrono::steady_clock::now();
    int node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        assert(true == set_array.search(values[i]));
    endTime = chrono::steady_clock::now();
    int array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    return {node_time, array_time};
}

pair<int, int> test_remove(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-Max, Max);
    vector<int> values(n);
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
    }
    Set_Node<int> set_node(values);
    Set_Array<int> set_array(values);
    auto startTime = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        set_node.remove(values[i]);
    auto endTime = chrono::steady_clock::now();
    int node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
        set_array.remove(values[i]);
    endTime = chrono::steady_clock::now();
    int array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    assert(set_node.data.root == nullptr);
    assert(set_array.size() == 0);

    return {node_time, array_time};
}

pair<int, int> test_sum(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-n, n);
    vector<int> values(n);
    vector<int> val(n);
    set<int> set1;
    set<int> set2;
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
        set1.insert(value);
        val[i] = distribution(generator);
        set2.insert(val[i]);
    }
    Set_Node<int> set_node(values);
    Set_Array<int> set_array(values);

    Set_Node<int> sub_node(val);
    Set_Array<int> sub_array(val);

    auto startTime = chrono::steady_clock::now();
    set_node + sub_node;
    auto endTime = chrono::steady_clock::now();
    int node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    set_array + sub_array;
    endTime = chrono::steady_clock::now();
    int array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    set1.insert(set2.begin(), set2.end());
    assert(set_node + sub_node == set1);
    assert(set_array + sub_array == set1);

    return {node_time, array_time};
}

pair<int, int> test_subtraction(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-n, n);
    vector<int> values(n);
    vector<int> val(n);
    set<int> set1;
    set<int> set2;
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
        set1.insert(value);
        val[i] = distribution(generator);
        set2.insert(val[i]);
    }

    Set_Node<int> set_node(values);
    Set_Array<int> set_array(values);

    Set_Node<int> sub_node(val);
    Set_Array<int> sub_array(val);

    auto startTime = chrono::steady_clock::now();
    set_node - sub_node;
    auto endTime = chrono::steady_clock::now();
    int node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    set_array - sub_array;
    endTime = chrono::steady_clock::now();
    int array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    assert(set_node.getSize() == set_array.size());

    return {node_time, array_time};
}

pair<int, int> test_intersection(const int n) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(-n, n);
    vector<int> values(n);
    vector<int> val(n);
    set<int> set1;
    set<int> set2;
    for (int i = 0; i < n; i++) {
        int value = distribution(generator);
        values[i] = value;
        set1.insert(value);
        val[i] = distribution(generator);
        set2.insert(val[i]);
    }
    Set_Node<int> set_node(values);
    Set_Array<int> set_array(values);

    Set_Node<int> sub_node(val);
    Set_Array<int> sub_array(val);

    auto startTime = chrono::steady_clock::now();
    set_node * sub_node;
    auto endTime = chrono::steady_clock::now();
    int node_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    startTime = chrono::steady_clock::now();
    set_array * sub_array;
    endTime = chrono::steady_clock::now();
    int array_time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    assert(set_node.getSize() == set_array.size());

    return {node_time, array_time};
}