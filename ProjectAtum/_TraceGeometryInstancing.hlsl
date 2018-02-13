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
	
	float2 colors : TEXCOORD3;
	float3 newpos : TEXCOORD4;
	float3 oldpos : TEXCOORD5;
	float3 offh : TEXCOORD6;
	float3 offv : TEXCOORD7;
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
	
	float3 offset = (input.trans.x * input.offh) + (input.trans.y * input.offv);
	
	float3 position = (input.pos.x * input.newpos) + (input.pos.y * input.oldpos) + offset;

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