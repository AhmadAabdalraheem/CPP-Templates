 Set (Ordered Unique Elements)

 set<int> mySet = {30, 10, 20};

    // Insertion
    mySet.insert(15);        // O(log n)
    mySet.insert(25);

    // Access
    auto it = mySet.find(20); // O(log n)
    if(it != mySet.end()) {
        cout << "Found: " << *it << endl;
    }

    // Range queries
    auto lb = mySet.lower_bound(15);
    auto ub = mySet.upper_bound(25);

    // Removal
    mySet.erase(10);         // O(log n)

    // Iteration (sorted order)
    for(int num : mySet) {
        cout << num << " ";
    }
