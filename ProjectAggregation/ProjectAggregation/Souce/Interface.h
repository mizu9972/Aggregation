#pragma once
//�Ɨ������C���^�[�t�F�[�X�N���X���܂Ƃ߂Ă����t�@�C��

//��e�C���^�[�t�F�[�X
//���҂�����̍U���ɑ΂��Ă̔�e������
class IHit {
public:
	virtual  ~IHit() {};
	virtual void HitFunction() = 0;
};