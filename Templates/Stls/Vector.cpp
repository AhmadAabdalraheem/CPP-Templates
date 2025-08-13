#include <vector>

vector<int> v;

// Insertion at end - O(1) amortized
v.push_back(10);  
v.emplace_back(20);  // More efficient than push_back

// Access elements - O(1)
int first = v[0];       // No bounds checking
int second = v.at(1);   // With bounds checking (throws exception)
int last = v.back();

// Insertion at middle - O(n)
v.insert(v.begin() + 1, 30);

// Deletion - O(n) for middle, O(1) for end
v.erase(v.begin() + 2);
v.pop_back();

// Size operations - O(1)
size_t s = v.size();
bool empty = v.empty();
