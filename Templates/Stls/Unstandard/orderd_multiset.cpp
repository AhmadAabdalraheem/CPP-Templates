#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
// Define a ordered multiset
template<class T>
struct ordered_multiset {
    // internal tree storing (value, unique_id) to handle duplicates
    tree<pair<T,int>, null_type, less<pair<T,int>>,
         rb_tree_tag, tree_order_statistics_node_update> t;

    int uid = 0;

    // insert a value into the multiset
    void insert(T x) {
        t.insert({x, uid++});
    }

    // erase ONE occurrence of value x (if exists)
    void erase(T x) {
        auto it = t.lower_bound({x, -1});
        if(it != t.end() && it->first == x)
            t.erase(it);
    }

    // return k-th smallest element (0-indexed)
    T find_by_order(int k) {
        return t.find_by_order(k)->first;
    }

    // return number of elements strictly less than x
    int order_of_key(T x) {
        return t.order_of_key({x, -1});
    }

    // return current size of multiset
    int size() {
        return (int)t.size();
    }

    // check if empty
    bool empty() {
        return t.empty();
    }

    // clear all elements
    void clear() {
        t.clear();
        uid = 0;
    }
};
