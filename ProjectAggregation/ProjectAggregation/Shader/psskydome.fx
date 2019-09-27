//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
Texture2D g_SubTex : register(t1); // テクスチャ


//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex) + g_SubTex.Sample(g_SamplerLinear, input.Tex) * ((sin(iTime) + 1) / 2);
	float4 col = texcol + ColorMaterial;

    return col;
}