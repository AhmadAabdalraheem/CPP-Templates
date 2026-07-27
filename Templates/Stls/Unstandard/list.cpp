/*
============================= std::list =============================

Doubly Linked List (NOT Random Access)

------------------------------------------------------------
Creation
------------------------------------------------------------

list<int> lst;

------------------------------------------------------------
Insertion
------------------------------------------------------------

lst.push_back(x);          // O(1) Insert at the end.
lst.push_front(x);         // O(1) Insert at the beginning.

lst.emplace_back(x);       // O(1) Construct at the end.
lst.emplace_front(x);      // O(1) Construct at the beginning.

------------------------------------------------------------
Deletion
------------------------------------------------------------

lst.pop_back();            // O(1) Remove last element.
lst.pop_front();           // O(1) Remove first element.

lst.erase(it);             // O(1) Remove element at iterator.

lst.remove(x);             // O(n) Remove all occurrences of x.

lst.remove_if(pred);       // O(n) Remove elements satisfying predicate.

lst.clear();               // O(n) Remove all elements.

------------------------------------------------------------
Access
------------------------------------------------------------

lst.front();               // O(1) First element.
lst.back();                // O(1) Last element.

------------------------------------------------------------
Iterators
------------------------------------------------------------

lst.begin();               // Iterator to first element.
lst.end();                 // Iterator after last element.

prev(lst.end());           // Iterator to last element.

next(it);                  // Next iterator.
prev(it);                  // Previous iterator.

advance(it, k);            // Move iterator k steps. O(k)

------------------------------------------------------------
Insert by Iterator
------------------------------------------------------------

lst.insert(it, x);         // O(1) Insert before iterator.

------------------------------------------------------------
Utilities
------------------------------------------------------------

lst.size();                // O(1) Number of elements.
lst.empty();               // O(1) Check if empty.

lst.reverse();             // O(n) Reverse list.

lst.sort();                // O(n log n) Sort ascending.
lst.sort(greater<int>());  // O(n log n) Sort descending.

lst.unique();              // O(n) Remove consecutive duplicates.

lst.merge(other);          // O(n) Merge two sorted lists.

lst.swap(other);           // O(1) Swap two lists.

lst.resize(n);             // Resize list.

lst.splice(pos, other);    // O(1) Move all nodes from other to this.
lst.splice(pos, other, it);// O(1) Move one node.
lst.splice(pos, other, first, last); // O(1) Move range.

------------------------------------------------------------
Traversal
------------------------------------------------------------

for (auto x : lst) { }

for (auto it = lst.begin(); it != lst.end(); ++it) {
    cout << *it;
}

------------------------------------------------------------
Important Notes
------------------------------------------------------------

❌ No operator[].
❌ No random access.

Accessing the k-th element:
auto it = lst.begin();
advance(it, k);            // O(k)

Insert / Erase using iterator:
O(1)

Searching for a value:
O(n)

Excellent for:
- LRU Cache
- Frequent insert/erase in the middle
- Stable iterators

Avoid when:
- You need arr[i] or random access.

====================================================================
*/
