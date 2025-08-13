// ==================== vector ====================
// Dynamic array (fast random access, can grow)
// Access O(1), Insert at end O(1) amortized, Insert in middle O(n)
vector<int> v = {1,2,3};
v.push_back(4);   // O(1) amortized
v.emplace_back(20);  // More efficient than push_back
v.pop_back();     // O(1)
v.front();        // O(1)
v.back();         // O(1)
v.size();         // O(1)
sort(v.begin(), v.end()); // O(n log n)
reverse(v.begin(), v.end()); // O(n)

vector<ll > vv ( n , 1 ) // initialize  with 1

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
