#include "model.hpp"
#include "pch.h"
#include "renderer.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define DEG_TO_RAD(x) ((x / 180.0f) * 3.14159f)

int main()
{
  const int width = 1280, height = 720;
  const float near = 0.1f, far = 1000.f;

  Model model("../assets/objs/teapot.obj");
  Camera camera({width, height, near, far, 80.f});

  Renderer renderer(width, height, near, far);
  sf::RenderWindow window;
  sf::Sprite sprite;
  sf::Texture texture;
  sf::Clock clock;

  window.create(sf::VideoMode(width, height), "Rad Renderer", sf::Style::Default);
  texture.create(width, height);
  sprite.setTexture(texture);

  bool mouse_down = false;
  int prev_x, prev_y;
  float forward = 0.f;
  float rotation_speed = 0.001f;
  float cam_movement = 0.f;

  while (window.isOpen())
  {
    float dx = 0.f;
    float dy = 0.f;

    // Events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape))
        window.close();

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::W)
      {
        forward = -1.f;
      }

      if (event.type == sf::Event::KeyReleased &&
          event.key.code == sf::Keyboard::W)
      {
        forward = 0.f;
      }

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::S)
      {
        forward = 1.f;
      }

      if (event.type == sf::Event::KeyReleased &&
          event.key.code == sf::Keyboard::S)
      {
        forward = 0.f;
      }

      if (event.type == sf::Event::MouseButtonPressed)
      {
        mouse_down = true;

        prev_x = event.mouseButton.x;
        prev_y = event.mouseButton.y;
      }

      if (event.type == sf::Event::MouseButtonReleased)
        mouse_down = false;

      if (event.type == sf::Event::MouseMoved)
      {
        if (mouse_down)
        {
          int x = event.mouseMove.x;
          int y = event.mouseMove.y;

          dx = (float)(x - prev_x);
          dy = (float)(y - prev_y);
        }
      }
    }

    float elapsed_time = clock.restart().asMilliseconds();

    // Update camera
    cam_movement += forward * elapsed_time * 0.001f;
    camera.SetPosition(mathz::Vec3(0.f, 0.f, cam_movement));

    // Update model
    mathz::Quaternion qx(DEG_TO_RAD(dy * elapsed_time * rotation_speed), {1.f, 0.f, 0.f});
    mathz::Quaternion qy(DEG_TO_RAD(dx * elapsed_time * rotation_speed), {0.f, 1.f, 0.f});

    model = model.GetQuaternion() * qx * qy;
    model.Translate(0.f, -3.f, 6.f);

    renderer.Render(model, camera);

    texture.update(renderer.GetFrameBuffer());
    window.draw(sprite);
    window.display();
    window.clear();
  }
  return 0;
}