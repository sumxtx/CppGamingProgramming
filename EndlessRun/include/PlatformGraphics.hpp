#pragma once
#include "Graphics.hpp"
#include "SFML/Graphics.hpp"

using namespace sf;

class PlatformGraphics : public Graphics
{
  private:
    FloatRect *m_Position = nullptr;
    int m_VertexStartIndex = -1;
  public:
    void draw(VertexArray& canvas) override;
    void assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords) override;
};
