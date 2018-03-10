//Copyright[2002] MasangSoft
#pragma once

// Ares Coordinate Units
using CoordUnit_t = SHORT;

// Ares 3-D Vector
struct AVECTOR3
{
    CoordUnit_t    x;
    CoordUnit_t    y;
    CoordUnit_t    z;

    AVECTOR3() : x { 0 }, y { 0 }, z { 0 } { }

    AVECTOR3(CoordUnit_t i_x, CoordUnit_t i_y, CoordUnit_t i_z) : x { i_x }, y { i_y }, z { i_z } { }

    // ReSharper disable once CppNonExplicitConvertingConstructor
    AVECTOR3(const D3DXVECTOR3& rhs) :
        x { CoordUnit_t(rhs.x) },
        y { CoordUnit_t(rhs.y) },
        z { CoordUnit_t(rhs.z) } { }

    AVECTOR3& operator=(const D3DXVECTOR3& rhs)
    {
        x = CoordUnit_t(rhs.x);
        y = CoordUnit_t(rhs.y);
        z = CoordUnit_t(rhs.z);
        return *this;
    }

    D3DXVECTOR3& CopyA2DX(D3DXVECTOR3& outVec) const
    {
        outVec.x = float(x);
        outVec.y = float(y);
        outVec.z = float(z);
        return outVec;
    }

    void Reset() { x = y = z = 0; }
};

// Convert AVECTOR3 to D3DXVECTOR3
inline D3DXVECTOR3 A2DX(const AVECTOR3& vec3) { return { float(vec3.x), float(vec3.y), float(vec3.z) }; }
// Convert AVECTOR3* to D3DXVECTOR3
inline D3DXVECTOR3 AP2DX(const AVECTOR3* vec3) { return { float(vec3->x), float(vec3->y), float(vec3->z) }; }
