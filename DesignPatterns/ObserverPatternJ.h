#pragma once
#include <iostream>
#include <list>

using namespace std;

class CAbstractObserver
{
public:
    virtual void update(CAbstractObservable *pObservable, void *arg) = 0;
};

class CAbstractDisplayElement
{
public:
    virtual void display() = 0;
};

class CAbstractObservable
{
public:
    virtual void addObserver(CAbstractObserver *pObserver) = 0;
    virtual void deleteObserver(CAbstractObserver *pObserver) = 0;
    virtual void notifyObservers() = 0;
};

class CWeatherData : public CAbstractObservable
{
public:
    CWeatherData()
    {
        m_bChanged = false;
    }
    virtual void addObserver(CAbstractObserver *pObserver)
    {
        m_ObserverList.push_back(pObserver);
    }
    virtual void deleteObserver(CAbstractObserver *pObserver)
    {
        m_ObserverList.remove(pObserver);
    }
    virtual void notifyObservers()
    {
        if (m_bChanged)
        {
            CAbstractObserver *pObserver;
            for (list<CAbstractObserver *>::iterator it = m_ObserverList.begin(); it != m_ObserverList.end(); ++it)
            {
                pObserver = *it;
                pObserver->update(m_fTemperature, m_fHumidity, m_fPressure);
            }
        }
        m_bChanged = false;
    }
    void measurementsChanged()
    {
        setChanged();
        notifyObservers();
    }
    void setMeasurements(float temp, float humidity, float pressure)
    {
        m_fTemperature = temp;
        m_fHumidity = humidity;
        m_fPressure = pressure;
        measurementsChanged();
    }
    float getTemperature()
    {
        return m_fTemperature;
    }
    float getHumidity()
    {
        return m_fHumidity;
    }
    float getPressure()
    {
        return m_fPressure;
    }
    void setChanged()
    {
        m_bChanged = true;
    }
    void clearChanged()
    {
        m_bChanged = false;
    }
    bool hasChanged()
    {
        return m_bChanged;
    }

private:
    list<CAbstractObserver *> m_ObserverList;
    float m_fTemperature;
    float m_fHumidity;
    float m_fPressure;
    bool  m_bChanged;
};

class CCurrentConditionsDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CCurrentConditionsDisplay(CAbstractObservable *pObservable)
    {
        m_pObservable = pObservable;
        m_pObservable->addObserver(this);
    }
    virtual void update(CAbstractObservable * pObservable,void * arg)
    {
        CWeatherData *pWeatherData = dynamic_cast<CWeatherData *>(pObservable);
        if (pWeatherData)
        {
            m_fTemperature = pWeatherData->getTemperature();
            m_fHumidity = pWeatherData->getHumidity();
            display();
        }
    }
    virtual void display()
    {
        cout << "Current conditions: " << m_fTemperature << "F degrees and " << m_fHumidity << "% humidity" << endl;
    }

private:
    float m_fTemperature;
    float m_fHumidity;
    CAbstractObservable *m_pObservable;
};

class CStatisticsDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CStatisticsDisplay(CWeatherData *pObservable)
    {
        m_fMaxTemp = 0.0f;
        m_fMinTemp = 200;
        m_fTempSum = 0.0f;
        m_iNumReadings = 0;
        m_pObservable = pObservable;
        m_pObservable->addObserver(this);
    }
    virtual void update(CAbstractObservable * pObservable,void * arg)
    {
        CWeatherData *pWeatherData = dynamic_cast<CWeatherData *>(pObservable);
        if (pWeatherData)
        {
            int temp = pWeatherData->getTemperature();
            
            m_fTempSum += temp;
            m_iNumReadings++;
            if (temp > m_fMaxTemp)
            {
                m_fMaxTemp = temp;
            }
            if (temp < m_fMinTemp)
            {
                m_fMinTemp = temp;
            }
            display();
        }
    }
    virtual void display()
    {
        cout << "Avg/Max/Min temperature = " << (m_fTempSum / m_iNumReadings) << '/' << m_fMaxTemp << '/' << m_fMinTemp << endl;
    }

private:
    float m_fMaxTemp;
    float m_fMinTemp;
    float m_fTempSum;
    int   m_iNumReadings;
    CAbstractObservable *m_pObservable;
};

class CForecastDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CForecastDisplay(CWeatherData *pObservable)
    {
        m_fCurrentPressure = 29.92f;
        m_pObservable = pObservable;
        m_pObservable->addObserver(this);
    }
    virtual void update(CAbstractObservable * pObservable,void * arg)
    {
        CWeatherData *pWeatherData = dynamic_cast<CWeatherData *>(pObservable);
        if (pWeatherData)
        {
            m_fLastPressure = m_fCurrentPressure;
            m_fCurrentPressure = pWeatherData->getPressure();
        }
    }
    virtual void display()
    {
        cout << "Forecast: ";
        if (m_fCurrentPressure > m_fLastPressure)
        {
            cout << "Improving weather on the way!" << endl;
        }
        else if (m_fCurrentPressure == m_fLastPressure)
        {
            cout << "More of the same" << endl;
        }
        else if (m_fCurrentPressure < m_fLastPressure)
        {
            cout << "Watch out for cooler, rainy weather" << endl;
        }
    }
    
private:
    float m_fCurrentPressure;
    float m_fLastPressure;
    CAbstractObservable *m_pObservable;
};

class CHeatIndexDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CHeatIndexDisplay(CWeatherData *pObservable)
    {
        m_fHeatIndex = 0.0f;
        m_pObservable = pObservable;
        m_pObservable->addObserver(this);
    }
    virtual void update(CAbstractObservable * pObservable,void * arg)
    {
        CWeatherData *pWeatherData = dynamic_cast<CWeatherData *>(pObservable);
        if (pWeatherData)
        {
            m_fHeatIndex = computerHeatIndex(pWeatherData->getTemperature(), pWeatherData->getHumidity());
            display();
        }
    }
    virtual void display()
    {
        cout << "Heat index is " << m_fHeatIndex << endl;
    }
    
private:
    float computerHeatIndex(float t, float rh)
    {
        float index = (float) ((16.923 + (0.185212 * t) + (5.37941 * rh)
				- (0.100254 * t * rh) + (0.00941695 * (t * t))
				+ (0.00728898 * (rh * rh)) + (0.000345372 * (t * t * rh))
				- (0.000814971 * (t * rh * rh))
				+ (0.0000102102 * (t * t * rh * rh))
				- (0.000038646 * (t * t * t)) + (0.0000291583 * (rh * rh * rh))
				+ (0.00000142721 * (t * t * t * rh))
				+ (0.000000197483 * (t * rh * rh * rh))
				- (0.0000000218429 * (t * t * t * rh * rh)) + 0.000000000843296 * (t
				* t * rh * rh * rh)) - (0.0000000000481975 * (t * t * t * rh
				* rh * rh)));
		return index;
    }

private:
    float m_fHeatIndex;
    CAbstractObservable *m_pObservable;
};
