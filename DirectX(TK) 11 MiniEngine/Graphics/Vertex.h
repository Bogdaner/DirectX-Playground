
#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() : pos{ 0.0f, 0.0f, 0.0f }, texcoord{ 0.0f, 0.0f } {};
    Vertex( float x, float y, float z, float u, float v ) : pos{ x, y, z }, texcoord{ u, v } {};
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 texcoord;
};