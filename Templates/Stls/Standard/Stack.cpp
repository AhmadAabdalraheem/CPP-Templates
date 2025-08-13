#include <stack>
#include <iostream>

using namespace std;

int main() {
    stack<int> s; // Create a stack of integers
     LIFO (Last In First Out)
    // Add elements to stack - push()
    s.push(10); // Bottom ← [10] ← Top
    s.push(20); // [10, 20]
    s.push(30); // [10, 20, 30]

    // Print stack size
    cout << "Stack size: " << s.size() << endl; // Output: 3

    // Access top element - top()
    cout << "Top element: " << s.top() << endl; // Output: 30

    // Remove elements - pop()
    s.pop(); // Removes 30
    cout << "Top after pop: " << s.top() << endl; // Output: 20

    // Check if empty
      required to not runtime error
    if(!s.empty()) {
        cout << "Stack is not empty" << endl;
    }

    return 0;
}
