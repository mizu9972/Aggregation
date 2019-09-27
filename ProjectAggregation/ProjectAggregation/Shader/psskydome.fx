//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
Texture2D g_SubTex : register(t1); // �e�N�X�`��

#define BLINK_SPEED 0.5
//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{

    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
    float4 subtexcol = g_SubTex.Sample(g_SamplerLinear, input.Tex) * ((sin((iTime * BLINK_SPEED) % 314 / 100) + 1) / 2);
    subtexcol.r = subtexcol.g;//R�̒l��G�̒l�ŏ㏑��(�Ԃ̕����͓����ɁA���̕����͂��̂܂�)
    float4 col = texcol + subtexcol;

    return col;
}