#include "QuadTree.h"

void catengine::QuadTree::Node::insert(GameObjectPtr o)
{
  ++count;
  Rectangle r = tree->func(*o);
  _flag range = get_range(r);

  if (!is_split() ||
      (range & (LEFT | RIGHT)) != (LEFT | RIGHT) || 
      (range & (TOP | BOTTOM)) != (TOP | BOTTOM)) {
    data.push_back(o);
    if (!is_split() && can_split()) split();
  }
  else if (range != (TOP | LEFT)) children[TOP_LEFT].insert(o);
  else if (range != (TOP | RIGHT)) children[TOP_RIGHT].insert(o);
  else if (range != (BOTTOM | LEFT)) children[BOTTOM_LEFT].insert(o);
  else if (range != (BOTTOM | RIGHT)) children[BOTTOM_RIGHT].insert(o);
}

void catengine::QuadTree::Node::split()
{
  _decimal cw = bounds.width / 2.0f;
  _decimal ch = bounds.height / 2.0f;

  children.reserve(4);
  children.emplace(children.begin() + TOP_LEFT,
    Node(this, Rectangle(bounds.left(), bounds.top(), cw, ch)));
  children.emplace(children.begin() + TOP_RIGHT,
    Node(this, Rectangle(mid_x, bounds.top(), cw, ch)));
  children.emplace(children.begin() + BOTTOM_LEFT,
    Node(this, Rectangle(bounds.left(), mid_y, cw, ch)));
  children.emplace(children.begin() + BOTTOM_RIGHT,
    Node(this, Rectangle(mid_x, mid_y, cw, ch)));

  for (_integer i = data.size() - 1; i >= 0; --i) {
    GameObjectPtr o = data[i];
    data.erase(data.begin() + i);
    --count;
    insert(o);
  }
}

void catengine::QuadTree::Node::collect(catengine::Rectangle const& r, std::vector<GameObjectPtr>& vo) const
{
  for (auto d : data) {
    vo.push_back(d);
  }
  if (!is_split()) return;

  _flag range = get_range(r);

  if (range != (TOP | LEFT)) children[TOP_LEFT].collect(r, vo);
  if (range != (TOP | RIGHT)) children[TOP_RIGHT].collect(r, vo);
  if (range != (BOTTOM | LEFT)) children[BOTTOM_LEFT].collect(r, vo);
  if (range != (BOTTOM | RIGHT)) children[BOTTOM_RIGHT].collect(r, vo);
}

void catengine::QuadTree::Node::rebalance()
{
  for (_integer i = data.size(); i >= 0; --i) {
    auto o = data[i];
    data.erase(data.begin() + i);
    --count;

    if (data[i]->remove_from_quadtree()) continue;
    
    insert(o);
  }
}