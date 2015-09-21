#include "Wire.h"
#include <LiquidCrystal_I2C.h>
#include <TPIC6B595/TPIC6B595.h>
#include <IOHandler/LEDhandler.h>
#include <IOHandler/SWHandler.h>
#include <IOHandler/LED.h>
const int dataPin = 14;
const int dataClock = 15;
const int latchClock = 16;
const int dataClear = 17;

String command;
String txtMsg;

//extern bool LED1BlinkStatus;

TPIC6B595 TPIC1(dataPin, dataClock, latchClock, dataClear);
int IO;
LiquidCrystal_I2C lcd(0x20, 16, 2); // set the LCD address to 0x20 for a 16 chars and 2 line display
LED LED1(0x80  , &TPIC1, IO);
LED LED2(0x40 , &TPIC1, IO);
LED LED3(0x20 , &TPIC1, IO);
LED LED4(0x10 , &TPIC1, IO);
LED LEDGR(0x08 , &TPIC1, IO);
LED LEDAM(0x04, &TPIC1, IO);
LED LEDRE(0x02, &TPIC1, IO);

LED LEDBACKLIGHT(0x01, &TPIC1, IO);
bool readCommand();
void parseCommand();

int inputSatus = 0;
int oldSatus = inputSatus;

// LCD Handler
void LCDHandler(int LineNo, String txtMsg)
{

	switch (LineNo)
	{
	case 1:
		lcd.setCursor(0, 0);
		lcd.print("                    ");
		lcd.setCursor(0, 0);
		lcd.print(txtMsg);
		break;
	case 2:
		lcd.setCursor(0, 1);
		lcd.print("                    ");
		lcd.setCursor(0, 1);
		lcd.print(txtMsg);
		break;
	case 3:
		lcd.setCursor(0, 2);
		lcd.print("                    ");
		lcd.setCursor(0, 2);
		lcd.print(txtMsg);
		break;
	case 4:
		lcd.setCursor(0, 3);
		lcd.print("                    ");
		lcd.setCursor(0, 3);
		lcd.print(txtMsg);
		break;
	default:
		break;
	}

}

void setup()
{
	TPIC1.begin();
	TPIC1.write(0x00);

	Serial.begin(9600);
	Serial.println("Ready...");

	lcd.init();                      // initialize the lcd

	// Print a message to the LCD.
	lcd.begin(20, 4);
	//	BacklightON();
	LEDBACKLIGHT.ON();
	// print about and version message

	lcd.setCursor(0, 0);
	lcd.print("TOYOTA MOTOR THAILAND");

	lcd.setCursor(0, 1);
	lcd.print(" VOC-CO-CO2 MONITOR ");

	lcd.setCursor(0, 2);
	lcd.print("               ");

	lcd.setCursor(0, 3);
	lcd.print("V3.xx (c)2015 KMITL.");

	//	lcd.setCursor(0, 0);
	//	lcd.print("ID:021   ONLINE:Y");
	//
	//	lcd.setCursor(0, 1);
	//	lcd.print("T:03:52  D:12/07/15");
	//
	//	lcd.setCursor(0, 2);
	//	lcd.print("VOC:9999 CO:9999");
	//
	//	lcd.setCursor(0, 3);
	//	lcd.print("CO2:9999 LOGGER:Y");

	pinMode(SW1, INPUT);
	pinMode(SW2, INPUT);
	pinMode(SW3, INPUT);
	pinMode(SW4, INPUT);

	LED1.BlinkStatus = false;
	//	LED1.Blink(200, 800);
	//	delay(100);
	//	LED2.Blink(200, 2300);
	//	LEDGR.Blink(1000, 500);
	//	LEDAM.Blink(1000, 500);
	//	LEDRE.Blink(1000, 500);


}

bool sw_1;
bool sw_2;
bool sw_3;
bool sw_4;
void loop()
{
	// read command (main -> panel)
	if (readCommand())
	{
		parseCommand();
		//		Serial.println(command);
		command = "";
	}

	sw_1 = SW1Handler(false);
	sw_2 = SW2Handler(false);
	sw_3 = SW3Handler(false);
	sw_4 = SW4Handler(false);

	if (LED1.BlinkStatus == true)
		LED1.Update();
	if (LED2.BlinkStatus == true)
		LED2.Update();
	if (LED3.BlinkStatus == true)
		LED3.Update();
	if (LED4.BlinkStatus == true)
		LED4.Update();
	if (LEDGR.BlinkStatus == true)
		LEDGR.Update();
	if (LEDAM.BlinkStatus == true)
		LEDAM.Update();
	if (LEDRE.BlinkStatus == true)
		LEDRE.Update();


}

bool readCommand()
{
	char c;
	if (Serial.available() > 0)
	{
		c = Serial.read();
		if (c != '\n')
		{
			command += c;
			return false;
		}
		else
			return true;
	}
	return false;
}
int onTime;
int offTime;

void parseCommand()
{
	String commandU = command;
	commandU.toUpperCase();

	//  ILLU LED 1
	if ((commandU == "LED 1 ON"))
	{
		LED1.BlinkStatus = false;
		LED1.ON();
	}
	if (commandU == "LED 1 OFF")
	{
		LED1.BlinkStatus = false;
		LED1.OFF();
	}
	if (commandU.startsWith("LED 1 BLINK"))
	{
		sscanf(command.substring(12).c_str(), "%d %d", &onTime, &offTime);
		LED1.Blink(onTime, offTime);
	}
	if (commandU == "LED 1 STOP")
	{
		LED1.BlinkStatus = false;
		LED1.OFF();
	}

	//  ILLU LED 2
	if (commandU == "LED 2 ON")
	{
		LED2.BlinkStatus = false;
		LED2.ON();
	}
	if (commandU == "LED 2 OFF")
	{
		LED2.BlinkStatus = false;
		LED2.OFF();
	}
	if (commandU.startsWith("LED 2 BLINK"))
	{
		sscanf(command.substring(12).c_str(), "%d %d", &onTime, &offTime);
		LED2.Blink(onTime, offTime);
	}
	if (commandU == "LED 2 STOP")
	{
		LED2.BlinkStatus = false;
		LED2.OFF();
	}

	//  ILLU LED 3
	if (commandU == "LED 3 ON")
	{
		LED3.BlinkStatus = false;
		LED3.ON();
	}
	if (commandU == "LED 3 OFF")
	{
		LED3.BlinkStatus = false;
		LED3.OFF();
	}
	if (commandU.startsWith("LED 3 BLINK"))
	{
		sscanf(command.substring(12).c_str(), "%d %d", &onTime, &offTime);
		LED3.Blink(onTime, offTime);
	}
	if (commandU == "LED 3 STOP")
	{
		LED3.BlinkStatus = false;
		LED3.OFF();
	}

	//  ILLU LED 4
	if (commandU == "LED 4 ON")
	{
		LED4.BlinkStatus = false;
		LED4.ON();
	}
	if (commandU == "LED 4 OFF")
	{
		LED4.BlinkStatus = false;
		LED4.OFF();
	}
	if (commandU.startsWith("LED 4 BLINK"))
	{
		sscanf(command.substring(12).c_str(), "%d %d", &onTime, &offTime);
		LED4.Blink(onTime, offTime);
	}
	if (commandU == "LED 4 STOP")
	{
		LED4.BlinkStatus = false;
		LED4.OFF();
	}

	// TODO: add function LED GR, AM, RE

	// GREEN LED
	if (commandU == "LED GR ON")
	{
		LEDGR.BlinkStatus = false;
		LEDGR.ON();
	}
	if (commandU == "LED GR OFF")
	{
		LEDGR.BlinkStatus = false;
		LEDGR.OFF();
	}
	if (commandU.startsWith("LED GR BLINK"))
	{
		sscanf(command.substring(13).c_str(), "%d %d", &onTime, &offTime);
		Serial.println(onTime);
		Serial.println(offTime);
		LEDGR.Blink(onTime, offTime);
	}
	if (commandU == "LED GR STOP")
	{
		LEDGR.BlinkStatus = false;
		LEDGR.OFF();
	}

	// YELLOW/AMBER LED
	if (commandU == "LED AM ON")
	{
		LEDAM.BlinkStatus = false;
		LEDAM.ON();
	}
	if (commandU == "LED AM OFF")
	{
		LEDAM.BlinkStatus = false;
		LEDAM.OFF();
	}
	if (commandU.startsWith("LED AM BLINK"))
	{
		sscanf(command.substring(13).c_str(), "%d %d", &onTime, &offTime);
		LEDAM.Blink(onTime, offTime);
	}
	if (commandU == "LED AM STOP")
	{
		LEDAM.BlinkStatus = false;
		LEDAM.OFF();
	}

	// RED LED
	if (commandU == "LED RE ON")
	{
		LEDRE.BlinkStatus = false;
		LEDRE.ON();
	}
	if (commandU == "LED RE OFF")
	{
		LEDRE.BlinkStatus = false;
		LEDRE.OFF();
	}
	if (commandU.startsWith("LED RE BLINK"))
	{
		sscanf(command.substring(13).c_str(), "%d %d", &onTime, &offTime);
		LEDRE.Blink(onTime, offTime);
	}
	if (commandU == "LED RE STOP")
	{
		LEDRE.BlinkStatus = false;
		LEDRE.OFF();
	}

	// TODO: Add handler for LCD
	if (commandU.startsWith("LCD 1"))
	{
		LCDHandler(1, command.substring(6, 26));
	}
	if (commandU.startsWith("LCD 2"))
	{
		LCDHandler(2, command.substring(6, 26));
	}

	if (commandU.startsWith("LCD 3"))
	{
		LCDHandler(3, command.substring(6, 26));
	}
	if (commandU.startsWith("LCD 4"))
	{
		LCDHandler(4, command.substring(6, 26));
	}

	// TODO: Add handler for PUSH BUTTON
	if (commandU == "SW QUERY")
	{
		SWQueryHandler(0);
	}
	if (commandU == "SW QUERY 1")
	{
		SWQueryHandler(1);
	}
	if (commandU == "SW QUERY 2")
	{
		SWQueryHandler(2);
	}
	if (commandU == "SW QUERY 3")
	{
		SWQueryHandler(3);
	}
	if (commandU == "SW QUERY 4")
	{
		SWQueryHandler(4);
	}

	// LCD Back Light
	if ((commandU == "BACKLIGHT ON"))
	{
		LEDBACKLIGHT.ON();
	}
	if (commandU == "BACKLIGHT OFF")
	{
		LEDBACKLIGHT.OFF();
	}
}

