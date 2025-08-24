# CS 300 – Project One: Runtime & Memory Analysis

## Runtime Summary (Worst-Case unless noted)

| Structure     | Build (read + insert)        | Lookup single course     | Print all (alphanumeric)     | Memory                   |
|---------------|-------------------------------|---------------------------|-------------------------------|--------------------------|
| Vector        | O(n) append; sort O(n log n)  | O(n)                      | sort keys O(n log n) + O(n)   | O(n)                     |
| Hash Table    | O(n) avg; O(n²) worst*        | O(1) avg; O(n) worst      | keys O(n) + sort O(n log n)   | O(n) + bucket overhead   |
| BST (unbal.)  | O(n log n) avg; O(n²) worst   | O(log n) avg; O(n) worst  | in-order traversal O(n)       | O(n) nodes               |

\* worst-case hash with many collisions or poor resizing policy.

### Per-Line Cost (Build step)
Assume **n** courses, average **r** prerequisites per course (bounded).

- **Pass 1 (collect IDs):** O(n) reads; split/check/insert ≈ O(n) → total **O(n)**  
- **Pass 2:** create objects (≈ O(n)), validate prereqs (≈ O(n·r)), insert:
  - Vector: O(1) each → **O(n)** (later sort **O(n log n)** to print)
  - Hash: O(1) avg each → **O(n)** avg (worst **O(n²)**)
  - BST (unbalanced): O(log n) avg each → **O(n log n)** avg; worst **O(n²)**

### Advantages / Disadvantages
- **Vector:** simple; must sort to print; O(n) lookups.
- **Hash Table:** fastest average lookups; must sort keys to print; depends on good hashing/resizing.
- **BST (unbalanced):** natural in-order print; can degrade to a list in worst case; balancing solves that.

### Recommendation
Use a **hash table** for fast lookups; sort keys when printing. If guaranteed ordered traversal without sorting is required, use a **self-balancing BST** (AVL/Red-Black).
