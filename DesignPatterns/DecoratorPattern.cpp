#include "DecoratorPattern.h"

int main()
{
    list<CAbstractBeverage *> pointList;
    
    CAbstractBeverage *pBeverage = new CEspresso(); pointList.push_back(pBeverage);
    cout << pBeverage->getDescription() + " $" << pBeverage->cost() << endl;

    CAbstractBeverage *pBeverage2 = new CDarkRoast(); pointList.push_back(pBeverage2);
    pBeverage2 = new CMocha(pBeverage2); pointList.push_back(pBeverage2);
    pBeverage2 = new CMocha(pBeverage2); pointList.push_back(pBeverage2);
    pBeverage2 = new CWhip(pBeverage2); pointList.push_back(pBeverage2);
    cout << pBeverage2->getDescription() + " $" << pBeverage2->cost() << endl;

    CAbstractBeverage *pBeverage3 = new CHouseBlend(); pointList.push_back(pBeverage3);
    pBeverage3 = new CSoy(pBeverage3); pointList.push_back(pBeverage3);
    pBeverage3 = new CMocha(pBeverage3); pointList.push_back(pBeverage3);
    pBeverage3 = new CWhip(pBeverage3); pointList.push_back(pBeverage3);
    cout << pBeverage3->getDescription() + " $" << pBeverage3->cost() << endl;

    for (list<CAbstractBeverage *>::iterator it = pointList.begin(); it != pointList.end(); ++it)
    {
        delete *it;
    }

    return 0;
}
