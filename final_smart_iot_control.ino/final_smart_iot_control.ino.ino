#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// ================================
// Wi-Fi Credentials
// ================================
const char* ssid = "Mi11X";
const char* password = "pruthvi@rai";

// ================================
// Telegram Bot Settings
// ================================
#define BOT_TOKEN "8712044124:AAHaxD38-LJnL5rHUHYIsDy1cOCGCCMTRMc"
#define CHAT_ID "7727270908"

// ================================
// LED Pin (Internal LED)
// ================================
const int ledPin = 2;

// ================================
// Objects
// ================================
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
WebServer server(80);

unsigned long bot_lasttime = 0;
const unsigned long BOT_MTBS = 2000;

// ================================
// Blink Function
// ================================
void blinkLED() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
  }
}

// ================================
// HTML Dashboard
// ================================
String getHTML() {
  String ledStatus = digitalRead(ledPin) ? "ON" : "OFF";
  String color = digitalRead(ledPin) ? "text-green-500" : "text-red-500";

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Smart Dashboard</title>
<script src="https://unpkg.com/@tailwindcss/browser@4"></script>
</head>

<body class="bg-gradient-to-br from-indigo-100 to-blue-50 min-h-screen font-sans">

<div class="bg-white shadow-md p-4 flex justify-between items-center">
  <h1 class="font-bold text-lg">Smart IoT Dashboard</h1>
  <a href="https://www.linkedin.com/in/pruthviraj-a-rai" target="_blank" class="text-blue-600 text-sm">LinkedIn</a>
</div>

<div class="max-w-3xl mx-auto mt-6 p-4">

<div class="bg-white p-6 rounded-xl shadow-lg">

<h2 class="text-lg font-semibold mb-4">ESP32 Control Panel</h2>

<div class="flex justify-between items-center bg-gray-100 p-4 rounded-lg">
<span>LED STATUS</span>
<span id="status" class="font-bold text-xl )rawliteral";

  html += color;
  html += R"rawliteral(">)rawliteral";
  html += ledStatus;
  html += R"rawliteral(</span>
</div>

<div class="grid grid-cols-2 gap-4 mt-6">

<a href="/on" class="bg-indigo-600 text-white text-center py-3 rounded-lg">TURN ON</a>
<a href="/off" class="bg-red-500 text-white text-center py-3 rounded-lg">TURN OFF</a>
<a href="/blink" class="bg-yellow-500 text-white text-center py-3 rounded-lg">BLINK</a>
<a href="/" class="bg-gray-300 text-center py-3 rounded-lg">REFRESH</a>

</div>

</div>

<div class="bg-white p-6 rounded-xl shadow-lg mt-6">

<h2 class="font-semibold mb-3">About Me</h2>
<p><h2>Pruthviraj A Rai<h2><br>I am a Computer Science and Engineering student at MITE.</p>
<p>Interested in Embedded Systems, IoT, and Cybersecurity.</p>

</div>

</div>

<script>
setInterval(async () => {
  const res = await fetch('/');
  const text = await res.text();
  const match = text.match(/LED STATUS.*>(ON|OFF)</);

  if (match) {
    const el = document.getElementById("status");
    el.innerText = match[1];
    el.className = match[1] === "ON"
      ? "font-bold text-xl text-green-500"
      : "font-bold text-xl text-red-500";
  }
}, 4000);
</script>

</body>
</html>
)rawliteral";

  return html;
}

// ================================
// Web Routes
// ================================
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/html", getHTML());
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html", getHTML());
}

void handleBlink() {
  blinkLED();
  server.send(200, "text/html", getHTML());
}

// ================================
// Telegram Handler
// ================================
void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String name = bot.messages[i].from_name;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized User", "");
      continue;
    }

    if (text == "/start" || text == "/help") {
      String msg = "Welcome " + name + "\n\n";
      msg += "/on - LED ON\n";
      msg += "/off - LED OFF\n";
      msg += "/blink - Blink LED\n";
      msg += "/status - LED Status\n";
      msg += "/web - Open Dashboard\n";

      bot.sendMessage(chat_id, msg, "");
    }

    else if (text == "/on") {
      digitalWrite(ledPin, HIGH);
      bot.sendMessage(chat_id, "LED ON", "");
    }

    else if (text == "/off") {
      digitalWrite(ledPin, LOW);
      bot.sendMessage(chat_id, "LED OFF", "");
    }

    else if (text == "/blink") {
      blinkLED();
      bot.sendMessage(chat_id, "LED Blinked", "");
    }

    else if (text == "/status") {
      String status = digitalRead(ledPin) ? "ON" : "OFF";
      bot.sendMessage(chat_id, "LED is " + status, "");
    }

    else if (text == "/web") {
      String ip = WiFi.localIP().toString();
      bot.sendMessage(chat_id, "Dashboard:\nhttp://" + ip, "");
    }
  }
}

// ================================
// Setup
// ================================
void setup() {

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  client.setInsecure();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/blink", handleBlink);

  server.begin();
}

// ================================
// Loop
// ================================
void loop() {

  server.handleClient();

  if (millis() - bot_lasttime > BOT_MTBS) {

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}