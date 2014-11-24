#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "GameObject.h"
#include "QuadTree.h"
#include "Renderer.h"

namespace catengine {
  class Scene {
  public:
    Scene() :
      rendering_tree_({ 0.f, 0.f, 640.f, 480.f }, 3),
      collision_tree_({ 0.f, 0.f, 640.f, 480.f })
    {
      for (int i = 0; i < 100; ++i) {
        GameObject* p = new GameObject();
        p->position({ float(rand() % 600), float(rand() % 420) });
        objects_.emplace_back(GameObjectPtr(p));
      }

      for (auto ptr : objects_)
        rendering_tree_.insert(ptr);
    }

    void update() {
    }

    void render(Renderer& renderer) {
      renderer.begin_draw();

      renderer.set_thickness(1.f);
      rendering_tree_.draw(renderer);

      auto collection = rendering_tree_.collect({ 0.0f, 0.0f, 640.0f, 480.0f });
      renderer.set_color(catengine::Color::ELECTRIC_LIME);
      for (auto c : collection) {
        renderer.fill_rect(catengine::GameObject::get_bounding_rect(*c.get()));
      }

      catengine::Rectangle sel(200.0f, 200.0f, 100.0f, 100.0f);
      collection = rendering_tree_.collect(sel);
      renderer.set_color(catengine::Color::BLUE);
      renderer.draw_rect(sel);
      for (auto c : collection) {
        renderer.fill_rect(catengine::GameObject::get_bounding_rect(*c.get()));
      }

      renderer.end_draw();
    }

  private:
    std::vector<GameObjectPtr> objects_;
    QuadTree rendering_tree_;
    QuadTree collision_tree_;
  };
}

#endif