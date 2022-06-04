#include <string.h>
#include <EEPROM.h>
#include "radio_manager.h"
#include "radio_packet_handler.h"
#include "pair.h"
#include "reset.h"
#include "addresses.h"
#include "pins.h"
#include "chrono.h"


uint16_t thisNode = PAIRING_NODE;


RadioManager radioManager;


void HandleCommand(RadioPacket* radioPacket);
void OnPairButton();

Chrono chronoPIR(1000);

void setup()
{
  if (Pair::IsPaired())
  {
    thisNode = EEPROM.read(0);
  }

  radioManager = RadioManager(CE_PIN, CSN_PIN, thisNode);

  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(ACTIVITY_LED_PIN, OUTPUT);

  Serial.begin(9600);
  radioManager.Init();

}

void loop()
{
  delay(100);


  digitalWrite(ACTIVITY_LED_PIN, LOW);

  if (chronoPIR.HasElpased()) {

    if (digitalRead(PIR_SENSOR_PIN) == HIGH)
    {
      Serial.println("Triggered");
      radioManager.SendPacket(HUB_NODE, &RadioPacket(PayloadType::TRIGGERED));
      digitalWrite(ACTIVITY_LED_PIN, HIGH);
      delay(1000);
    }


  }



  if (digitalRead(PAIR_BUTTON_PIN) == HIGH)
  {
    OnPairButton();
    delay(1000);
  }


  RadioPacket radioPacket;

  if (radioManager.HasReceivedPacket(&radioPacket))
  {
    Serial.println((uint16_t)radioPacket.payload);
    HandleCommand(&radioPacket);
  }

  radioManager.Update();
}

void HandleCommand(RadioPacket* radioPacket)
{
  if (radioPacket->payload == PayloadType::PING)
  {
    Serial.println("Received ping");

    radioManager.SendPacket(HUB_NODE, &RadioPacket(PayloadType::PONG));
  }
}

void OnPairButton()
{
  if (Pair::IsPaired())
  {
    Pair::Reset();
    reset();
    return;
  }

  Pair pair(&radioManager);

  if (pair.Init())
  {
    Serial.println("Pairing successful");
    reset();
    return;
  }

  Serial.println("Pairing failed");
}