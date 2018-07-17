#include "ObserverPattern.h"

int main()
{
    CWeatherData weatherData;
    
    CCurrentConditionsDisplay currentDisplay(&weatherData);
    CStatisticsDisplay statisticsDisplay(&weatherData);
    CForecastDisplay forecastDisplay(&weatherData);
    CHeatIndexDisplay heatIndexDisplay(&weatherData);

    weatherData.setMeasurements(80, 65, 30.4f);
    weatherData.setMeasurements(82, 70, 29.2f);
    weatherData.setMeasurements(78, 90, 29.2f);

    return 0;
}
