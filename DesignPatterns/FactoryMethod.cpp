#include "FactoryMethod.h"

int main()
{
    CAbstractPizzaStore *nyStore = new CNYPizzaStore();
	CAbstractPizzaStore *chicagoStore = new CChicagoPizzaStore();
	CPizza *pizza = NULL;

	pizza = nyStore->orderPizza("cheese");
	cout << "Ethan ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = chicagoStore->orderPizza("cheese");
	cout << "Joel ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = nyStore->orderPizza("clam");
	cout << "Ethan ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = chicagoStore->orderPizza("clam");
	cout << "Joel ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = nyStore->orderPizza("pepperoni");
	cout << "Ethan ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = chicagoStore->orderPizza("pepperoni");
	cout << "Joel ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = nyStore->orderPizza("veggie");
	cout << "Ethan ordered a " + pizza->getName() << endl;
	delete pizza;

	pizza = chicagoStore->orderPizza("veggie");
	cout << "Joel ordered a " + pizza->getName() << endl;
	delete pizza;

	delete nyStore;
	delete chicagoStore;
    
    return 0;
}

