#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
//jezeli rowny to po prawo
namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class Node;
  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  Node* root=nullptr;
  int size=0;

public:

  TreeMap()
  {}

  TreeMap(std::initializer_list<value_type> list)
  {
    for (auto&& item: list)
        insert(item);
  }

  TreeMap(const TreeMap& other)
  {
    for (auto&& item: other)
        insert(item);
  }

  TreeMap(TreeMap&& other)
  {
    Node*temp=root;
    root=other.root;
    size=other.size;
    other.root=temp;
    other.size=0;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(*this==other)//jak sie bedsie sypac zmien na this==other
        return *this;
    deleteTree(root);
    for (auto&& item: other)
        insert(item);
    return *this;

  }

  TreeMap& operator=(TreeMap&& other)
  {
    if(*this==other)
        return *this;
    deleteTree(root);
    root=other.root;
    size=other.size;
    other.root=nullptr;
    other.size=0;
    return *this;
  }

  bool isEmpty() const
  {
    return (size==0);
  }

  mapped_type& operator[](const key_type& key)
  {
    Node*temp=findNode(key);
    if(temp!=nullptr)
        return temp->pair.second;
    temp=insert(key);
    return temp->pair.second;
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
    Node* node = findNode(key);//!!albo tu constcast albo wyzej const temp=to
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
    Node* toRemove=findNode(key);
    if(toRemove==nullptr)
        throw std::out_of_range("can not remove key that not exist in tree");
    if(toRemove==root)
        deleteRoot();
    else if(toRemove->parent->pair.first>toRemove->pair.first)
        deleteLeftChild(toRemove);
    else deleteRightChild(toRemove);
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

  bool operator==(const TreeMap& other) const
  {
    if (size != other.size)
        return false;
    for (auto&& item : other) {
        Node* node = findNode(item.first);
        if (node == nullptr || node->pair.second != item.second)
            return false;
        }
    return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return iterator(*this, theLowest() );
  }

  iterator end()
  {
    return iterator(*this, nullptr);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(*this, theLowest() );
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
  ~TreeMap()
  {
    deleteTree(root);
  }
private:
void insert(value_type newPair)
{
    Node* temp=insert(newPair.first);
    temp->pair.second=newPair.second;
}
Node* insert(key_type key)
{
    Node* newNode= new Node(std::make_pair(key, ValueType()));
    newNode->left=nullptr;
    newNode->right=nullptr;
    ++size;
    if(root==nullptr){
        root=newNode;
        root->parent=nullptr;
        return root;
    }
    Node* temp=root;
    Node* parent;
    while(true){
        parent=temp;
        if(temp->pair.first > newNode->pair.first){
            temp=temp->left;
            if(temp==nullptr){
                newNode->parent=parent;
                parent->left=newNode;
                break;
            }
        }
        else{
            temp=temp->right;
            if(temp==nullptr){
                newNode->parent=parent;
                parent->right=newNode;
                break;
            }
        }
    }
    return newNode;
  }
  void deleteTree(Node* node)
  {
    if(node==nullptr)
        return;
    deleteTree(node->left);
    deleteTree(node->right);
   // delete nodeToStart;
    //--size;

    if (node->left != nullptr) {
        --size;
        delete node->left;
        node->left = nullptr;
    }
    if (node->right != nullptr) {
        --size;
        delete node->right;
        node->right = nullptr;
    }
    if (node == root) {
        delete root;
        root = nullptr;
        --size;
    }
  }
  Node* findNode(const key_type& key) const
  {
      Node* node=root;
      while (node!=nullptr)
      {
          if(node->pair.first==key)
            return node;
          if(node->pair.first>key)
            node=node->left;
          else
            node=node->right;
      }
      return nullptr;
  }
  Node* theLowest() const//!!pytanie o brak consta i wywo章nie iteratora
  {
    if(isEmpty())
        return nullptr;
    Node* temp=root;
    while(temp->left!=nullptr)
        temp=temp->left;
    return temp;
  }
  void deleteRoot()
  {
      --size;
      Node*oldRoot=root;
      if(root->right==nullptr){
          root=root->left;
          delete oldRoot;
          return;
      }
      root=root->right;
      if(oldRoot->left!=nullptr){
        Node*temp=root;
        while(temp->left!=nullptr)
            temp=temp->left;
        temp->left=oldRoot->left;
      }
      delete oldRoot;
  }
  void deleteRightChild(Node* toRemove)
  {
      --size;
      if(toRemove->left==nullptr){
          toRemove->parent->right=toRemove->right;
          delete toRemove;
          return;
      }
      Node*temp=toRemove->left;
      toRemove->parent->right=temp;
      if(toRemove->right!=nullptr){
        while(temp->right!=nullptr)
            temp=temp->right;
        temp->right=toRemove->right;
      }
      delete toRemove;
  }
  void deleteLeftChild(Node* toRemove)
  {
     --size;
      if(toRemove->right==nullptr){
        toRemove->parent->left=toRemove->left;
        delete toRemove;
        return;
      }
      toRemove->parent->left=toRemove->right;
      if(toRemove->left!=nullptr){
        Node*temp=toRemove->right;
        while(temp->left!=nullptr)
            temp=temp->left;
        temp->left=toRemove->left;
      }
      delete toRemove;
      return;
  }
  Node* getLast() const
  {
      Node* node=root;
      while(node->right!=nullptr)
        node=node->right;
      return node;
  }

};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
private:
    const TreeMap& tree;
    Node* current;
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  explicit ConstIterator(const TreeMap& pTree, Node* node): tree(pTree), current(node)
        {};

  ConstIterator( const ConstIterator& other): tree(other.tree), current(other.current)
        {};

  ConstIterator& operator++()
  {
    if(current==nullptr)
        throw std::out_of_range("can not increment the end");
    if(current->right!=nullptr){
        current=current->right;
        while(current->left!=nullptr)
            current=current->left;
        return *this;
    }
    while(current->parent!=nullptr && current->parent->left!=current){
        current=current->parent;
    }
    current=current->parent;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator temp(*this);
    operator++();
    return temp;
  }

  ConstIterator& operator--()
  {
    if(tree.isEmpty())
        throw std::out_of_range("can not decrement iterator of empty collection");
    if(current==nullptr){
        current=tree.getLast();
        return *this;
    }
    if(current->left!=nullptr){
        current=current->left;
        while(current->right!=nullptr)
            current=current->right;
        return *this;
    }
    do{
        if(current->parent==nullptr)
            throw std::out_of_range("can not decrement begin");//!!czy sprawdza to jakis test-> nie dopisz
        if(current->parent->right==current)
            break;
        current=current->parent;
    }
    while(true);
    current=current->parent;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator temp(*this);
    operator++();
    return temp;
  }

  reference operator*() const
  {
    //if(tree.isEmpty())
       // throw std::out_of_range("can not refer iterator of empty collection"); takie cos pzechodzi testy
    if(current==nullptr)
        std::out_of_range("can not refer iterator of end");
    return current->pair;
  }

  pointer operator->() const
  {
    if(current==nullptr)
        std::out_of_range("can not derefer iterator of end");
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return ( current==other.current);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
  Node* getParent() const
  {
      return current->parent;
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Node
{
    friend class TreeMap;
  private:
      Node* parent;
      Node* left;
      Node* right;
      value_type pair;
  public:
    Node(): pair(std::make_pair(KeyType(), ValueType()) ),  parent(nullptr), left(nullptr), right(nullptr)
            {};
    Node(value_type pPair): pair(pPair), parent(nullptr), left(nullptr), right(nullptr)
            {};
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator( const TreeMap& tree, Node*node): ConstIterator(tree, node)
    {};

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

}

#endif /* AISDI_MAPS_MAP_H */
