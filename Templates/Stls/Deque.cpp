#include <deque>

deque<int> dq;

// Insertion at both ends - O(1)
dq.push_back(10);
dq.push_front(5);

// Access - O(1)
int front = dq.front();
int back = dq.back();
int elem = dq[2];

// Removal - O(1) at ends, O(n) in middle
dq.pop_back();
dq.pop_front();
dq.erase(dq.begin() + 1);
