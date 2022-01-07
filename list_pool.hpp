#pragma once
#include <utility>


//*********LIST ITERATOR CLASS ****************

template <typename pool_type, typename address_type, typename T>
class list_iterator{

  pool_type* pool;
  address_type ind;

public:
  using value_type = T;
  explicit list_iterator(pool_type* pool, address_type ind) : pool(pool), ind(ind){}

  list_iterator& operator++(){
    ind = pool->pool.at(ind - 1).next;
    return *this;
  }

  list_iterator operator++(int){
    list_iterator tmp(ind);
    ind = pool->pool.at(ind - 1).next;
    return tmp;
  }

  value_type& operator*()const{
    return pool->pool.at(ind - 1).value;
  }

};

//************************---------**************************

//******************** LIST POOL ****************************

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
  //using pool_type = std::vector<node_t>;
  list_type free_node_list; // at the beginning, it is empty

  node_t& node(list_type x) noexcept { return pool[x-1]; }
  const node_t& node(list_type x) const noexcept { return pool[x-1]; }

public:

  list_pool()noexcept{}
  explicit list_pool(size_type n)noexcept{
    pool.reserve(n);
  } // reserve n nodes in the pool

  using iterator = list_iterator<list_pool, N, T>;
  using const_iterator = list_iterator<const list_pool, N, const T>;

  iterator begin(list_type x){
    return iterator(this, x);
  }
  iterator end(list_type ){
    return iterator(this, list_type(0));
  }// this is not a typo

  const_iterator begin(list_type x) const{
    return const_iterator(this, x);
  }
  const_iterator end(list_type ) const{
    return const_iterator(this, list_type(0));
  }

  const_iterator cbegin(list_type x) const{
    return const_iterator(this, x);
  }
  const_iterator cend(list_type ) const{
    return const_iterator(this, list_type(0));
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
 //************ PUSH FRONT *****************
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

//************ PUSH BACK ******************
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

//******** DELETE FIRST NODE **********
  list_type free(list_type x){
    if(x == end())
      return end();

    const list_type head = next(x - 1);

    const list_type fnl = free_node_list;
    free_node_list = x;
    next(x - 1) = fnl;
    return head;
  } // delete first node
  // ************FREE ENTIRE LIST*****************
  list_type free_list(list_type x){
    list_type head = free(x);
    while(head != end()){
      head = free(head);
    }
    return head;
  } // free entire list
};
