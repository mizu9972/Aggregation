//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
Texture2D g_SubTex : register(t1); // テクスチャ

#define BLINK_SPEED 0.5
//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{

    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
    float4 subtexcol = g_SubTex.Sample(g_SamplerLinear, input.Tex) * ((sin((iTime * BLINK_SPEED) % 314 / 100) + 1) / 2);
    subtexcol.r = subtexcol.g;//Rの値をGの値で上書き(赤の部分は透明に、白の部分はそのまま)
    float4 col = texcol + subtexcol;

    return col;
}