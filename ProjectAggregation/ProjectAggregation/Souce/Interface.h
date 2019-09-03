#pragma once
//独立したインターフェースクラスをまとめておくファイル

//被弾インターフェース
//何者かからの攻撃に対しての被弾時処理
class IHit {
public:
	virtual  ~IHit() {};
	virtual void HitFunction() = 0;
};