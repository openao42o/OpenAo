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