#include "QuadTree.h"

#include "Tracer.h"

void catengine::QuadTree::Node::insert(GameObjectPtr o)
{
  ++count;
  Rectangle r = tree->func(*o);
  _flag range = get_range(r);

  if (!is_split() ||
      (range & (LEFT | RIGHT)) == (LEFT | RIGHT) || 
      (range & (TOP | BOTTOM)) == (TOP | BOTTOM)) {
    data.push_back(o);
    if (!is_split() && can_split()) split();
  }
  else if (range == (TOP | LEFT)) children[TOP_LEFT].insert(o);
  else if (range == (TOP | RIGHT)) children[TOP_RIGHT].insert(o);
  else if (range == (BOTTOM | LEFT)) children[BOTTOM_LEFT].insert(o);
  else if (range == (BOTTOM | RIGHT)) children[BOTTOM_RIGHT].insert(o);
}

void catengine::QuadTree::Node::split()
{
  _decimal cw = bounds.width / 2.0f;
  _decimal ch = bounds.height / 2.0f;

  children.reserve(4);
  children.emplace_back(Node(this, Rectangle(bounds.left(), bounds.top(), cw, ch)));
  children.emplace_back(Node(this, Rectangle(mid_x, bounds.top(), cw, ch)));
  children.emplace_back(Node(this, Rectangle(bounds.left(), mid_y, cw, ch)));
  children.emplace_back(Node(this, Rectangle(mid_x, mid_y, cw, ch)));

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

  if ((range & (TOP | LEFT)) == (TOP | LEFT)) children[TOP_LEFT].collect(r, vo);
  if ((range & (TOP | RIGHT)) == (TOP | RIGHT)) children[TOP_RIGHT].collect(r, vo);
  if ((range & (BOTTOM | LEFT)) == (BOTTOM | LEFT)) children[BOTTOM_LEFT].collect(r, vo);
  if ((range & (BOTTOM | RIGHT)) == (BOTTOM | RIGHT)) children[BOTTOM_RIGHT].collect(r, vo);
}

void catengine::QuadTree::Node::rebalance()
{
  LOG(FATAL) << "NOT IMPLEMENTED";
}

void catengine::QuadTree::Node::draw(Renderer& renderer) const
{
  renderer.draw_rect(bounds);
  for (auto child : children)
    child.draw(renderer);
}