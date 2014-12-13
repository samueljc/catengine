// QuadTree.h
// Written by: Samuel Clark

#ifndef QUAD_treeH
#define QUAD_treeH

#include <vector>
#include <functional>

#include "Types.h"
#include "Logging.h"
#include "GameObject.h"
#include "Rectangle.h"
#include "Renderer.h"

namespace catengine {
  class QuadTree {
    typedef std::function<Rectangle (GameObject const&)> rect_func;

  public:
    QuadTree(Rectangle const& bounds, 
        _unsigned split_size = 10,
        _unsigned prune_size = 5, 
        _unsigned max_depth = 5, 
        rect_func f = GameObject::get_bounding_rect) :
      root_(nullptr, bounds),
      split_size_(split_size),
      prune_size_((prune_size < split_size) ? prune_size : split_size - 1),
      max_depth_(max_depth),
      func_(f)
    {
      root_.tree = this;
    }

    inline _unsigned split_size() const { return split_size_; }
    inline void split_size(_unsigned split_size) { split_size_ = split_size; }

    inline _unsigned prune_size() const { return prune_size_; }
    inline void prune_size(_unsigned prune_size) { prune_size_ = prune_size; }

    inline _unsigned max_depth() const { return max_depth_; }
    inline void max_depth(_unsigned max_depth) { max_depth_ = max_depth; }

    inline Rectangle func(GameObject const& o) const { return func_(o); }
    inline void func(rect_func f) { func_ = f; }

    /**
     * Retrieve the number of objects contained in this tree.
     * @return unsigned of the object count
     */
    inline _unsigned count() const { return root_.count; }

    /**
     * Remove everything contained in this tree.
     */
    inline void clear()
    { 
      root_.children.clear();
      root_.data.clear();
      root_.count = 0;
    }

    /**
    * Insert the given game object reference into our collection of game
    * objects by recursively going through the children until we find the
    * appropriate node.
    * @param o A pointer to the object we want to insert.
    */
    inline void insert(GameObject* o) { root_.insert(o); }

    /**
    * Collect all of the potential collisions with o and store them in vo.
    * @param r Range we want to collect in.
    */
    inline std::vector<GameObject*> collect(Rectangle const& r) const {
      std::vector<GameObject*> rv;
      if (r.width == 0.0f || r.height == 0.0f) return rv;
      root_.collect(r, rv);
      return rv;
    }

    /**
     * Rebalance the tree moving everything into the new appropriate node.
     */
    inline void rebalance() { root_.rebalance(); }
    
    inline void draw(Renderer& renderer) const {
      root_.draw(renderer);
    }

  private:
    struct Node {
      static const _flag LEFT = 1 << 0;
      static const _flag RIGHT = 1 << 1;
      static const _flag TOP = 1 << 2;
      static const _flag BOTTOM = 1 << 3;

      static const _unsigned TOP_LEFT = 0;
      static const _unsigned TOP_RIGHT = 1;
      static const _unsigned BOTTOM_LEFT = 2;
      static const _unsigned BOTTOM_RIGHT = 3;
      static const _unsigned CHILD_COUNT = 4;

      std::vector<GameObject*> data;
      Node* parent;
      std::vector<Node> children;
      QuadTree const* tree;
      _unsigned depth;
      _unsigned count;
      Rectangle bounds;
      _decimal mid_x;
      _decimal mid_y;

      Node(Node* parent, Rectangle const& bounds) :
        parent(parent),
        tree((parent != nullptr) ? parent->tree : nullptr),
        depth((parent != nullptr) ? parent->depth + 1 : 0),
        count(0),
        bounds(bounds),
        mid_x(bounds.center_x()),
        mid_y(bounds.center_y())
      {
      }

      inline bool is_split() const { return children.size() > 0; }
      inline bool can_split() const 
      {
        return data.size() >= tree->split_size() && depth < tree->max_depth();
      }

      /**
       * Build a bitmask to signify which regions this rectangle touches in
       * the bounds.
       * @param The area we want to check over.
       */
      inline _flag get_range(Rectangle const& r) const {
        _flag range = (r.left() <= mid_x) << 0;
        range |= (mid_x <= r.right()) << 1;
        range |= (r.top() <= mid_y) << 2;
        range |= (mid_y <= r.bottom()) << 3;
        return range;
      }

      void insert(GameObject* go);
      void split();
      void collect(Rectangle const& rect, std::vector<GameObject*>&) const;
      void rebalance();
      void draw(Renderer& renderer) const;
    };

    Node root_;
    _unsigned split_size_;
    _unsigned prune_size_;
    _unsigned max_depth_;
    rect_func func_;
  };
}

#endif