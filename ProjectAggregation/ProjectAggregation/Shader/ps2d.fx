#include	"psvscommon2.fx"

cbuffer ConstantBuffer2DColor : register(b6)
{
    float4 Color;
};
//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{

    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
    float4 OutColor = texcol + Color;
    return OutColor;
}