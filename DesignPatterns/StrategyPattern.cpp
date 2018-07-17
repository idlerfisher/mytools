#include "StrategyPattern.h"

int main()
{
	CDuck *pMallard = new CMallardDuck();
	pMallard->performQuack();
	pMallard->performFly();
	delete pMallard;

	CDuck *pModel = new CModelDuck();
	pModel->performFly();
	pModel->setFlyBehavior(new CFlyRocketPowered());
	pModel->performFly();
	delete pModel;

	return 0;
}
