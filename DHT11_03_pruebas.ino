// 
//   FILE:  DHT11_02_pruebas
// PURPOSE: DHT11 library test sketch for Arduino
//          
//        case: ' ' read sensor
//        case: '1' switch relay
//

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
	return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//
double dewPoint(double celsius, double humidity)
{
	// (1) Saturation Vapor Pressure = ESGG(T)
	double RATIO = 373.15 / (273.15 + celsius);
	double RHS = -7.90298 * (RATIO - 1);
	RHS += 5.02808 * log10(RATIO);
	RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
	RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
	RHS += log10(1013.246);

        // factor -3 is to adjust units - Vapor Pressure SVP * humidity
	double VP = pow(10, RHS - 3) * humidity;

        // (2) DEWPOINT = F(Vapor Pressure)
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
	double Td = (b * temp) / (a - temp);
	return Td;
}


#include <dht11.h>

dht11 DHT11;

#define DHT11PIN  2
#define RELAY_PIN 3

void readTempSensor()
{
    int chk = DHT11.read(DHT11PIN);

    //Serial.print("Read sensor:\t");
    switch (chk)
    {
        case DHTLIB_OK: 
		Serial.print("OK\t"); 
		break;
        case DHTLIB_ERROR_CHECKSUM: 
		Serial.println("Checksum error"); 
		break;
        case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("Time out error"); 
		break;
        default: 
		Serial.println("Unknown error"); 
		break;
    }

    Serial.print("Humidity (%):\t");
    Serial.print((float)DHT11.humidity, 2);
    Serial.print("\t");
  
    Serial.print("Temperature (°C):\t");
    Serial.print((float)DHT11.temperature, 2);
    Serial.print("\t");
  
    Serial.print("Dew Point (°C):\t");
    Serial.print(dewPoint(DHT11.temperature, DHT11.humidity));
    Serial.print("\t");

    Serial.print("\n");
}  

void switchRelayOff()
{
    // turn LED off:
    digitalWrite(RELAY_PIN, LOW); 
    Serial.print ("OK\t");
    Serial.print ("Switch Relay OFF" );
    Serial.print ("\n");
  
}

void switchRelayOn()
{
    // turn LED off:
    digitalWrite(RELAY_PIN, HIGH); 
    Serial.print ("OK\t");
    Serial.print ("Switch Relay ON" );
    Serial.print ("\n");
  
}
  
  

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();

  pinMode(RELAY_PIN, OUTPUT);
}

void loop()
{
    static int relayVal = 0;
    int cmd;

    while (Serial.available() > 0)
    {
        cmd = Serial.read();

        switch (cmd)
        {
            case ' ':
            {
                readTempSensor();
                delay(2000);
                break;                
            }
            case '1':
            {
                switchRelayOff();
                break;
            }
            case '2':
            {
                switchRelayOn();
                break;
            }
        }
    }        
}
//
// END OF FILE
//

