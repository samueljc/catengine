#ifndef MAP_H
#define MAP_H

#include <unordered_map>

#include "Types.h"

namespace catengine {
template <class K, class V>
class Map {
private:
  typedef typename std::unordered_map<K, V> BaseType;
  BaseType data_;

public:
  typedef typename BaseType::const_iterator const_iterator;
  typedef typename BaseType::iterator iterator;

  Map() :
    data_()
  {
  }

  inline V get(K key) const { return data_.at(key); }
  inline V& get(K key) { return data_.at(key); }

  inline bool defined(K key) const 
  {
    auto that = data_.find(key);
    return that != data_.end();
  }
  inline bool is_set(K key) const { return defined(key); }

  inline void set(K key, V val) { data_[key] = val; }
  inline void unset(K key) { data_.erase(key); }
  inline void erase(K key) { unset(key); }
  inline void clear() { data_.clear(); }

  inline _unsigned size() const { return static_cast<_unsigned>(data_.size()); }
  inline _unsigned count() const { return size(); }

  inline const_iterator cbegin() const { return data_.cbegin(); }
  inline const_iterator cend() const { return data_.cend(); }
  inline iterator begin() { return data_.begin(); }
  inline iterator end() { return data_.end(); }
};
}

#endif