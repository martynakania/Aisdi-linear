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
    for(size_t i=0; i<bucketCount; ++i)
        tab[i]=nullptr;
  }

  HashMap(std::initializer_list<value_type> list):HashMap()
  {
    for(auto&& item :list)
        insert(item.first, item.second);
  }

  HashMap(const HashMap& other):HashMap(other.bucketCount)
  {
    for(auto&& item :other)
        insert(item.first, item.second);
  }

  HashMap(HashMap&& other):HashMap(other.bucketCount)
  {
    size=other.size;
    std::swap(tab, other.tab);
  }

  HashMap& operator=(const HashMap& other)
  {
    deleteElementsOfHashMap();
    *this=HashMap(other);
  }

  HashMap& operator=(HashMap&& other)
  {
      std::swap(tab, other.tab);
      size= other.size;
      bucketCount= other.bucketCount;
      other.size=0;
  }

  bool isEmpty() const
  {
    return (size==0);
  }

  mapped_type& operator[](const key_type& key)
  {
    Node*node=findNode(key);
    if(node==nullptr)
        node=insert(key);
    return node->pair.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    Node* node = findNode(key);
    if (node == nullptr)
        throw std::out_of_range("Key does not exists");
    return node->pair.second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    Node* node = findNode(key);
    if (node == nullptr)
        throw std::out_of_range("Key does not exists");
    return node->pair.second;
  }

  const_iterator find(const key_type& key) const
  {
    return ConstIterator(*this, findNode(key) );
  }

  iterator find(const key_type& key)
  {
    return Iterator(*this, findNode(key) );
  }

  void remove(const key_type& key)
  {
    Node*toRemove=findNode(key);
    if(toRemove==nullptr)
        throw std::out_of_range("can not remove key that not exist in tree");
    toRemove->nxt->prev=toRemove->prev;
    toRemove->prev->nxt=toRemove->nxt;
    delete toRemove;
  }

  void remove(const const_iterator& it)
  {
    if (it == end())
        throw std::out_of_range("Can not remove the end");
    remove(it->first);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    if(size!=other.size)
        return false;
    Node*tempNode;
    for(auto&& item:other){
        tempNode=findNode(item.first);
        if(tempNode==nullptr || tempNode->pair.second!=item.second)
            return false;
    }
    return true;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

   iterator begin()
  {
    return iterator(*this, getFirst() );
  }

  iterator end()
  {
    return iterator(*this, nullptr);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(*this, getFirst() );
  }

  const_iterator cend() const
  {
    return ConstIterator(*this, nullptr );
  }
  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

  ~HashMap()
  {
    deleteElementsOfHashMap();
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
      return newNode;
  }
  Node* findNode(key_type key) const
  {
    size_t idx=hash(key);
    Node*tempNode=tab[idx];
    while(tempNode!=nullptr){
       if(tempNode->pair.first==key)
            return tempNode;
       tempNode=tempNode->nxt;
    }
    return nullptr;
  }
  Node*getFirst() const
  {
      size_t idx=0;
      Node*node=nullptr;
      while(node==nullptr && idx<bucketCount){
          node=tab[idx];
      }
      return node;
  }
  void deleteElementsOfHashMap()
  {
    Node* node;
    Node* temp;
    for (size_type i = 0; i < bucketCount; ++i) {
        node = tab[i];
        while (node != nullptr) {
            temp = node;
            node = node->nxt;
            delete temp;
            --size;
        }
    }
    delete[]tab;
  }
  Node*getlast() const
  {
      if(size==0)
        return nullptr;
      size_t idx=bucketCount;
      Node*node=nullptr;
      while(idx>=0){
        node=tab[idx];
        if(node!=nullptr)
            while(node->nxt=nullptr){
            node=node->nxt;
            }
      }
      return node;
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
  const HashMap& hashMap; //Node**tab;//
  Node*currentNode;
  size_t currentIdx;

public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

  explicit ConstIterator(const HashMap& pMap, Node* pNode): hashMap(pMap), currentNode(pNode)
    {
        if(pNode==nullptr)
            currentIdx=hashMap.bucketCount;
        else
            currentIdx=findIndex(pNode->pair.first);
    }

  ConstIterator(const ConstIterator& other): hashMap(other.hashMap), currentNode(other.currentNode), currentIdx(other.currentIdx)
        {}

  ConstIterator& operator++()
  {
     if(currentNode==nullptr)
        throw std::out_of_range("can not increase end");
     currentNode=currentNode->nxt;
     if(currentNode!=nullptr){
        return *this;
     }
     while(currentNode==nullptr && ++currentIdx<hashMap.bucketCount){
         currentNode=hashMap.tab[currentIdx];
     }
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator preIncremented(*this);
    operator++();
    return preIncremented;
  }

  ConstIterator& operator--()
  {
     if(hashMap.size==0)
        throw std::out_of_range("can not deincrement empty collection");
     if(currentNode==nullptr){
        currentNode=hashMap.getlast();
        return *this;
     }
     if(currentNode==hashMap.getFirst())
        throw std::out_of_range("can not deincrement begin");
     currentNode=currentNode->prev;
     while(currentNode==nullptr && --currentIdx>0){
         currentNode=hashMap.tab[currentIdx];
     }
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator preDecremented(*this);
    operator--();
    return preDecremented;
  }

  reference operator*() const
  {
    if(currentNode==nullptr)
        std::out_of_range("can not dereferent iterator of end");
    return currentNode->pair;
  }

  pointer operator->() const
  {
    if(currentNode==nullptr)
        std::out_of_range("can not dereferent iterator of end");
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (currentNode==other.currentNode && hashMap.tab==other.hashMap.tab);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
protected:
    size_t findIndex(KeyType key) const
    {
        size_t idx=hashMap.hash(key);
        Node* node=hashMap.tab[idx];
        while (node!=nullptr){
          if(node->pair.first==key)
                return idx;
        }
        return hashMap.bucketCount;//not find key
    }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator(const HashMap& pMap, Node* pNode)
        : ConstIterator(pMap, pNode)
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
