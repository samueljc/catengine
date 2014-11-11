#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <vector>

#include "GameObject.h"

namespace catengine {
  class QuadTree {
    struct Node {
    public:
      Node(Node& parent) :
        parent_(&parent)
      {
      }
      Node(Node* parent) :
        parent_(parent)
      {
      }

      /**
       * Free up any children if we have them.
       */
      ~Node()
      {
        if (children_ == nullptr) return;

        for (size_t i = 0; i < CHILD_COUNT; ++i) {
          delete children_[i];
        }
        delete[] children_;
      }

      inline void insert(GameObject& o)
      {
        insert(&o);
      }

      void collect(GameObject const& o, std::vector<GameObject*>& vo) const;

    private:
      std::vector<GameObject*> data_;
      Node** children_;
      Node* parent_;

      static const size_t TOP_RIGHT = 0;
      static const size_t TOP_LEFT = 1;
      static const size_t BOTTOM_LEFT = 2;
      static const size_t BOTTOM_RIGHT = 3;
      static const size_t CHILD_COUNT = 4;

      /**
       * Split this node into children.
       */
      void split();

      /**
       * Insert the given game object reference into our collection of game
       * objects by recurssively going through the children until we find the
       * appropriate node.
       * @param A pointer to the object we want to insert.
       */
      void insert(GameObject* o);
    };
  };
}

#endif