#include "QuadTree.h"

void catengine::QuadTree::Node::collect(catengine::GameObject const& o, std::vector<catengine::GameObject*>& vo) const
{
  // if o doesn't have a collider then we don't want to check for it
  for (auto d : data_) {
    vo.push_back(d);
  }
  if (children_ == nullptr) return;

  // look at the children
  bool left = true; // if left corner to the left of the center
  bool right = true; // right corner to the right of the center
  bool top = true; // top to the top of the center
  bool bottom = true; // bottom to the bottom of the center
  if (top && right) children_[TOP_RIGHT]->collect(o, vo);
  if (top && left) children_[TOP_LEFT]->collect(o, vo);
  if (bottom && left) children_[BOTTOM_LEFT]->collect(o, vo);
  if (bottom && right) children_[BOTTOM_RIGHT]->collect(o, vo);
}

void catengine::QuadTree::Node::split()
{
  children_ = new Node*[CHILD_COUNT];
  for (size_t i = 0; i < CHILD_COUNT; ++i) {
    children_[i] = new Node(this);
  }

  for (auto& d : data_) {
    insert(d);
  }
}

void catengine::QuadTree::Node::insert(catengine::GameObject* o)
{
}