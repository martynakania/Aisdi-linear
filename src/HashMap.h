#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  class Node;

private:
    int size;
    int bucketCount;
    Node**tab;
    static const int BUCKETINIT=101;
public:
  HashMap(int pBucketCount = BUCKETINIT) : size(0), bucketCount(pBucketCount)
  {
    tab = new Node*[bucketCount]();
  }

  HashMap(std::initializer_list<value_type> list):HashMap()
  {
    for(auto&& item :list)
        insert(item.first, item.second);
  }

  HashMap(const HashMap& other):HashMap(other.bucketCount):HashMap()
  {
    for(auto&& item :other)
        insert(item.first, item.second);
  }

  HashMap(HashMap&& other)
  {
    deleteMap();
    size=other.size;
    bucketCount=other.bucketCount;
    tab = new Node*[bucketCount]();
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    throw std::runtime_error("TODO");
  }

  mapped_type& operator[](const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const_iterator find(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  iterator find(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO");
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    throw std::runtime_error("TODO");
  }

  iterator end()
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
private:
  size_t hash(const key_type& key) const
  {
      return std::hash<key_type>{}(key) % bucketCount;
  }/*
  void insert(const value_type& pPair)
  {
      Node* newNode=insert(pPair.first);
     //newNode->pair.second=pPair.second;
  }*/
  Node* insert(const key_type& key,  mapped_type value=ValueType())
  {
      ++size;
      Node* newNode= new Node(key, ValueType());
      size_t indx=hash(key);
      if(tab[indx]!=nullptr){
          tab[indx]->prev=newNode;
          newNode->nxt=tab[indx];
      }
      tab[indx]=newNode;
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

  explicit ConstIterator()
  {}

  ConstIterator(const ConstIterator& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
    throw std::runtime_error("TODO");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};
template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>:: Node
{
    friend class HashMap;
    Node* nxt;
    Node* prev;
    value_type pair;
public:
    Node() : pair(), nxt(nullptr), prev(nullptr)
            {};
    Node(KeyType key, ValueType value) : pair(std::make_pair(key,value)), nxt(nullptr), prev(nullptr)
            {}
};
}

#endif /* AISDI_MAPS_HASHMAP_H */
