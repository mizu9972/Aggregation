//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
Texture2D g_SubTex : register(t1); // �e�N�X�`��

#define BLINK_SPEED   2.5 //�_�ő��x�␳
#define STAR_NUM      50  //���̐��̐�
#define ACTIVE_STAR   8   //�`�悵�Ȃ�����
#define STAR_PATTERN  20  //�_�ł̐F�p�^�[��

//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------

//�����_����float�̒l��Ԃ�
float fRandom(float x)
{
    return frac(sin(x) * 1000.0);
}

//�X�J�C�h�[���̃s�N�Z���V�F�[�_�[
//���f���̐F�̏�ɏd�˂ĕ`�悷��摜�������_���ɓ_�ł�����
float4 main(VS_OUTPUT input) : SV_Target
{
    //�X�J�C�h�[���̐F���o��
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);  

    float2 CustomCoord = input.Tex * STAR_NUM; //���̐��ŕ���

    float2 fracCoord  = frac(CustomCoord);  //�����������o��
    float2 floorCoord = floor(CustomCoord); //�����������o��
    floorCoord = fRandom(floorCoord.x * 10 + floorCoord.y) * 10;//�����_���ɂ΂炯������

    //���W���̐F����萔0�ɌŒ肵�S�̂�0~1�͈̔͂Ő��K��
    float Color = max(fmod(floorCoord, STAR_PATTERN), ACTIVE_STAR);
    Color = (Color - ACTIVE_STAR) / (STAR_PATTERN - 1 - ACTIVE_STAR);

    //�����g�𗘗p���ē_�ł�����
    float4 subtexcol = g_SubTex.Sample(g_SamplerLinear, fracCoord)
    * ((sin((iTime * BLINK_SPEED * Color) % 314 / 100) + 1) / 2) * Color;

    subtexcol.r = subtexcol.g;//R�̒l��G�̒l�ŏ㏑��(�Ԃ̕����͓����ɁA���̕����͂��̂܂�)
    float4 col = texcol + subtexcol;//�X�J�C�h�[���̐F�Ɛ��̐F������

    return col;
}