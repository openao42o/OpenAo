#pragma once

#define D3DFVF_PARTICLE_VERTEX (D3DFVF_XYZ|D3DFVF_PSIZE|D3DFVF_DIFFUSE)
#define D3DFVF_SPRITE_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

const BodyCond_t _fist_bodycondition = 1;

#define FIRST_BODYCONDITION _fist_bodycondition // 64bit flag
#define VEL_COLOR_CHANGE	fElapsedTime * 50.0f

// object particle moving type
//#define OBJ_MOVE_TYPE0	0
//#define OBJ_MOVE_TYPE1	1
//#define OBJ_MOVE_TYPE2	2
//#define OBJ_MOVE_TYPE3	3

enum OBJECT_TYPE { OBJ_MOVE_TYPE0, OBJ_MOVE_TYPE1, OBJ_MOVE_TYPE2, OBJ_MOVE_TYPE3 };

// PARTICLE_TRACE_TYPE is not used
enum PARTICLE_TYPE { PARTICLE_OBJECT_TYPE, PARTICLE_SPRITE_TYPE, PARTICLE_TRACE_TYPE };
enum EMITTER_TYPE { EMITTER_POINT, EMITTER_CIRCLE, EMITTER_SPHERE, EMITTER_AREA };
enum TEXTURE_SIZE { TEXTURE_SIZE_RANDOM, TEXTURE_SIZE_STEP, TEXTURE_SIZE_FIX };
enum TEXTURE_ANIMATION { TEXTURE_ANIMATION_NONE = -1, TEXTURE_ANIMATION_RANDOM, TEXTURE_ANIMATION_STEP };
enum EFFECT_TYPE { EFFECT_TYPE_OBJECT, EFFECT_TYPE_SPRITE, EFFECT_TYPE_PARTICLE, EFFECT_TYPE_TRACE };


struct PARTICLE_VERTEX
{
    D3DXVECTOR3 p; // The position.
	float		s; // The PointSize
    D3DCOLOR    c; // The color.
};
struct SPRITE_VERTEX
{
    D3DXVECTOR3 p; // The position.
    D3DCOLOR    c; // The color.
	float		tu,tv;
};

struct SPRITE_GEOMETRY
{
	float		tu, tv; // Texture coordinates
	float		tx, ty; // Translation
};
struct SPRITE_INSTANCE
{
    D3DXVECTOR3 pos; // World position.
    D3DCOLOR    col; // The color.
	float		scalar, rot; // Scaling and rotation angle
};

struct TRACE_GEOMETRY
{
	float		tu, tv;					// Texture coordinates
	float		trans_h, trans_v;		// Indicates which offset to use
	float		usenewpos, useoldpos;	// Indicates which positon to use
};
struct TRACE_INSTANCE
{
	float		newscale, oldscale; // seperate scaling for each end of the trail
	float		newcol, oldcol;		// Colors to be used
	D3DXVECTOR3	newpos, oldpos;		// New and old positions of the plane
	D3DXVECTOR3	offset_h, offset_v;	// Horizontal and vertical offsets for translation, scaled
};


struct EffectFileHeader
{
	DWORD	dwProductID;
	DWORD	dwVersion;
	DWORD	dwEffectType;
};

struct Effect
{
	DWORD dwType;
	float fDistance;
};