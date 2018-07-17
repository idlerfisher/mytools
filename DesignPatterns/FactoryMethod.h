#pragma once
#include <iostream>
#include <vector>

using namespace std;

typedef vector<string> StringVector;

class CPizza
{
public:
    string getName()
    {
        return m_strName;
    }
    virtual void prepare()
    {
        cout << "Preparing " + m_strName << endl;
		cout << "Tossing dough..." << endl;
		cout << "Adding sauce..." << endl;
		cout << "Adding toppings: " << endl;
		for (StringVector::iterator it = m_ToppingVec.begin(); it != m_ToppingVec.end(); ++it)
        {
            cout << "   " + *it;
        }
    }
    virtual void bake()
    {
        cout << "Bake for 25 minutes at 350" << endl;
    }
    virtual void cut()
    {
        cout << "Cutting the pizza into diagonal slices" << endl;
    }
    virtual void box()
    {
        cout << "Place pizza in official PizzaStore box" << endl;
    }
    string toString()
    {
        string display;
        display += "---- " + m_strName + " ----\n";
        display += m_strDough + "\n";
        display += m_strSauce + "\n";
        for (StringVector::iterator it = m_ToppingVec.begin(); it != m_ToppingVec.end(); ++it)
        {
            display += *it + "\n";
        }
        return display;
    }

protected:
    string m_strName;
    string m_strDough;
    string m_strSauce;
    StringVector m_ToppingVec;
};

class CAbstractPizzaStore
{
public:
    CPizza *orderPizza(const string &type)
    {
        CPizza *pPizza = createPizza(type);
        cout << "--- Making a " + pPizza->getName() + " ---" << endl;
        pPizza->prepare();
        pPizza->bake();
        pPizza->cut();
        pPizza->box();
        return pPizza;
    }
    virtual CPizza *createPizza(const string &type) = 0;
};

class CNYStyleCheesePizza : public CPizza
{
public:
	CNYStyleCheesePizza()
	{ 
		m_strName  = "NY Style Sauce and Cheese Pizza";
		m_strDough = "Thin Crust Dough";
		m_strSauce = "Marinara Sauce";
 
		m_ToppingVec.push_back("Grated Reggiano Cheese");
	}
};

class CNYStyleVeggiePizza : public CPizza
{
public:
	CNYStyleVeggiePizza()
	{
		m_strName  = "NY Style Veggie Pizza";
		m_strDough = "Thin Crust Dough";
		m_strSauce = "Marinara Sauce";
 
		m_ToppingVec.push_back("Grated Reggiano Cheese");
		m_ToppingVec.push_back("Garlic");
		m_ToppingVec.push_back("Onion");
		m_ToppingVec.push_back("Mushrooms");
		m_ToppingVec.push_back("Red Pepper");
	}
};

class CNYStyleClamPizza : public CPizza
{
public:
	CNYStyleClamPizza()
	{
		m_strName  = "NY Style Clam Pizza";
		m_strDough = "Thin Crust Dough";
		m_strSauce = "Marinara Sauce";
 
		m_ToppingVec.push_back("Grated Reggiano Cheese");
		m_ToppingVec.push_back("Fresh Clams from Long Island Sound");
	}
};

class CNYStylePepperoniPizza : public CPizza
{
public:
	CNYStylePepperoniPizza()
	{
		m_strName  = "NY Style Pepperoni Pizza";
		m_strDough = "Thin Crust Dough";
		m_strSauce = "Marinara Sauce";
 
		m_ToppingVec.push_back("Grated Reggiano Cheese");
		m_ToppingVec.push_back("Sliced Pepperoni");
		m_ToppingVec.push_back("Garlic");
		m_ToppingVec.push_back("Onion");
		m_ToppingVec.push_back("Mushrooms");
		m_ToppingVec.push_back("Red Pepper");
	}
};

class CChicagoStyleCheesePizza : public CPizza
{
public:
	CChicagoStyleCheesePizza()
	{
		m_strName  = "Chicago Style Deep Dish Cheese Pizza";
		m_strDough = "Extra Thick Crust Dough";
		m_strSauce = "Plum Tomato Sauce";
 
		m_ToppingVec.push_back("Shredded Mozzarella Cheese");
	}
 
	void cut()
	{
		cout << "Cutting the pizza into square slices" << endl;
	}
};

class CChicagoStyleVeggiePizza : public CPizza
{
public:
    CChicagoStyleVeggiePizza()
    {
		m_strName  = "Chicago Deep Dish Veggie Pizza";
		m_strDough = "Extra Thick Crust Dough";
		m_strSauce = "Plum Tomato Sauce";
 
		m_ToppingVec.push_back("Shredded Mozzarella Cheese");
		m_ToppingVec.push_back("Black Olives");
		m_ToppingVec.push_back("Spinach");
		m_ToppingVec.push_back("Eggplant");
	}
 
	void cut()
	{
		cout << "Cutting the pizza into square slices" << endl;
	}
};

class CChicagoStyleClamPizza : public CPizza
{
public:
	CChicagoStyleClamPizza()
	{
		m_strName  = "Chicago Style Clam Pizza";
		m_strDough = "Extra Thick Crust Dough";
		m_strSauce = "Plum Tomato Sauce";
 
		m_ToppingVec.push_back("Shredded Mozzarella Cheese");
		m_ToppingVec.push_back("Frozen Clams from Chesapeake Bay");
	}
 
	void cut()
	{
		cout << "Cutting the pizza into square slices" << endl;
	}
};

class CChicagoStylePepperoniPizza : public CPizza
{
public:
	CChicagoStylePepperoniPizza()
	{
		m_strName  = "Chicago Style Pepperoni Pizza";
		m_strDough = "Extra Thick Crust Dough";
		m_strSauce = "Plum Tomato Sauce";
 
		m_ToppingVec.push_back("Shredded Mozzarella Cheese");
		m_ToppingVec.push_back("Black Olives");
		m_ToppingVec.push_back("Spinach");
		m_ToppingVec.push_back("Eggplant");
		m_ToppingVec.push_back("Sliced Pepperoni");
	}
 
	void cut()
	{
		cout << "Cutting the pizza into square slices" << endl;
	}
};

class CNYPizzaStore : public CAbstractPizzaStore
{
public:
    virtual CPizza *createPizza(const string &type)
    {
        CPizza *pPizza = NULL;
        
		if (type == "cheese")
		{
			pPizza = new CNYStyleCheesePizza();
		}
		else if (type == "veggie")
		{
			pPizza = new CNYStyleVeggiePizza();
		}
		else if (type == "clam")
		{
			pPizza = new CNYStyleClamPizza();
		}
		else if (type == "pepperoni")
		{
			pPizza = new CNYStylePepperoniPizza();
		}
		
		return pPizza;
	}
};

class CChicagoPizzaStore : public CAbstractPizzaStore
{
public:
    virtual CPizza *createPizza(const string &type)
    {
        CPizza *pPizza = NULL;
        
    	if (type == "cheese")
    	{
        	pPizza = new CChicagoStyleCheesePizza();
    	}
    	else if (type == "veggie")
    	{
    	    pPizza = new CChicagoStyleVeggiePizza();
    	}
    	else if (type == "clam")
    	{
    	    pPizza = new CChicagoStyleClamPizza();
    	}
    	else if (type == "pepperoni")
    	{
        	pPizza = new CChicagoStylePepperoniPizza();
    	}

    	return pPizza;
	}
};


