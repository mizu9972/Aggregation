//--------------------------------------------------------------------------------------
// psNumber.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"

//--------------------------------------------------------------------------------------
//数字１桁を描画するピクセルシェーダー
// 
//デジタル時計などに使われる数字をイメージし、
//構成する７本の線を、数値によって描画するかしないかを切り替えて描画する
//--------------------------------------------------------------------------------------
//色を付ける部分に描画色が、つけない部分に(0,0,0)が返ってくる
float3 DrawLine(float2 StartPoint, float2 Thickness, float2 Position)
{
    float3 OutColor = float3(1.0, 1.0, 1.0); //描画色
    
    //線の太さ
    float2 Length;
    Length.x = StartPoint.x + Thickness.x;
    Length.y = StartPoint.y - Thickness.y;

    //線描画
    OutColor *= (max(StartPoint.x, Position.x) - StartPoint.x) / (Position.x - StartPoint.x)
    * (min(Length.x, Position.x) - Length.x) / (Position.x - Length.x)
    * (min(StartPoint.y, Position.y) - StartPoint.y) / (Position.y - StartPoint.y)
    * (max(Length.y, Position.y) - Length.y) / (Position.y - Length.y);

    return OutColor;
}

//数値によって描画する線を判別する
float3 JudgeLineByNumber(float Number,float2 Position)
{
    float3 OutColor;
    Number = floor(Number);

    //線ごとの描画判定
    float judge1 = min(abs(Number - 1.0), 1.0) * min(abs(Number - 4.0), 1.0); //Line1
    float judge2 = min((abs(Number - 2.0) - 1.0), 1.0); //Line2
    float judge3 = abs(min((max(Number, 4.0) - 4.0) * (abs(min(Number, 7.0) - 7.0)), 1.0) - 1.0); //Line3
    float judge4 = min((max(Number, 1.0) - 1.0) * min(abs(Number - 7.0), 1.0), 1.0); //Line4 ///
    float judge5 = abs(fmod(Number, 2.) - 1.0) * min(abs(Number - 4.0), 1.0); //Line5
    float judge6 = min(abs(Number - 2.0), 1.0); //Line6
    float judge7 = abs(fmod(Number, 3.) - 1.0); //Line7

    //線を描画
    OutColor = DrawLine(float2(0.40, 0.80), float2(0.25, 0.05), Position) * judge1; //1
    OutColor += DrawLine(float2(0.35, 0.75), float2(0.05, 0.25), Position) * judge2; //2
    OutColor += DrawLine(float2(0.65, 0.75), float2(0.05, 0.25), Position) * judge3; //3
    OutColor += DrawLine(float2(0.40, 0.50), float2(0.25, 0.05), Position) * judge4; //4
    OutColor += DrawLine(float2(0.35, 0.45), float2(0.05, 0.25), Position) * judge5; //5
    OutColor += DrawLine(float2(0.65, 0.45), float2(0.05, 0.25), Position) * judge6; //6
    OutColor += DrawLine(float2(0.40, 0.20), float2(0.25, 0.05), Position) * judge7; //7

    return OutColor;
}

float4 main(VS_OUTPUT input) : SV_Target
{
    float time = 0;
    float2 Position = input.Tex;
    //Position = 1.0 - Position;
    float3 texcol = Position.x;
    return float4(texcol,1.0);
}