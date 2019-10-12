//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
Texture2D g_SubTex : register(t1); // テクスチャ

#define BLINK_SPEED   2.5 //点滅速度補正
#define STAR_NUM      50  //一列の星の数
#define ACTIVE_STAR   8   //描画しない割合
#define STAR_PATTERN  20  //点滅の色パターン

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------

//ランダムなfloatの値を返す
float fRandom(float x)
{
    return frac(sin(x) * 1000.0);
}

//スカイドームのピクセルシェーダー
//モデルの色の上に重ねて描画する画像をランダムに点滅させる
float4 main(VS_OUTPUT input) : SV_Target
{
    //スカイドームの色取り出し
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);  

    float2 CustomCoord = input.Tex * STAR_NUM; //星の数で分解

    float2 fracCoord  = frac(CustomCoord);  //小数部分取り出し
    float2 floorCoord = floor(CustomCoord); //整数部分取り出し
    floorCoord = fRandom(floorCoord.x * 10 + floorCoord.y) * 10;//ランダムにばらけさせる

    //座標毎の色を一定数0に固定しつつ全体を0~1の範囲で正規化
    float Color = max(fmod(floorCoord, STAR_PATTERN), ACTIVE_STAR);
    Color = (Color - ACTIVE_STAR) / (STAR_PATTERN - 1 - ACTIVE_STAR);

    //正弦波を利用して点滅させる
    float4 subtexcol = g_SubTex.Sample(g_SamplerLinear, fracCoord)
    * ((sin((iTime * BLINK_SPEED * Color) % 314 / 100) + 1) / 2) * Color;

    subtexcol.r = subtexcol.g;//Rの値をGの値で上書き(赤の部分は透明に、白の部分はそのまま)
    float4 col = texcol + subtexcol;//スカイドームの色と星の色を合成

    return col;
}