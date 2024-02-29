//
// Created by Enola on 27/02/2024.
//

#include "Renderer/Figures.h"
#include "Core.hpp"
#include <utility>
#include "Renderer/Texture.hpp"

void Figures::DrawHeart(const Mat4 &ViewProjMatrix) const {
    if (m_Texture != nullptr) {
        m_Texture->Bind();
    }

    glBegin(GL_TRIANGLE_FAN);

    Mat4 modelMatrix = Math::TRS(m_Position, m_Size);

    const float PI = 3.14159265f;
    const float heartSize = 0.2f;

    for (float t = 0.0f; t <= 2.0f * PI; t += 0.01f) {
        float x = 16.0f * sin(t) * sin(t) * sin(t);
        float y = 13.0f * cos(t) - 5.0f * cos(2.0f * t) - 2.0f * cos(3.0f * t) - cos(4.0f * t);

        Vec3 vertice(x * heartSize, y * heartSize, 0.0f);
        const Vec2 uv(x, y); //

        auto pos = modelMatrix * Vec4(vertice, 1.0);
        Vec3 v = ViewProjMatrix * pos;

        glColor3f(1.0, 0.0, 0.0);
        glVertex3fv(&v[0]);
        glTexCoord2fv(&uv[0]);
    }

    glEnd();

    if (m_Texture != nullptr) {
        Texture::Unbind();
    }
}

void Figures::DrawStar(const Mat4 &ViewProjMatrix) const {
    if (m_Texture != nullptr) {
        m_Texture->Bind();
    }

    glBegin(GL_TRIANGLE_FAN);

    Mat4 modelMatrix = Math::TRS(m_Position, m_Size);

    const float PI = 3.14159265f;
    const float armLength = 0.1f;
    const int numArms = 5;

    for (int i = 0; i <= numArms * 2; ++i) {
        float angle = static_cast<float>(i) * PI / numArms;
        float radius = i % 2 == 0 ? 0.5f : 1.0f;

        float x = radius * armLength * cos(angle);
        float y = radius * armLength * sin(angle);

        Vec3 vertice(x, y, 0.0f);
        const Vec2 uv(x, y);

        auto pos = modelMatrix * Vec4(vertice, 1.0);
        Vec3 v = ViewProjMatrix * pos;

        glColor3f(1.0, 1.0, 0.0);
        glVertex3fv(&v[0]);
        glTexCoord2fv(&uv[0]);
    }

    glEnd();

    if (m_Texture != nullptr) {
        Texture::Unbind();
    }
}
void Figures::DrawPentagone(const Mat4 &ViewProjMatrix) const {
    if (m_Texture != nullptr) {
        m_Texture->Bind();
    }

    glBegin(GL_POLYGON);
    const float PI = 3.14159265f;
    const int numSides = 5;
    for (int i = 0; i < numSides; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / numSides;
        float x = cos(angle) * 0.1;
        float y = sin(angle) * 0.1;
        glVertex2f(x, y);
    }
    glEnd();
}

void Figures::DrawHexagone(const Mat4 &ViewProjMatrix) const {
    if (m_Texture != nullptr) {
        m_Texture->Bind();
    }

    glBegin(GL_POLYGON);
    const float PI = 3.14159265f;
    const int numSides = 6;
    for (int i = 0; i < numSides; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / numSides;
        float x = cos(angle) * 0.1;
        float y = sin(angle) * 0.1;
        glVertex2f(x, y);
    }
    glEnd();
}

void Figures::DrawEllipse(const Mat4 &ViewProjMatrix) const {
    if (m_Texture != nullptr) {
        m_Texture->Bind();
    }

    glBegin(GL_TRIANGLE_FAN);
    const float PI = 3.14159265f;
    const int numSegments = 100;
    const float radiusX = 0.1;
    const float radiusY = 0.05;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / numSegments;
        float x = cos(angle) * radiusX;
        float y = sin(angle) * radiusY;
        glVertex2f(x, y);
    }
    glEnd();
}