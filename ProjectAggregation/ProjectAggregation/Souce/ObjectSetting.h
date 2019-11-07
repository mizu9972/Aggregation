#pragma once

//キャラクターの設定

//開始時に出現するエネミーの数
//変更する場合GameSceneクラスのInit内のエネミー初期位置設定する部分も追加・削除する
#define ENEMY_START_NUM 5

#define PLAYER_CAMERAPOS_Z -3.25f

#define LOOKAT_LIMIT_ANGLE 50 //視点移動の限界角度

#define CHARACTER_HP 5 //体力
#define CHARACTER_SPEED 0.5f //初期移動速度
#define CHARACTER_MAXSPEED 10.0f //最大速度

#define SITE_SIZE 50 //標準サイトのサイズ
//自動操縦
#define RANDOM_MOVE_CONTINUE_TIME_MIN 1 //同じ行動をとる最小時間
#define RANDOM_MOVE_CONTINUE_TIME_MAX 5 //同じ行動をとる最大時間