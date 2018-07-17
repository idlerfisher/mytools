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
    void prepare()
    {
        cout << "Preparing " + m_strName << endl;
    }
    void bake()
    {
        cout << "Baking " + m_strName << endl;
    }
    void cut()
    {
        cout << "Cutting " + m_strName << endl;
    }
    void box()
    {
        cout << "Boxing " + m_strName << endl;
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

class CCheesePizza : public CPizza
{
public:
    CCheesePizza()
    {
        m_strName  = "Cheese Pizza";
        m_strDough = "Regular Crust";
        m_strSauce = "Marinara Pizza Sauce";
        m_ToppingVec.push_back("Fresh Mozzarella");
        m_ToppingVec.push_back("Parmesan");
    }
};

class CPepperoniPizza : public CPizza
{
public:
    CPepperoniPizza()
    {
        m_strName  = "Pepperoni Pizza";
        m_strDough = "Crust";
        m_strSauce = "Marinara sauce";
        m_ToppingVec.push_back("Sliced Pepperoni");
        m_ToppingVec.push_back("Sliced Onion");
        m_ToppingVec.push_back("Grated parmesan cheese");
    }
};

class CClamPizza : public CPizza
{
public:
    CClamPizza()
    {
        m_strName  = "Clam Pizza";
        m_strDough = "Thin crust";
        m_strSauce = "White garlic sauce";
        m_ToppingVec.push_back("Clams");
        m_ToppingVec.push_back("Grated parmesan cheese");
    }
};

class CVeggiePizza : public CPizza
{
public:
    CVeggiePizza()
    {
        m_strName = "Veggie Pizza";
		m_strDough = "Crust";
		m_strSauce = "Marinara sauce";
		m_ToppingVec.push_back("Shredded mozzarella");
		m_ToppingVec.push_back("Grated parmesan");
		m_ToppingVec.push_back("Diced onion");
		m_ToppingVec.push_back("Sliced mushrooms");
		m_ToppingVec.push_back("Sliced red pepper");
		m_ToppingVec.push_back("Sliced black olives");
    }
};

class CSimplePizzaFactory
{
public:
    CPizza *createPizza(const string &type)
    {
        CPizza *pPizza = NULL;
        
        if (type == "cheese")
        {
            pPizza = new CCheesePizza();
        }
        else if (type == "pepperoni")
        {
            pPizza = new CPepperoniPizza();
        }
        else if (type == "clam")
        {
            pPizza = new CClamPizza();
        }
        else if (type == "veggie")
        {
            pPizza = new CVeggiePizza();
        }
        
        return pPizza;
    }
};

class CPizzaStore
{
public:
    CPizzaStore(CSimplePizzaFactory *pFactory)
    {
        m_pFactory = pFactory;
    }
    CPizza *orderPizza(const string &type)
    {
        CPizza *pPizza = m_pFactory->createPizza(type);
        
        pPizza->prepare();
        pPizza->bake();
        pPizza->cut();
        pPizza->box();

        return pPizza;
    }

private:
    CSimplePizzaFactory *m_pFactory;
};

