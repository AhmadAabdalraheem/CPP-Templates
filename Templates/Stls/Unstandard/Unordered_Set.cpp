#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

struct ordered_set {
    tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> t;

    // insert element
    void insert(int x) {
        t.insert(x);
    }

    // erase element if exists
    void erase(int x) {
        auto it = t.find(x);
        if(it != t.end()) t.erase(it);
    }

    // check if element exists
    bool exist(int x) {
        return t.find(x) != t.end();
    }

    // kth smallest element (0-indexed)
    int kth(int k) {
        return *t.find_by_order(k);
    }

    // count elements strictly less than x
    int count_less(int x) {
        return t.order_of_key(x);
    }

    // size of set
    int size() {
        return (int)t.size();
    }

    // clear all elements
    void clear() {
        t.clear();
    }
};
