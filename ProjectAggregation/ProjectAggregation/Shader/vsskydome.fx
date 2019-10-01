//--------------------------------------------------------------------------------------
// vs.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
Texture2D g_SubTex : register(t1); // テクスチャ

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos		:	POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

    float4 subtex = g_Tex.Sample(g_SamplerLinear, output.Tex);
    subtex = mul(subtex, 0.5);
    output.Tex = Tex;

	float4 N = Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, World);
	N = normalize(N);

	output.Normal = N;

	return output;
}