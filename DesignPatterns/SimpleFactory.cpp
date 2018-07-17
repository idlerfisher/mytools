#include "SimpleFactory.h"

int main()
{
    CSimplePizzaFactory factory;
    CPizzaStore store(&factory);

    CPizza *pPizza = store.orderPizza("cheese");
    cout << "We ordered a " + pPizza->getName() << endl;
    delete pPizza;

    pPizza = store.orderPizza("veggie");
    cout << "We ordered a " + pPizza->getName() << endl;
    delete pPizza;
    
    return 0;
}

