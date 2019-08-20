#pragma once

class CEffectiveObject {
private:
public:
	CEffectiveObject() {};
	virtual ~CEffectiveObject() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

class CFeedIn : public CEffectiveObject{
private:
	CFeedIn() {};
	virtual void Init();
	virtual void Update();
	virtual void Draw();
};