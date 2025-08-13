
    // 1. Create an empty deque of integers
    deque<int> dq;

    // 2. Add elements to the back - O(1) amortized
    dq.push_back(10);
    dq.push_back(20);
    dq.push_back(30);

    // 3. Add elements to the front - O(1) amortized
    dq.push_front(5);
    dq.push_front(1);

    // 4. Print all elements - O(n)
    cout << "Elements in deque: ";
    for(int num : dq) {
        cout << num << " ";
    }
    cout << endl;

    // 5. Access elements
    cout << "First element: " << dq.front() << endl;  // O(1)
    cout << "Last element: " << dq.back() << endl;    // O(1)
    cout << "Element at index 2: " << dq[2] << endl;   // O(1)

    // 6. Remove elements from front - O(1)
    dq.pop_front();

    // 7. Remove elements from back - O(1)
    dq.pop_back();

    // 8. Check size - O(1)
    cout << "Current size: " << dq.size() << endl;

    // 9. Check if empty - O(1)
    if(!dq.empty()) {
        cout << "Deque is not empty" << endl;
    }

    // 10. Insert at middle position - O(n)
    dq.insert(dq.begin() + 1, 99);

    // 11. Erase from middle position - O(n)
    dq.erase(dq.begin() + 2);
