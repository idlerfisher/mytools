#pragma once
#include <iostream>
#include <list>

using namespace std;

class CAbstractObserver
{
public:
    virtual void update(float temp, float humidity, float pressure) = 0;
};

class CAbstractDisplayElement
{
public:
    virtual void display() = 0;
};

class CAbstractSubject
{
public:
    virtual void registerObserver(CAbstractObserver *pObserver) = 0;
    virtual void removeObserver(CAbstractObserver *pObserver) = 0;
    virtual void notifyObservers() = 0;
};

class CWeatherData : public CAbstractSubject
{
public:
    CWeatherData()
    {
    }
    virtual void registerObserver(CAbstractObserver *pObserver)
    {
        m_ObserverList.push_back(pObserver);
    }
    virtual void removeObserver(CAbstractObserver *pObserver)
    {
        m_ObserverList.remove(pObserver);
    }
    virtual void notifyObservers()
    {
        CAbstractObserver *pObserver;
        for (list<CAbstractObserver *>::iterator it = m_ObserverList.begin(); it != m_ObserverList.end(); ++it)
        {
            pObserver = *it;
            pObserver->update(m_fTemperature, m_fHumidity, m_fPressure);
        }
    }
    void measurementsChanged()
    {
        notifyObservers();
    }
    void setMeasurements(float temp, float humidity, float pressure)
    {
        m_fTemperature = temp;
        m_fHumidity = humidity;
        m_fPressure = pressure;
        measurementsChanged();
    }

private:
    list<CAbstractObserver *> m_ObserverList;
    float m_fTemperature;
    float m_fHumidity;
    float m_fPressure;
};

class CCurrentConditionsDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CCurrentConditionsDisplay(CAbstractSubject *pWeatherData)
    {
        m_pWeatherData = pWeatherData;
        m_pWeatherData->registerObserver(this);
    }
    virtual void update(float temp,float humidity,float pressure)
    {
        m_fTemperature = temp;
        m_fHumidity = humidity;
        display();
    }
    virtual void display()
    {
        cout << "Current conditions: " << m_fTemperature << "F degrees and " << m_fHumidity << "% humidity" << endl;
    }

private:
    float m_fTemperature;
    float m_fHumidity;
    CAbstractSubject *m_pWeatherData;
};

class CStatisticsDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CStatisticsDisplay(CWeatherData *pWeatherData)
    {
        m_fMaxTemp = 0.0f;
        m_fMinTemp = 200;
        m_fTempSum = 0.0f;
        m_iNumReadings = 0;
        pWeatherData->registerObserver(this);
    }
    virtual void update(float temp,float humidity,float pressure)
    {
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
    virtual void display()
    {
        cout << "Avg/Max/Min temperature = " << (m_fTempSum / m_iNumReadings) << '/' << m_fMaxTemp << '/' << m_fMinTemp << endl;
    }

private:
    float m_fMaxTemp;
    float m_fMinTemp;
    float m_fTempSum;
    int   m_iNumReadings;
};

class CForecastDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CForecastDisplay(CWeatherData *pWeatherData)
    {
        m_fCurrentPressure = 29.92f;
        pWeatherData->registerObserver(this);
    }
    virtual void update(float temp,float humidity,float pressure)
    {
        m_fLastPressure = m_fCurrentPressure;
        m_fCurrentPressure = pressure;

        display();
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
};

class CHeatIndexDisplay : public CAbstractObserver, public CAbstractDisplayElement
{
public:
    CHeatIndexDisplay(CWeatherData *pWeatherData)
    {
        m_fHeatIndex = 0.0f;
        pWeatherData->registerObserver(this);
    }
    virtual void update(float temp,float humidity,float pressure)
    {
        m_fHeatIndex = computerHeatIndex(temp, humidity);
        display();
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
};
