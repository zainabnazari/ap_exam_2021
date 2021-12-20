## Advanced Programming Exam 2021-22

### Deadline

The deadline for this project is **January 7, 2022**, at midnight.

### How to submit

Fork the repository, populate the file **list_pool.hpp** then push on the master branch of the fork. I will not accept any other form (e.g., you send me a tar.gz). I will check just the master branch. You can write additional tests, but your code is required to be compliant to the given ones. You can run the test suite by typing `make check`.

### A pool of blazingly fast linked lists

In this project, you must write a pool for fast linked lists. The pool stores each node in a `std::vector<node_t>`. The "address" of a node is `1+idx`, where `idx` is the index where the node is stored in the vector. This trick allows us to use address `0` as `end`, so we can use unsigned integers type. The first node stored in the vector will be put at `idx == 0`, but it will be referenced as `1`.

```c++
  list_pool<int, std::size_t> pool{};
  auto l = pool.new_list();
  // l == pool.end() == std::size_t(0)
  l = pool.push_front(42,l);
  // l == std::size_t(1)
```

The pool maintains a list of free nodes, i.e., available positions in the vector. The `free_node_list` is empty at the beginning.

```c++
  list_pool<int, std::size_t> pool{};
  auto l = pool.new_list();
  l = pool.push_front(10,l); // l == std::size_t(1)
  l = pool.push_front(11,l); // l == std::size_t(2) <-- later, this node will be deleted
  
  auto l2 = pool.new_list();
  l2 = pool.push_front(20,l2); // l2 == std::size_t(3)
  
  l = pool.free(l); // that node is deleted, so it is added to free_node_list
  
  l2 = pool.push_front(21,l2); // l2 == std::size_t(2)
```

Here is the class template (without the `noexcept`s, remember to use them). The following functions **must be implemented**, and you don't have to change the names. Of course, you are free to implement more private and public methods. Remember to write a good class for iterators to avoid code duplication for `iterator` and `const_iterator`.

```c++
template <typename T, typename N = std::size_t>
class list_pool{
  struct node_t{
    T value;
    N next;
  };
  std::vector<node_t> pool;
  using list_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
  list_type free_node_list; // at the beginning, it is empty
  
  node_t& node(list_type x) noexcept { return pool[x-1]; }
  const node_t& node(list_type x) const noexcept { return pool[x-1]; }

  public:
  list_pool();
  explicit list_pool(size_type n); // reserve n nodes in the pool
    
  using iterator = ...;
  using const_iterator = ...;

  iterator begin(list_type x);
  iterator end(list_type ); // this is not a typo
    
  const_iterator begin(list_type x) const;
  const_iterator end(list_type ) const; 
  
    const_iterator cbegin(list_type x) const;
  const_iterator cend(list_type ) const;
    
  list_type new_list(); // return an empty list

  void reserve(size_type n); // reserve n nodes in the pool
  size_type capacity() const; // the capacity of the pool

  bool is_empty(list_type x) const;

  list_type end() const noexcept { return list_type(0); }

  T& value(list_type x);
  const T& value(list_type x);

  list_type& next(list_type x);
  const list_type& next(list_type x) const;

  list_type push_front(const T& val, list_type head);
  list_type push_front(T&& val, list_type head);

  list_type push_back(const T& val, list_type head);
  list_type push_back(T&& val, list_type head);
  
  list_type free(list_type x); // delete first node

  list_type free_list(list_type x); // free entire list
};
```

Finally, there is an example of the usage of the iterators.

```c++
#include <cassert>
#include <algorithm> max_element, min_element

list_pool<int> pool{22};
auto l1 = pool.new_list();
// credits: pi as random number generator :)
l1 = pool.push_front(3, l1);
l1 = pool.push_front(1, l1);
l1 = pool.push_front(4, l1);
l1 = pool.push_front(1, l1);
l1 = pool.push_front(5, l1);
l1 = pool.push_front(9, l1);
l1 = pool.push_front(2, l1);
l1 = pool.push_front(6, l1);
l1 = pool.push_front(5, l1);
l1 = pool.push_front(3, l1);
l1 = pool.push_front(5, l1);
    
auto l2 = pool.new_list();
l2 = pool.push_front(8, l2);
l2 = pool.push_front(9, l2);
l2 = pool.push_front(7, l2);
l2 = pool.push_front(9, l2);
l2 = pool.push_front(3, l2);
l2 = pool.push_front(1, l2);
l2 = pool.push_front(1, l2);
l2 = pool.push_front(5, l2);
l2 = pool.push_front(9, l2);
l2 = pool.push_front(9, l2);
l2 = pool.push_front(7, l2);

auto M = std::max_element(pool.begin(l1), pool.end(l1));
assert(*M == 9);

auto m = std::min_element(pool.begin(l2), pool.end(l2));
assert(*m == 1);

```

