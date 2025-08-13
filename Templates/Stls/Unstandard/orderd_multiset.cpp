#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
// Define a ordered multiset
template<typename T>
using ordered_multiset = tree<
T, // Key type
null_type, // Mapped type (not used)
less_equal<T>, // Comparison (allows duplicates)
rb_tree_tag, // Red-Black tree
tree_order_statistics_node_update // Node update policy
>;
// Usage example:
ordered_multiset<int> ms;
ms.insert(1);
ms.insert(2);
ms.insert(2);
ms.insert(3);
// Number of elements less than 3 (returns 3)
cout << ms.order_of_key(3) << endl;
// Find k-th element (0-based, returns iterator to 2)
cout << *ms.find_by_order(2) << endl;
// Erase elements (be careful with duplicates)
ms.erase(ms.lower_bound(2)); // Erases one occurrence of 2
