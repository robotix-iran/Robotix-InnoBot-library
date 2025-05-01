#define TINY_GSM_MODEM_M66


// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

/*
 * Tests enabled
 */
#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_CALL true
#define TINY_GSM_TEST_SMS true
#define TINY_GSM_TEST_BATTERY true

// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET "+989000000000"
#define CALL_TARGET "+989000000000"

// Your GPRS credentials, if any
const char apn[] = "MCINET";
// const char apn[] = "ibasis.iot";
const char gprsUser[] = "";
const char gprsPass[] = "";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

bool res;

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  SerialAT.println("ATE0");
  DBG("Wait...");
  delay(1000);
}

void loop() {


  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);

  DBG("Waiting for network...");
  if (!modem.waitForNetwork(200000L, true)) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) { DBG("Network connected"); }

#if TINY_GSM_TEST_GPRS
  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
  }

  res = modem.isGprsConnected();
  DBG("GPRS status:", res ? "connected" : "not connected");

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String imsi = modem.getIMSI();
  DBG("IMSI:", imsi);

  String cop = modem.getOperator();
  DBG("Operator:", cop);

  IPAddress local = modem.localIP();
  DBG("Local IP:", local);

  int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);
#endif

#if TINY_GSM_TEST_CALL && defined TINY_GSM_MODEM_HAS_CALLING && defined CALL_TARGET
  DBG("Calling:", CALL_TARGET);

  // This is NOT supported on M590
  res = modem.callNumber(CALL_TARGET);
  DBG("Call:", res ? "OK" : "fail");

  if (res) {
    delay(1000L);

    // Play DTMF A, duration 1000ms
    modem.dtmfSend('A', 1000);

    // Play DTMF 0..4, default duration (100ms)
    for (char tone = '0'; tone <= '4'; tone++) { modem.dtmfSend(tone); }

    delay(8000);

    res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail");
  }
#endif

#if TINY_GSM_TEST_SMS && defined TINY_GSM_MODEM_HAS_SMS && defined SMS_TARGET
  res = modem.sendSMS(SMS_TARGET, String("Hello World"));
  DBG("SMS:", res ? "OK" : "fail");

#endif

#if TINY_GSM_TEST_BATTERY && defined TINY_GSM_MODEM_HAS_BATTERY
  int8_t chargeState = -99;
  int8_t percent = -99;
  int16_t milliVolts = -9999;
  modem.getBattStats(chargeState, percent, milliVolts);
  DBG("Battery charge state:", chargeState);
  DBG("Battery charge 'percent':", percent);
  DBG("Battery voltage:", milliVolts / 1000.0F);
#endif

  DBG("End of tests.");

  // Do nothing forevermore
  while (true) { modem.maintain(); }
}