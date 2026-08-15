// Finds the first index (0-indexed) where prefix sum becomes >= target_sum
int find_first_prefix_sum(int u, long long target_sum) {
    if (!u) return -1;
    push_down(u);

    long long left_sum = tree[tree[u].l].sum;

    if (left_sum >= target_sum) {
        return find_first_prefix_sum(tree[u].l, target_sum);
    }
    
    // Check current node
    if (left_sum + tree[u].val >= target_sum) {
        return get_sz(tree[u].l); // Index of node 'u' relative to current subtree
    }

    // Search in right subtree
    int res = find_first_prefix_sum(tree[u].r, target_sum - left_sum - tree[u].val);
    if (res == -1) return -1;
    return get_sz(tree[u].l) + 1 + res;
}

int find_first_prefix_sum(long long target_sum) {
    return find_first_prefix_sum(root, target_sum);
}
