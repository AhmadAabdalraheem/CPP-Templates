
// ==================== multiset ====================
// Ordered elements (duplicates allowed)
multiset<int> ms = {1,1,2};
ms.insert(3);          // O(log n) : insert
ms.erase(ms.find(1));  // O(log n) : erase one occurrence
ms.count(1);           // O(log n) -> size_t : count occurrences
ms.lower_bound(1);     // O(log n) -> iterator
ms.upper_bound(1);     // O(log n) -> iterator
