#pragma once

/*************
 * 第一章: 策略模式
 ************/

#include <iostream>

using namespace std;

//fly action
class CAbstractFlyBehavior
{
public:
	CAbstractFlyBehavior() {}
	virtual ~CAbstractFlyBehavior() {}
	virtual void fly() = 0;
};

class CFlyWithWings : public CAbstractFlyBehavior
{
public:
	virtual void fly()
	{
		cout << "I'm flying." << endl;
	}
};

class CFlyNoWay : public CAbstractFlyBehavior
{
public:
	virtual void fly()
	{
		cout << "I can't fly." << endl;
	}
};

class CFlyRocketPowered : public CAbstractFlyBehavior
{
public:
	virtual void fly()
	{
		cout << "I'm flying with a rocket." << endl;
	}
};

//quack action
class CAbstractQuackBehavior
{
public:
	CAbstractQuackBehavior() {}
	virtual ~CAbstractQuackBehavior() {}
	virtual void quack() = 0;
};

class CQuack : public CAbstractQuackBehavior
{
public:
	virtual void quack()
	{
		cout << "Quack" << endl;
	}
};

class CSqueak : public CAbstractQuackBehavior
{
public:
	virtual void quack()
	{
		cout << "Squeak" << endl;
	}
};

class CMuteQuack : public CAbstractQuackBehavior
{
public:
	virtual void quack()
	{
		cout << "Silence" << endl;
	}
};

//duck
class CDuck
{
public:
	CDuck()
	{
		m_pFlyBehavior   = NULL;
		m_pQuackBehavior = NULL;
	}
	virtual ~CDuck()
	{
	}
	virtual void display()
	{
	}
	void performFly()
	{
		if (m_pFlyBehavior)
		{
			m_pFlyBehavior->fly();
		}
	}
	void performQuack()
	{
		if (m_pQuackBehavior)
		{
			m_pQuackBehavior->quack();
		}
	}
	void swim()
	{
		cout << "All ducks float, even decoys." << endl;
	}
	void setFlyBehavior(CAbstractFlyBehavior *pFlyBehavior)
	{
		if (m_pFlyBehavior && m_pFlyBehavior != pFlyBehavior)
		{
			delete m_pFlyBehavior;
		}
		m_pFlyBehavior = pFlyBehavior;
	}
	void setQuackBehavior(CAbstractQuackBehavior *pQuackBehavior)
	{
		if (m_pQuackBehavior && m_pQuackBehavior != pQuackBehavior)
		{
			delete m_pQuackBehavior;
		}
		m_pQuackBehavior = pQuackBehavior;
	}

	CAbstractFlyBehavior   *m_pFlyBehavior;
	CAbstractQuackBehavior *m_pQuackBehavior;
};

class CMallardDuck : public CDuck
{
public:
	CMallardDuck()
	{
		m_pFlyBehavior   = new CFlyWithWings();
		m_pQuackBehavior = new CQuack();
	}
	~CMallardDuck()
	{
		if (m_pFlyBehavior)
		{
			delete m_pFlyBehavior;
		}
		if (m_pQuackBehavior)
		{
			delete m_pQuackBehavior;
		}
	}
	virtual void display()
	{
		cout << "I'm a real Mallard duck." << endl;
	}
};

class CModelDuck : public CDuck
{
public:
	CModelDuck()
	{
		m_pFlyBehavior   = new CFlyNoWay();
		m_pQuackBehavior = new CQuack();
	}
	virtual void display()
	{
		cout << "I'm a model duck." << endl;
	}
};
