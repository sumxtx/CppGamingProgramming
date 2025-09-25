#pragma once
#include "Graphics.hpp"
#include "SFML/Graphics.hpp"

class MenuGraphics : public Graphics
{
  private:
    FloatRect* m_MenuPosition = nullptr;
    int m_VertexStartIndex;
    bool *m_GameOver;
    bool m_CurrentStatus = false;

    int uPos;
    int vPos;
    int texWidth;
    int texHeight;

  public:
    void draw(VertexArray& canvas) override;
    void assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords) override;
};
