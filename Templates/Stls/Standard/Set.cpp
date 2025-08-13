// ==================== set ====================
// Ordered unique elements (balanced BST)
set<int> s = {3,1,2};
s.insert(4);           // O(log n) : insert element
s.erase(1);            // O(log n) : erase by value
s.count(3);            // O(log n) -> size_t : how many times val exists (0/1)
s.find(2);             // O(log n) -> iterator to element or end()
s.lower_bound(2);      // O(log n) -> iterator (first >= val)
s.upper_bound(2);      // O(log n) -> iterator (first > val)
