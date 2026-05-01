#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Mi11X";
const char* password = "pruthvi@rai";

// -----------------------------------
// Telegram Bot Settings
// -----------------------------------

#define BOT_TOKEN "8712044124:AAHaxD38-LJnL5rHUHYIsDy1cOCGCCMTRMc"
#define CHAT_ID "7727270908"

// -------------------- LED --------------------
const int ledPin = 2;
bool ledState = false;

// -------------------- Objects --------------------
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
WebServer server(80);

unsigned long lastTime = 0;
const int interval = 2000;

// -------------------- LED Functions --------------------
void turnOn() {
  digitalWrite(ledPin, HIGH);
  ledState = true;
}

void turnOff() {
  digitalWrite(ledPin, LOW);
  ledState = false;
}

void blinkLED() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

// -------------------- JSON API --------------------
void handleStatus() {
  String json = "{";
  json += "\"led\":\"" + String(ledState ? "ON" : "OFF") + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// -------------------- WEB PAGE --------------------
void handleRoot() {
String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart IoT Dashboard – Pruthviraj A. Rai</title>

  <script src="https://unpkg.com/@tailwindcss/browser@4"></script>

  <style>
    /* LED Glow Animation */
    .led-on {
      color: #22c55e;
      text-shadow: 0 0 10px #22c55e, 0 0 20px #22c55e, 0 0 40px #22c55e;
      animation: glow 1s infinite alternate;
    }

    .led-off {
      color: #ef4444;
      text-shadow: none;
    }

    @keyframes glow {
      from {
        text-shadow: 0 0 5px #22c55e;
      }
      to {
        text-shadow: 0 0 20px #22c55e, 0 0 40px #22c55e;
      }
    }
  </style>
</head>

<body class="bg-gray-50 font-sans text-gray-800">

  <!-- Header -->
  <header class="bg-white shadow-sm py-4">
    <div class="max-w-4xl mx-auto px-4 flex items-center space-x-4">
      
      <div class="rounded-full bg-indigo-200 w-16 h-16 flex items-center justify-center text-xl font-semibold text-indigo-800">
        PA
      </div>

      <div>
        <h1 class="text-2xl font-bold text-indigo-900">Pruthviraj A. Rai</h1>
        <p class="text-sm text-gray-600">Computer Science & Engineering student @ MITE</p>

        <div class="mt-1 flex items-center space-x-2">
          <img src="https://th.bing.com/th/id/OIP.giEfoXvjjpl2Qu6-nQQP2AHaHa?w=174&h=180&c=7&r=0&o=7&dpr=1.3&pid=1.7&rm=3"
               alt="LinkedIn" class="w-5 h-5">
          <a href="https://www.linkedin.com/in/pruthviraj-a-rai"
             class="text-indigo-600 hover:underline text-sm"
             target="_blank">LinkedIn Profile</a>
        </div>
      </div>

    </div>
  </header>

  <!-- Main -->
  <main class="max-w-4xl mx-auto mt-8 px-4">

    <!-- Dashboard -->
    <section class="bg-white rounded-lg shadow-md p-6 text-center">

      <h2 class="text-xl font-semibold mb-4 text-gray-800">
        ESP32 Smart Web Dashboard
      </h2>

      <!-- LED Status -->
      <div class="mb-6">
        <span class="font-medium">LED STATUS:</span>
        <div id="ledStatus" class="text-3xl font-bold mt-2 led-off">
          LOADING...
        </div>
      </div>

      <!-- Buttons -->
      <div class="grid grid-cols-2 sm:grid-cols-3 gap-4">

        <a href="#" onclick="control('/on')" class="bg-indigo-600 text-white py-2 rounded-lg hover:bg-indigo-700">
          TURN ON
        </a>

        <a href="#" onclick="control('/off')" class="bg-red-600 text-white py-2 rounded-lg hover:bg-red-700">
          TURN OFF
        </a>

        <a href="#" onclick="control('/blink')" class="bg-amber-500 text-white py-2 rounded-lg hover:bg-amber-600">
          BLINK LED
        </a>

        <a href="#" onclick="updateStatus()" class="bg-gray-300 text-gray-800 py-2 rounded-lg hover:bg-gray-400">
          REFRESH
        </a>

      </div>

    </section>

    <!-- About -->
    <section class="mt-8 bg-white rounded-lg shadow-md p-6">
      <h2 class="text-xl font-semibold mb-4 text-gray-800">About Me</h2>

      <p class="text-gray-700 mb-2">
        I am a Computer Science and Engineering student at <strong>Mangalore Institute of Technology & Engineering (MITE)</strong>, Moodabidri.
      </p>

      <p class="text-gray-700 mb-2">
        Driven by a fascination for how hardware and software interact, I focus on <strong>embedded systems</strong> and <strong>microcontroller programming</strong>.
      </p>

      <p class="text-gray-700">
        I also have strong interest in <strong>cybersecurity</strong> and <strong>network hardware</strong>.
      </p>
    </section>

  </main>

  <!-- Footer -->
  <footer class="mt-12 py-4 text-center text-sm text-gray-500">
    © 2026 Pruthviraj A. Rai – All rights reserved.
  </footer>

  <!-- SCRIPT -->
  <script>

    async function control(endpoint) {
      await fetch(endpoint);
      setTimeout(updateStatus, 300);
    }

    async function updateStatus() {
      try {
        const res = await fetch('/status');
        const data = await res.json();

        const el = document.getElementById('ledStatus');
        el.textContent = data.led;

        if (data.led === "ON") {
          el.classList.remove("led-off");
          el.classList.add("led-on");
        } else {
          el.classList.remove("led-on");
          el.classList.add("led-off");
        }

      } catch (e) {
        console.error(e);
      }
    }

    setInterval(updateStatus, 2000);
    updateStatus();

  </script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// -------------------- WEB ROUTES --------------------
void handleOn() {
  turnOn();
  server.send(200, "text/plain", "ON");
}

void handleOff() {
  turnOff();
  server.send(200, "text/plain", "OFF");
}

void handleBlink() {
  blinkLED();
  server.send(200, "text/plain", "BLINK");
}

// -------------------- TELEGRAM MENU --------------------
void sendMenu(String chat_id) {
  String keyboard = "[[\"ON\",\"OFF\"],[\"BLINK\",\"STATUS\"],[\"WEB\"]]";
  
  bot.sendMessageWithReplyKeyboard(
    chat_id,
    "📱 Control Panel",
    "",
    keyboard,
    true
  );
}

// -------------------- TELEGRAM HANDLER --------------------
void handleMessages(int num) {
  for (int i = 0; i < num; i++) {

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized", "");
      continue;
    }

    if (text == "/start" || text == "/menu") {
      sendMenu(chat_id);
    }

    else if (text == "ON") {
      turnOn();
      bot.sendMessage(chat_id, "LED ON", "");
    }

    else if (text == "OFF") {
      turnOff();
      bot.sendMessage(chat_id, "LED OFF", "");
    }

    else if (text == "BLINK") {
      blinkLED();
      bot.sendMessage(chat_id, "LED BLINKED", "");
    }

    else if (text == "STATUS") {
      bot.sendMessage(chat_id, ledState ? "LED ON" : "LED OFF", "");
    }

    else if (text == "WEB") {
      bot.sendMessage(chat_id, "🌐 http://" + WiFi.localIP().toString(), "");
    }
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  turnOff();

  client.setInsecure();

  WiFi.begin(ssid, password);
  Serial.println("Connecting WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("...");
  }

  Serial.println("Connected!");
  Serial.println(WiFi.localIP());

  // Web routes
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/blink", handleBlink);
  server.on("/status", handleStatus);

  server.begin();
}

// -------------------- LOOP --------------------
void loop() {
  server.handleClient();

  if (millis() - lastTime > interval) {
    int num = bot.getUpdates(bot.last_message_received + 1);

    while (num) {
      handleMessages(num);
      num = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTime = millis();
  }
}