
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Credentials.h>

#define DEBUG 1
int servoPin = D3;

const char *mqtt_server = "192.168.1.9";
const char *topic = "pc"; // this is the [root topic]
String pubTopic;

long timeBetweenMessages = 1000 * 5 * 1;
long timeBetweenReconnectAttempts = 1000 * 5;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
long lastReconnectAttempt = 0;
long cntReconnectAttempt = 0;
String clientId = "fishfeeder";
int status = WL_IDLE_STATUS; // the starting Wifi radio's status

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PW);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

#define MAX_QUEUE_ITEMS 8
int feedIndex = 0;
int insertIndex = 0;
int numFeedItems = 0;
int feedQueue[MAX_QUEUE_ITEMS];
boolean stateFeeding = false;
int pulseCount = 0;
long totalFeedCount = 0;
long pinStartMicros = 0;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  for (unsigned int i = 0; i < length; i++)
  {
    //Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println(msg);

  // Switch on the LED if an 1 was received as first character
  if (strcmp(topic, "pc/fishfeeder/in/feed") == 0)
  {
    if (msg.indexOf("{value:") != -1)
    {
      int value = atoi(msg.substring(msg.indexOf(":") + 1, msg.indexOf("}")).c_str());
      if (numFeedItems < MAX_QUEUE_ITEMS)
      {
        Serial.print("Feeding! numFeedItems = ");
        Serial.println(numFeedItems);

        // x8 to translate ml to pulses (depending on the feeding screw dimension)
        feedQueue[insertIndex] = value * 8;
        numFeedItems++;
        insertIndex++;
        if (insertIndex >= MAX_QUEUE_ITEMS)
        {
          insertIndex = 0;
        }
        Serial.print("Feeding!");
      }
      else
      {
        Serial.println("Max Queue reached!!!!");
      }
    }
    else
    {
      Serial.println("Unknown sub command");
    }
  }
  else if (strcmp(topic, "pc/fishfeeder/in/ack_request") == 0)
  {
    if (msg.indexOf("{value:") != -1)
    {
      int value = atoi(msg.substring(msg.indexOf(":") + 1, msg.indexOf("}")).c_str());
      String topic = pubTopic;
      topic += "/ack";
      String valStr = "{\"value\":";
      valStr += value;
      valStr += "}";
      client.publish((char *)topic.c_str(), (char *)valStr.c_str(), true);
    }
    else
    {
      Serial.println("Unknown sub command");
    }
  }
  else
  {
    Serial.print("Unknown topic: ");
    Serial.println(topic);
  }
}

void reconnect()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    setup_wifi();
  }

  Serial.print("Attempting MQTT connection...");

  // Attempt to connect
  if (client.connect(clientId.c_str()))
  {
    Serial.println("connected");
    // Once connected, publish an announcement...
    client.publish(topic, ("connected " + clientId).c_str(), true);
    // ... and resubscribe
    // topic + clientID + in
    String subscription;
    subscription += topic;
    subscription += "/";
    subscription += clientId;
    subscription += "/in/#";
    client.subscribe(subscription.c_str());
    Serial.print("subscribed to : ");
    Serial.println(subscription);
  }
  else
  {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.print(" wifi=");
    Serial.print(WiFi.status());
    Serial.println(" try again in 5 seconds");
  }
}

void setup()
{
  pinMode(servoPin, OUTPUT);
  pubTopic += topic;
  pubTopic += "/";
  pubTopic += clientId;
  pubTopic += "/out";

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
int toggleCnt = 0;

boolean commandIsOngoing = false;
long lastKnownSwitchStateTime = millis();

//////////////////////////////////////////////////////////////////////////////////////////
// feed
/////////////////////////////////////////////////////////////////////////////////////////
void handle_feeding(long now)
{

  if (stateFeeding)
  {
    if (digitalRead(servoPin) == LOW)
    {
      if (micros() - pinStartMicros > 20000)
      {
        totalFeedCount++;
        pulseCount++;
        // Limit to max 100ml (800 pulses) at one time
        if ((pulseCount > feedQueue[feedIndex]) || (pulseCount > 800))
        {
          stateFeeding = false;
          feedIndex++;
          if (feedIndex >= MAX_QUEUE_ITEMS)
          {
            feedIndex = 0;
          }
          numFeedItems--;
          Serial.println("Feeding ready");
          String topic = pubTopic;
          topic += "/feeding_amount";
          String valStr = "{\"value\":";
          valStr += (totalFeedCount / 8);
          valStr += "}";
          client.publish((char *)topic.c_str(), (char *)valStr.c_str(), true);
#ifdef DEBUG
          Serial.print("Publish message: ");
          Serial.println(valStr);
#endif
        }
        else
        {
          digitalWrite(servoPin, HIGH);
          pinStartMicros = micros();
        }
      }
    }
    else
    {
      if (micros() - pinStartMicros > 2000)
      {
        digitalWrite(servoPin, LOW);
        pinStartMicros = micros();
      }
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
// loop
/////////////////////////////////////////////////////////////////////////////////////////
void loop()
{

  long now = millis();

  if (stateFeeding)
  {
    handle_feeding(now);
  }
  else if (numFeedItems > 0)
  {
    stateFeeding = true;
    pulseCount = 0;
    digitalWrite(servoPin, LOW);
    pinStartMicros = micros();
  }

  // confirm still connected to mqtt server
  if (!client.connected())
  {
    if (now - lastReconnectAttempt > timeBetweenReconnectAttempts)
    {
      lastReconnectAttempt = now;
      reconnect();
      cntReconnectAttempt++;
    }
  }
  if (client.connected())
  {
    client.loop();
  }

  if (now - lastMsg > timeBetweenMessages)
  {
    lastMsg = now;

    // print the received signal strength:
    long rssi = WiFi.RSSI();
#ifdef DEBUG
    Serial.print("RSSI:");
    Serial.println(rssi);

#endif
    String payload = "{\"type\":\"periodic_wifi_mqtt\",";
    payload += "\"micros\":";
    payload += micros();
    payload += ",\"rssi\":";
    payload += rssi;
    payload += ",\"cnt_reconnect\":";
    payload += cntReconnectAttempt;
    payload += ",\"client\":";
    payload += clientId;
    payload += "}";

#ifdef DEBUG
    Serial.print("Publish message: ");
    Serial.println(payload);
#endif
    client.publish((char *)pubTopic.c_str(), (char *)payload.c_str(), true);
  }
}