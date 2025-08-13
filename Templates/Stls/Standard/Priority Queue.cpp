
// compare function
struct mycmp {
    // top is smallest number
    bool operator()(int a, int b)
    {
        return a > b;
    }
};

priority_queue<ll, vi, mycmp> L;

int main() {
    // Max-heap (default)
     priority_queue<int> pq; 
    // like queue but is sorted 
    // sorted according to min , max , or compare function 
  
    // Add elements - push()
    pq.push(30); // [30]
    pq.push(10); // [30, 10]
    pq.push(20); // [30, 10, 20] (internally ordered)

    // Access top element
    cout << "Top element: " << pq.top() << endl; // Output: 30

    // Remove top element
    pq.pop(); // Removes 30
    cout << "Top after pop: " << pq.top() << endl; // Output: 20

    // Min-heap example
    priority_queue<int, vector<int>, greater<int>> min_pq;
    min_pq.push(30);
    min_pq.push(10);
    min_pq.push(20);
    cout << "Min-heap top: " << min_pq.top() << endl; // Output: 10

    return 0;
}
