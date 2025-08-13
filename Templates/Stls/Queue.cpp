
#include <queue>
#include <iostream>

using namespace std;

int main() {
    queue<int> q; // Create a queue of integers
     FIFO (First In First Out)
    // Add elements - push()
    q.push(10); // Front → [10] ← Rear
    q.push(20); // [10, 20]
    q.push(30); // [10, 20, 30]

    // Print queue size
    cout << "Queue size: " << q.size() << endl; // Output: 3

    // Access front and back
    cout << "Front: " << q.front() << endl; // Output: 10
    cout << "Back: " << q.back() << endl;   // Output: 30

    // Remove element - pop()
    q.pop(); // Removes 10
    cout << "Front after pop: " << q.front() << endl; // Output: 20

    // Check if empty
    if(!q.empty()) {
        cout << "Queue is not empty" << endl;
    }

    return 0;
}
