#pragma once
#include <vector>
//別クラスを監視するオブザーバパターンクラスファイル
class Observer {
private:
public:
	Observer() {};
	virtual ~Observer() {};
	virtual void OnNotify() {};
};

class Subject {
private:
	std::vector<Observer*> m_ObserverList;
public:
	void AddObsever(Observer* obsever_);
	void RemoveObserver(Observer* observer_);
protected:
	void Notify();
};