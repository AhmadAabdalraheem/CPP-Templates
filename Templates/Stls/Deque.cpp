// ==================== deque ====================
// Double-ended queue (fast insertion/removal at both ends)
// Access O(1), Insert/Remove at ends O(1)

deque<int> dq = {1,2};
dq.push_front(0); // O(1)
dq.push_back(3);  // O(1)
dq.pop_front();   // O(1)
dq.pop_back();    // O(1)
dq.front();       // O(1)
dq.back();        // O(1)
