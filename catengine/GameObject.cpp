#include "GameObject.h"

void catengine::GameObject::update_transformation()
{

}

void catengine::GameObject::add_tag(std::string const& tag)
{
  tags_.push_back(tag);
}

void catengine::GameObject::remove_tag(std::string const& tag)
{
  for (auto i = tags_.begin(); i != tags_.end(); ++i)
  {
    if (*i == tag) {
      tags_.insert(i, *tags_.rbegin());
      tags_.pop_back();
    }
  }
}