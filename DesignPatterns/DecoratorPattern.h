#pragma once
#include <iostream>
#include <list>

using namespace std;

class CAbstractBeverage
{
public:
    CAbstractBeverage()
    {
        m_strDescription = "Unknown Beverage";
    }
    virtual ~CAbstractBeverage()
    {
    }
    virtual string getDescription()
    {
        return m_strDescription;
    }
    virtual double cost() = 0;

    string m_strDescription;
};

class CAbstractCondimentDecorator : public CAbstractBeverage
{
};

class CEspresso : public CAbstractBeverage
{
public:
    CEspresso()
    {
        m_strDescription = "Espresso Coffee";
    }
    virtual double cost()
    {
        return 1.99;
    }
};

class CHouseBlend : public CAbstractBeverage
{
public:
    CHouseBlend()
    {
        m_strDescription = "House Blend Coffee";
    }
    virtual double cost()
    {
        return 0.89;
    }
};

class CDarkRoast : public CAbstractBeverage
{
public:
    CDarkRoast()
    {
        m_strDescription = "Dark Roast Coffee";
    }
    virtual double cost()
    {
        return 0.99;
    }
};

class CDecaf : public CAbstractBeverage
{
public:
    CDecaf()
    {
        m_strDescription = "Decaf Coffee";
    }
    virtual double cost()
    {
        return 0.99;
    }
};

class CMocha : public CAbstractCondimentDecorator
{
public:
    CMocha(CAbstractBeverage *pBeverage)
    {
        m_pBeverage = pBeverage;
    }
    virtual string getDescription()
    {
        return m_pBeverage->getDescription() + ", Mocha";
    }
    virtual double cost()
    {
        return 0.20 + m_pBeverage->cost();
    }
    CAbstractBeverage *m_pBeverage;
};

class CSoy : public CAbstractCondimentDecorator
{
public:
    CSoy(CAbstractBeverage *pBeverage)
    {
        m_pBeverage = pBeverage;
    }
    virtual string getDescription()
    {
        return m_pBeverage->getDescription() + ", Soy";
    }
    virtual double cost()
    {
        return 0.15 + m_pBeverage->cost();
    }
    CAbstractBeverage *m_pBeverage;
};

class CWhip : public CAbstractCondimentDecorator
{
public:
    CWhip(CAbstractBeverage *pBeverage)
    {
        m_pBeverage = pBeverage;
    }
    virtual string getDescription()
    {
        return m_pBeverage->getDescription() + ", Whip";
    }
    virtual double cost()
    {
        return 0.10 + m_pBeverage->cost();
    }
    CAbstractBeverage *m_pBeverage;
};
