#include "stdafx.h"
#include "AtumApplication.h"
#include "EffectRender.h"
#include "GameDataLast.h"
#include "ParticleSystem.h"
#include "TraceAni.h"
#include "Camera.h"
//#include "fstream"

extern LPDIRECT3DDEVICE9 g_pD3dDev;

static bool SpriteShaderReady = false;
static bool TraceShaderReady = false;

static LPD3DXEFFECT SpriteEffect = nullptr;
static LPD3DXEFFECT TraceEffect = nullptr;

static const D3DVERTEXELEMENT9 SpriteDeclElements[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// texture uv
	{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// translation
	{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// world position
	{ 1, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },	// color
	{ 1, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	// scaling and rotation
	D3DDECL_END()
};
static const D3DVERTEXELEMENT9 TraceDeclElements[] =
{
	{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// texture uv
	{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// which translation
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	// which position
	{ 1, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	// front/back scaling
	{ 1, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	// front/back colors
	{ 1, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },	// front position
	{ 1, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6 },	// back position
	{ 1, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 7 },	// horizontal translation offset
	{ 1, 52, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 8 },	// vertical translation offset
	D3DDECL_END()
};

static LPDIRECT3DVERTEXDECLARATION9 SpriteDecl = nullptr;
static LPDIRECT3DVERTEXDECLARATION9 TraceDecl = nullptr;

static LPDIRECT3DVERTEXBUFFER9 SpriteGeometryBuffer = nullptr;
static LPDIRECT3DVERTEXBUFFER9 TraceGeometryBuffer = nullptr;

static LPDIRECT3DINDEXBUFFER9 SpriteIndexBuffer = nullptr;
static LPDIRECT3DINDEXBUFFER9 TraceIndexBuffer = nullptr;

static const char SpriteShaderText[] =
R"(

float4x4 View : VIEW;
float4x4 Proj : PROJECTION;

float4x4 Billboard;

texture ModelTexture : TEXTURE;

sampler2D textureSampler = sampler_state
{
    Texture = <ModelTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};
 
struct VertexShaderInput
{
	float2 uv : TEXCOORD0;
	float2 trans : TEXCOORD1;
    float3 pos : POSITION;
	float4 color : COLOR;
	float2 scalars : TEXCOORD2;
};
 
struct VertexShaderOutput
{
	float4 pos : POSITION;
	float4 col : COLOR;
    float2 uv : TEXCOORD;
};
 
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output = (VertexShaderOutput)0;
	
	float scale = 0.70710678f * input.scalars.x;
	float theta = 0.78539816f - input.scalars.y;
	
	float4 Position = float4(scale * cos(theta) * input.trans.x, scale * sin(theta) * input.trans.y, 0, 1);
	
	float4x4 thisBillboard = Billboard;
	
	thisBillboard._41_42_43 = input.pos.xyz;
	
	Position = mul(Position, thisBillboard);
    Position = mul(Position, View);
    Position = mul(Position, Proj);
	
	output.pos = Position;
	output.col = input.color;
    output.uv = input.uv;
	
    return (output);
}
 
float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
    float4 c = tex2D(textureSampler, input.uv);
	
    return float4(c.x * input.col.x, c.y * input.col.y, c.z * input.col.z, c.w * input.col.w);
}
 
technique Basic
{
    pass Pass1
    {
        VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 PixelShaderFunction();
    }
}

)";

static const char TraceShaderText[] =
R"(

float4x4 View : VIEW;
float4x4 Proj : PROJECTION;

texture ModelTexture : TEXTURE;

sampler2D textureSampler = sampler_state
{
	Texture = <ModelTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VertexShaderInput
{
	float2 uv : TEXCOORD0;
	float2 trans : TEXCOORD1;
	float2 pos : TEXCOORD2;

	float2 scaling : TEXCOORD3;
	float2 colors : TEXCOORD4;
	float3 newpos : TEXCOORD5;
	float3 oldpos : TEXCOORD6;
	float3 offh : TEXCOORD7;
	float3 offv : TEXCOORD8;
};

struct VertexShaderOutput
{
	float4 pos : POSITION;
	float4 col : COLOR;
	float2 uv : TEXCOORD;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;

	float3 offset = (input.trans.x * input.offv) + (input.trans.y * input.offh);

	float scale = (input.scaling.x * input.pos.x) + (input.scaling.y * input.pos.y);

	float3 position = (input.pos.x * input.newpos) + (input.pos.y * input.oldpos) + (offset * scale);

	output.pos = float4(position.xyz, 1.0f);

	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Proj);

	float color = (input.pos.x * input.colors.x) + (input.pos.y * input.colors.y);
	
	output.col = float4(color, color, color, color);

	output.uv = input.uv;

	return (output);
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	float4 c = tex2D(textureSampler, input.uv);

	return float4(c.x * input.col.x, c.y * input.col.y, c.z * input.col.z, c.w * input.col.w);
}

technique Basic
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}

)";

bool InitializeSpriteShader()
{
	static auto triedonce = false;

	if (triedonce) return SpriteShaderReady;

	triedonce = true;

	LPD3DXBUFFER errors;

	// With this method you load your effect file
	auto status = D3DXCreateEffect(g_pD3dDev, SpriteShaderText, sizeof(SpriteShaderText), NULL, NULL, /*D3DXSHADER_DEBUG*/ 0, NULL, &SpriteEffect, &errors);

	if (errors)
	{
		MessageBox(NULL, LPSTR(errors->GetBufferPointer()), NULL, MB_OK);

		errors->Release();

		return false;
	}
	
	if (status != D3D_OK)
	{
		MessageBox(NULL, "Check if you are using integrated graphics.", NULL, MB_OK);

		return false;
	}

	// Set the technique to be used 
	if (FAILED(SpriteEffect->SetTechnique("Basic"))) return false;

	if (FAILED(g_pD3dDev->CreateVertexDeclaration(SpriteDeclElements, &SpriteDecl))) return false;

	if (SpriteGeometryBuffer == nullptr)
	{
		if (FAILED(g_pD3dDev->CreateVertexBuffer(sizeof(SPRITE_GEOMETRY) * 4, D3DUSAGE_WRITEONLY, D3DFVF_SPRITE_VERTEX, D3DPOOL_DEFAULT, &SpriteGeometryBuffer, nullptr)))
			return false;

		SPRITE_GEOMETRY* gbuffer;

		SpriteGeometryBuffer->Lock(0, 0, reinterpret_cast<void**>(&gbuffer), D3DLOCK_DISCARD);

		gbuffer[0] = { 0.0f, 1.0f, -1.0f, -1.0f };
		gbuffer[1] = { 0.0f, 0.0f, -1.0f, 1.0f };
		gbuffer[2] = { 1.0f, 1.0f, 1.0f, -1.0f };
		gbuffer[3] = { 1.0f, 0.0f, 1.0f, 1.0f };

		SpriteGeometryBuffer->Unlock();
	}
	if (SpriteIndexBuffer == nullptr)
	{
		if (FAILED(g_pD3dDev->CreateIndexBuffer(12, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &SpriteIndexBuffer, nullptr)))
			return false;

		WORD* ibuffer = nullptr;
		SpriteIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&ibuffer), D3DLOCK_DISCARD);


		ibuffer[0] = 0;
		ibuffer[1] = 1;
		ibuffer[2] = 2;
		ibuffer[3] = 2;
		ibuffer[4] = 1;
		ibuffer[5] = 3;

		SpriteIndexBuffer->Unlock();
	}

	return SpriteShaderReady = true;
}
bool InitializeTraceShader()
{
	static auto triedonce = false;

	if (triedonce) return TraceShaderReady;

	triedonce = true;

	LPD3DXBUFFER errors;

	//With this method you load your effect file
	//D3DXCreateEffectFromFile(g_pD3dDev, "basic_plane.hlsl", 0, 0, D3DXSHADER_DEBUG, 0, &TraceEffect, &errors);
	auto status = D3DXCreateEffect(g_pD3dDev, TraceShaderText, sizeof(TraceShaderText), NULL, NULL, /*D3DXSHADER_DEBUG*/ 0, NULL, &TraceEffect, &errors);

	if (errors)
	{
		MessageBox(NULL, LPSTR(errors->GetBufferPointer()), NULL, MB_OK);
		errors->Release();
		return false;
	}

	if (status != D3D_OK)
	{
		MessageBox(NULL, "Check if you are using integrated graphics.", NULL, MB_OK);

		return false;
	}

	// Set the technique to be used 
	if (FAILED(TraceEffect->SetTechnique("Basic"))) return false;

	if (FAILED(g_pD3dDev->CreateVertexDeclaration(TraceDeclElements, &TraceDecl))) return false;

	if (TraceGeometryBuffer == nullptr)
	{
		if (FAILED(g_pD3dDev->CreateVertexBuffer(sizeof(TRACE_GEOMETRY) * 24, D3DUSAGE_WRITEONLY, D3DFVF_SPRITE_VERTEX, D3DPOOL_DEFAULT, &TraceGeometryBuffer, nullptr)))
			return false;

		TRACE_GEOMETRY* gbuffer;

		TraceGeometryBuffer->Lock(0, 0, reinterpret_cast<void**>(&gbuffer), D3DLOCK_DISCARD);

		//				  (uv)		(x)	  (y) newpos olpos

		gbuffer[0] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[1] = { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		gbuffer[2] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[3] = { 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

		gbuffer[4] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[5] = { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		gbuffer[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[7] = { 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

		gbuffer[8] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[9] = { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		gbuffer[10] = { 0.0f, 0.0f, 0.5f, 0.8660f, 0.0f, 1.0f };
		gbuffer[11] = { 1.0f, 0.0f, 0.5f, 0.8660f, 1.0f, 0.0f };

		gbuffer[12] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[13] = { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		gbuffer[14] = { 0.0f, 0.0f, 0.5f, -0.8660f, 0.0f, 1.0f };
		gbuffer[15] = { 1.0f, 0.0f, 0.5f, -0.8660f, 1.0f, 0.0f };

		gbuffer[16] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[17] = { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		gbuffer[18] = { 0.0f, 0.0f, -0.5f, 0.8660f, 0.0f, 1.0f };
		gbuffer[19] = { 1.0f, 0.0f, -0.5f, 0.8660f, 1.0f, 0.0f };

		gbuffer[20] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		gbuffer[21] = { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		gbuffer[22] = { 0.0f, 0.0f, -0.5f, -0.8660f, 0.0f, 1.0f };
		gbuffer[23] = { 1.0f, 0.0f, -0.5f, -0.8660f, 1.0f, 0.0f };


		TraceGeometryBuffer->Unlock();
	}

	if (TraceIndexBuffer == nullptr)
	{
		if (FAILED(g_pD3dDev->CreateIndexBuffer(6 * 6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &TraceIndexBuffer, nullptr)))
			return false;

		WORD* ibuffer = nullptr;
		TraceIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&ibuffer), D3DLOCK_DISCARD);


		ibuffer[0] = 0;
		ibuffer[1] = 1;
		ibuffer[2] = 2;
		ibuffer[3] = 2;
		ibuffer[4] = 1;
		ibuffer[5] = 3;

		ibuffer[6] = 4;
		ibuffer[7] = 5;
		ibuffer[8] = 6;
		ibuffer[9] = 6;
		ibuffer[10] = 5;
		ibuffer[11] = 7;

		ibuffer[12] = 8;
		ibuffer[13] = 9;
		ibuffer[14] = 10;
		ibuffer[15] = 10;
		ibuffer[16] = 9;
		ibuffer[17] = 11;

		ibuffer[18] = 12;
		ibuffer[19] = 13;
		ibuffer[20] = 14;
		ibuffer[21] = 14;
		ibuffer[22] = 13;
		ibuffer[23] = 15;

		ibuffer[24] = 16;
		ibuffer[25] = 17;
		ibuffer[26] = 18;
		ibuffer[27] = 18;
		ibuffer[28] = 17;
		ibuffer[29] = 19;

		ibuffer[30] = 20;
		ibuffer[31] = 21;
		ibuffer[32] = 22;
		ibuffer[33] = 22;
		ibuffer[34] = 21;
		ibuffer[35] = 23;

		TraceIndexBuffer->Unlock();
	}

	return TraceShaderReady = true;
}


void CEffectRender::RenderAllSpriteParticles()
{
	// Initialize, if it fails, no particle drawing will be supported
	if (!InitializeSpriteShader()) return;

	auto billboardMatrix = g_pD3dApp->m_pCamera->GetBillboardMatrix();

	// Get view and projection matrices to pass them to the shader later
	D3DXMATRIX view, proj;
	g_pD3dDev->GetTransform(D3DTS_VIEW, &view);
	g_pD3dDev->GetTransform(D3DTS_PROJECTION, &proj);

	DWORD dwSrc, dwDest;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND, &dwDest);

	g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	for (auto& i : m_mapSpriteRenderInfo)
	{
		// Local variables containing all the information of this batch
		auto texture = i.first;
		const auto& inf = i.second;

		// Set up the instance buffer for this batch
		LPDIRECT3DVERTEXBUFFER9 instancebuffer = nullptr;
		if (FAILED(g_pD3dDev->CreateVertexBuffer(i.second.vecInstances.size() * sizeof(SPRITE_INSTANCE), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &instancebuffer, nullptr))) continue;

		// Copy data to instance buffer
		LPVOID ibuffer;
		instancebuffer->Lock(0, 0, &ibuffer, D3DLOCK_DISCARD);
		memcpy(ibuffer, inf.vecInstances.data(), i.second.vecInstances.size() * sizeof(SPRITE_INSTANCE));
		instancebuffer->Unlock();

		// Set up render state
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, inf.zWriteEnable);
		g_pD3dDev->SetRenderState(D3DRS_ZENABLE, inf.zBufferEnable);
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, inf.srcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, inf.dstBlend);

		// Set geometry data stream
		g_pD3dDev->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | inf.vecInstances.size()));
		g_pD3dDev->SetStreamSource(0, SpriteGeometryBuffer, 0, sizeof(SPRITE_GEOMETRY));

		// Set instance data stream
		g_pD3dDev->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		g_pD3dDev->SetStreamSource(1, instancebuffer, 0, sizeof(SPRITE_INSTANCE));

		g_pD3dDev->SetVertexDeclaration(SpriteDecl);

		g_pD3dDev->SetIndices(SpriteIndexBuffer);

		// Foreach pass, set the global variables and render the batch
		UINT passes = 0;
		SpriteEffect->Begin(&passes, 0);
		for (UINT pass = 0; pass < passes; pass++)
		{
			SpriteEffect->SetMatrix("View", &view);
			SpriteEffect->SetMatrix("Proj", &proj);
			SpriteEffect->SetMatrix("Billboard", &billboardMatrix);
			SpriteEffect->SetTexture("ModelTexture", texture);
			SpriteEffect->CommitChanges();
			SpriteEffect->BeginPass(pass);
			g_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
			SpriteEffect->EndPass();
		}
		SpriteEffect->End();

		// Don't forget to release the instances buffer
		instancebuffer->Release();
	}

	// Reset stream source frequencies
	g_pD3dDev->SetStreamSourceFreq(0, 1);
	g_pD3dDev->SetStreamSourceFreq(1, 1);

	// Reset render state
	g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, dwDest);
}
void CEffectRender::RenderAllTraceEffects()
{
	if (!InitializeTraceShader()) return;

	DWORD oldculling;
	g_pD3dDev->GetRenderState(D3DRS_CULLMODE, &oldculling);
	g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	g_pD3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	DWORD dwSrc, dwDest;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND, &dwDest);

	// Get view and projection matrices to pass them to the shader later
	D3DXMATRIX view, proj;
	g_pD3dDev->GetTransform(D3DTS_VIEW, &view);
	g_pD3dDev->GetTransform(D3DTS_PROJECTION, &proj);

	for (auto& i : m_mapTraceRenderInfo)
	{
		// Local variables containing all the information of this batch
		auto texture = i.first;
		const auto& inf = i.second;
		
		LPDIRECT3DVERTEXBUFFER9 instancebuffer = nullptr;

		// Set up the instance buffer for this batch
		if (FAILED(g_pD3dDev->CreateVertexBuffer(inf.vecInstances.size() * sizeof(TRACE_INSTANCE), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &instancebuffer, nullptr))) continue;

		LPVOID ibuffer;

		instancebuffer->Lock(0, 0, &ibuffer, D3DLOCK_DISCARD);

		memcpy(ibuffer, inf.vecInstances.data(), inf.vecInstances.size() * sizeof(TRACE_INSTANCE));

		instancebuffer->Unlock();

		// Set render state
		g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, inf.bAlphaEnable);
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, inf.zWriteEnable);
		g_pD3dDev->SetRenderState(D3DRS_ZENABLE, inf.zBufferEnable);
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, inf.srcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, inf.dstBlend);

		// Set geometry data stream
		g_pD3dDev->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | inf.vecInstances.size()));
		g_pD3dDev->SetStreamSource(0, TraceGeometryBuffer, 0, sizeof(TRACE_GEOMETRY));

		// Set instance data stream
		g_pD3dDev->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		g_pD3dDev->SetStreamSource(1, instancebuffer, 0, sizeof(TRACE_INSTANCE));

		g_pD3dDev->SetVertexDeclaration(TraceDecl);

		g_pD3dDev->SetIndices(TraceIndexBuffer);

		// Foreach pass, set the global variables and render the batch
		UINT passes = 0;
		TraceEffect->Begin(&passes, 0);
		for (UINT pass = 0; pass < passes; pass++)
		{
			TraceEffect->SetMatrix("View", &view);
			TraceEffect->SetMatrix("Proj", &proj);
			TraceEffect->SetTexture("ModelTexture", texture);
			TraceEffect->CommitChanges();
			TraceEffect->BeginPass(pass);
			g_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
			TraceEffect->EndPass();
		}
		TraceEffect->End();

		// Don't forget to release the instances buffer
		instancebuffer->Release();
	}

	// Reset stream source frequencies
	g_pD3dDev->SetStreamSourceFreq(0, 1);
	g_pD3dDev->SetStreamSourceFreq(1, 1);

	// Restore render state
	g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, dwDest);
	g_pD3dDev->SetRenderState(D3DRS_CULLMODE, oldculling);
}

SpriteRenderInfo::SpriteRenderInfo(CParticleSystem* system)
	: SpriteRenderInfo { system->m_bZWriteEnable, system->m_bZbufferEnable, system->m_dwSrcBlend, system->m_dwDestBlend }
{
	add(system);
}

void SpriteRenderInfo::add(CParticleSystem* system)
{
	// this part can be replaced with a memcpy

	for (auto& instance : system->m_vecParticleInstances)

		vecInstances.push_back(instance);
}

bool SpriteRenderInfo::matches(CParticleSystem* system) const
{
	return
		zWriteEnable == system->m_bZWriteEnable &&
		zBufferEnable == system->m_bZbufferEnable &&
		srcBlend == system->m_dwSrcBlend &&
		dstBlend == system->m_dwDestBlend;
}


TraceRenderInfo::TraceRenderInfo(CTraceAni* trace)
	: TraceRenderInfo{ trace->EnableAlphaBlend(), trace->m_bZWriteEnable, trace->m_bZbufferEnable, trace->m_dwSrcBlend, trace->m_dwDestBlend, trace->m_nTextureRenderState }
{
	add(trace);
}

void TraceRenderInfo::add(CTraceAni* trace)
{
	for (int i = 0; i < trace->m_nNumberOfTrace; i++)
	{
		auto eff = trace->m_pEffectPlane[i];

		if (eff != nullptr) vecInstances.push_back(eff->m_TraceInstance);
	}
}

bool TraceRenderInfo::matches(CTraceAni* trace) const
{
	return
		bAlphaEnable == trace->GetAlphaValue() &&
		zWriteEnable == trace->m_bZWriteEnable &&
		zBufferEnable == trace->m_bZbufferEnable &&
		srcBlend == trace->m_dwSrcBlend &&
		dstBlend == trace->m_dwDestBlend;
}


LPDIRECT3DTEXTURE9 CEffectRender::LoadTextureNew(char* strName)
{
	if (m_pTexEffectData == nullptr || *strName == '\0') return nullptr;

	int texnum = 0;

	for (const char* ptr = strName; *ptr != '\0'; ptr++)
	{
		if (*ptr >= '0' && *ptr <= '9') texnum = texnum * 10 + (*ptr - '0');
		else break;
	}

	return LoadTextureNew(texnum);
}

LPDIRECT3DTEXTURE9 CEffectRender::LoadTextureNew(unsigned texnum)
{
	if (m_pTexEffectData == nullptr) return nullptr;

	auto ptex = m_TextureManager.use(texnum);

	if (ptex != nullptr) return ptex;

	auto tex = m_pTexEffectData->find_by_number(texnum);

	if (tex == nullptr) return nullptr;

	LPDIRECT3DTEXTURE9 result;

	HRESULT hr = D3DXCreateTextureFromFileInMemory(g_pD3dDev, tex->m_pData, tex->m_DataSize, &result);

	if (FAILED(hr)) return nullptr;

	if (m_TextureManager.add(texnum, result)) return result;

	result->Release();

	result = nullptr;

	return nullptr;

}