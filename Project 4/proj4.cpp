/*
 * CSU0013 Data Structures
 * Project 4
 * Hash Table
 */

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>

// User specified generic hash functor.
template <typename K>
class hasher{
public:
    size_t operator()(int i, const K &key, size_t size);
private:
    size_t h1(const K &key, size_t size);
    size_t h2(const K &key, size_t size);
};

template <typename K, typename V>
class _item_type{
public:
    _item_type(void) : isdeleted(false), isempty(true) {};
    K &key() { return std::get<0>(_item); }
    V &value() { return std::get<1>(_item); }
    bool isempty;
    bool isdeleted;
private:
    std::tuple<K, V> _item;
};

// Generic hash table with open addresing using double hashing
// Will double the table when getting full.
template <typename K, typename V, typename Hasher = hasher<K>>
class hash_table{
public:
    typedef _item_type<K, V> item_type;
    hash_table(const size_t size = 128);
    ~hash_table(void);
    bool remove(const K &key);
    bool empty(void) const;
    bool find(const K &key) const;
    size_t size(void) const;
    size_t item_count(void) const;
    int resize(size_t size);
    V &operator[](const K &key);
private:
    size_t _find(const K &key) const;
    size_t _item_count;
    size_t _size;
    Hasher _hash;
    item_type *_list;
};

int main(void) {
    hash_table<int, std::string> i_table(131072);
    hash_table<std::string, int> s_table(131072);
    int data;
    scanf("%d", &data);
    char buf[16];
    int num;
    for(int i = 0; i < data; ++i) {
        scanf("%s %d", buf, &num);
        i_table[num] = std::string(buf);
        s_table[std::string(buf)] = num;
    }
    int i = 0;
    while(scanf("%d", &num)) {
        ++i;
        if(!num)
            break;
        if(num == 1) {
            scanf("%d", &num);
            printf("%s\n", i_table[num].c_str());
        }
        else if(num == 2) {
            scanf("%s", buf);
            printf("%d\n", s_table[std::string(buf)]);
        }
    }
    return 0;
}

template <typename K>
inline size_t hasher<K>::operator()(int i, const K &key, size_t size) {
    return (h1(key, size) + i * h2(key, size)) % size;
}

template <>
inline size_t hasher<int>::h1(const int &key, size_t size) {
    return key % size;
}

template <>
inline size_t hasher<int>::h2(const int &key, size_t size) {
    return (key % (size - 1)) | 1;
}

template <>
inline size_t hasher<std::string>::h1(const std::string &key,  __attribute__((unused))size_t size) {
    size_t h = 5381;
    for(size_t i = 0; i < key.length(); ++i)
        h = ((h << 5) + h) + key[i];
    return h;
}

template <>
inline size_t hasher<std::string>::h2(const std::string &key, size_t size) {
    size_t c = 0;
    for(size_t i = 0; i < key.length(); ++i, c += key[i]);
    return (c % (size - 1)) | 1;
}

// Initialize the table with specified size, default size=128
template <typename K, typename V, typename Hasher>
inline hash_table<K, V, Hasher>::hash_table(const size_t size) 
    : _size(size), _item_count(0)
{
    _list = new item_type[size];
}

template <typename K, typename V, typename Hasher>
inline hash_table<K, V, Hasher>::~hash_table(void) {
   delete[] _list;
} 

// Remove the item specified the by key from table, return false if not found.
template <typename K, typename V, typename Hasher>
bool hash_table<K, V, Hasher>::remove(const K &key) {
    size_t addr = _find(key);
    if(addr == _size + 1)
        return false;
    _list[addr].isempty = true;
    _list[addr].isdeleted = true;
    --_item_count;
    return true;
}

// Return if table contains the key
template <typename K, typename V, typename Hasher>
inline bool hash_table<K, V, Hasher>::find(const K &key) const {
    return _find(key) != _size + 1;
}

template <typename K, typename V, typename Hasher>
inline size_t hash_table<K, V, Hasher>::item_count(void) const {
    return _item_count;
}

// Return if the table contains any item;
template <typename K, typename V, typename Hasher>
inline bool hash_table<K, V, Hasher>::empty(void) const {
    return _item_count == 0;
}

// Return addr of key, _size + 1 if not found.
template <typename K, typename V, typename Hasher>
size_t hash_table<K, V, Hasher>::_find(const K &key) const {
    size_t addr = 0;
    for(int i = 0;
            key != _list[addr].key() && !(_list[addr].isempty && !_list[addr].isdeleted);
            addr = _hash(i, key, _size));
    if (_list[addr].isempty && _list[addr].isdeleted)
        addr = _size + 1;
    return addr;
}

// Return the size of table
template <typename K, typename V, typename Hasher>
inline size_t hash_table<K, V, Hasher>::size(void) const {
    return _size;
}

// Resize the table to specified size, new size must greater than old size.
template <typename K, typename V, typename Hasher>
int hash_table<K, V, Hasher>::resize(size_t size) {
    size_t old_size = _size;
    item_type *old_list = _list;
    _size = size;
    _list = new item_type[size];
    _item_count = 0;
    for(size_t i = 0; i < old_size; ++i) {
        if(!old_list[i].isempty)
            (*this)[old_list[i].key()] = old_list[i].value();
    }
    delete[] old_list;
    return _size;
}

// Return the value of corresponding key as lvalue, insert one if no exist.
template <typename K, typename V, typename Hasher>
V &hash_table<K, V, Hasher>::operator[](const K &key){
    if(_item_count + 1 == _size)
        resize(_size * 2);
    size_t addr = 0;
    for(int i = 0;
            key != _list[addr].key() && !_list[addr].isempty ;
            addr = _hash(++i, key, _size)) 
    ;
    _item_count += _list[addr].isempty;
    _list[addr].isempty = false;
    _list[addr].key() = key;
    return _list[addr].value();
}
