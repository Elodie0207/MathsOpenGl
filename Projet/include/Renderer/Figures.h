//
// Created by Enola on 27/02/2024.
//

#ifndef MATHSOPENGL_FIGURES_H
#define MATHSOPENGL_FIGURES_H

#include <memory>
#include <vector>
#include "Core.hpp"
#include "Renderer/Texture.hpp"



class Figures {
public:
     void DrawHeart(const Mat4 &ViewProjMatrix) const ;
    Vec2 m_Position = Vec2(0.0);
    Vec2 m_Size = Vec2(1.0);
    Ref<Texture> m_Texture = nullptr;
private:
    std::array<Vec3, 4> vertices = {Vec3(-0.5, -0.5, 0.0), Vec3(+0.5, -0.5, 0.0), Vec3(+0.5, +0.5, 0.0), Vec3(-0.5, +0.5, 0.0)};
    std::array<Vec2, 4> uvPos = { Vec2(1.0, 0.0), Vec2(1.0, 1.0), Vec2(0.0, 1.0), Vec2(0.0, 0.0)};

    void DrawStar(const Mat4 &ViewProjMatrix) const;

    void DrawPentagone(const Mat4 &ViewProjMatrix) const;

    void DrawHexagone(const Mat4 &ViewProjMatrix) const;

    void DrawEllipse(const Mat4 &ViewProjMatrix) const;
};


#endif //MATHSOPENGL_FIGURES_H
