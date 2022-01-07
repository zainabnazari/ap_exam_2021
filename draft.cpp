#pragma once
#include <utility>


template <typename pool_type, typename address_type, typename T>
class list_iterator{

  pool_type* pool;
  address_type ind;
public:
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::output_iterator_tag;
  explicit list_iterator(pool_type* pool, address_type ind) : pool(pool), ind(ind){}

  list_iterator& operator++(){
    ind = pool->at(ind - 1).next;
    return *this;
  }

  list_iterator operator++(int){
    list_iterator tmp(ind);
    ind = pool->at(ind - 1).next;
    return tmp;
  }

  bool operator==(const list_iterator& other){
    return pool->at(ind - 1).value == other.pool->at(other.ind - 1).value;
  }

  bool operator<(const list_iterator& other){
    return pool->at(ind - 1).value < other.pool->at(other.ind - 1).value;
  }

  bool operator>(const list_iterator& other){
    return pool->at(ind - 1).value > other.pool->at(other.ind - 1).value;
  }

  reference operator*()const{
    return pool->at(ind - 1).value;
  }

};


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
  using pool_type = std::vector<node_t>;
  list_type free_node_list; // at the beginning, it is empty

  node_t& node(list_type x) noexcept { return pool[x-1]; }
  const node_t& node(list_type x) const noexcept { return pool[x-1]; }

public:

  list_pool()noexcept{}
  explicit list_pool(size_type n)noexcept{
    pool.reserve(n);
  } // reserve n nodes in the pool

  using iterator = list_iterator<pool_type, N, T>;
  using const_iterator = list_iterator<pool_type, N, const T>;

  iterator begin(list_type x){
    return iterator(&pool, x);
  }
  iterator end(list_type ){
    return iterator(&pool, list_type(0));
  }// this is not a typo

  const_iterator begin(list_type x) const{
    return const_iterator(&pool, x);
  }
  const_iterator end(list_type ) const{
    return const_iterator(&pool, list_type(0));
  }

  const_iterator cbegin(list_type x) const{
    return const_iterator(&pool, x);
  }
  const_iterator cend(list_type ) const{
    return const_iterator(&pool, list_type(0));
  }

  list_type new_list()noexcept{
    return list_type{};
  } // return an empty list

  void reserve(size_type n){
    pool.reserve(n);
  }; // reserve n nodes in the pool
  size_type capacity() const{
    return pool.capacity();
  } // the capacity of the pool

  bool is_empty(list_type x) const{
    return x == end();
  }

  list_type end() const noexcept { return list_type(0); }

  T& value(list_type x){
    return node(x).value;
  }
  const T& value(list_type x)const{
    return node(x).value;
  }

  list_type& next(list_type x){
    return node(x).next;
  }
  const list_type& next(list_type x)const{
    return node(x).next;
  }

  list_type push_front(const T& val, list_type head){
    return push_front(std::move(val), head);
  }
  list_type push_front(T&& val, list_type head){
    node_t newNode{val, head};

    list_type newNodeAddr = end();

    if(free_node_list == end()){
      pool.push_back(newNode);
      newNodeAddr = pool.size();
    }else{
      const list_type fnl = free_node_list;
      free_node_list = next(free_node_list - 1);
      pool[fnl - 1] = newNode;
      newNodeAddr = fnl;
    }
    return newNodeAddr;
  }


  list_type push_back(const T& val, list_type head){
    return push_back(std::move(val), head);
  }
  list_type push_back(T&& val, list_type head){
    node_t newNode{val, end()};

    list_type newNodeAddr = end();

    if(free_node_list == end()){
      pool.push_back(newNode);
      newNodeAddr = pool.size();
    }else{
      const list_type fnl = free_node_list;
      free_node_list = next(free_node_list - 1);
      pool[fnl - 1] = newNode;
      newNodeAddr = fnl;
    }

    if(head != end()){
      list_type tail = head;
      while(next(tail - 1) != end()){
        tail = next(tail - 1);
      }
      next(tail - 1) = newNodeAddr;
    }
    return head == end() ? newNodeAddr : head;
  }

  list_type free(list_type x){
    if(x == end())
      return end();

    const list_type head = next(x - 1);

    const list_type fnl = free_node_list;
    free_node_list = x;
    next(x - 1) = fnl;
    return head;
  } // delete first node
  list_type free_list(list_type x){
    list_type head = free(x);
    while(head != end()){
      head = free(head);
    }
    return head;
  } // free entire list
};
