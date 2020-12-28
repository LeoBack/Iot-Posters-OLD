//enum statusRun { ON, OFF, PAUSE };
struct ConfigWifi {
  String ssid = "";
  String password = "";
};

struct ConfigMqtt {
  String IP_server = "";
  String pub = "";
  String sub = "";
};

struct Config {
  unsigned int id = 0;
  String name = "";
  boolean on = true;
  boolean debug = true;
  ConfigWifi wifi; 
  ConfigMqtt mqtt;
};

struct InputConsole {
  boolean isRead = false;
  String readConsole = "";
  String module = "";
  String command = "";
  String parameters = "";
};

struct Timer {
  unsigned long previousMillis;
  unsigned long currentMillis;
  unsigned long interval = 1000;
};

struct ClientRequest {
  boolean isUp = false;
  String command;
  String va1;
  String va2;
};


boolean isDebug = false;
boolean ModeAp = false;
byte on_off = 1;
byte on_off_temp = 1;
String run_sec_name = "none";

//statusRun isRun = ON;
InputConsole inputConsole;
Timer timerRunSecuence;
Timer timerCheckSecuence;
ClientRequest clientResquest;
Config config;

void print_config() {
  Serial.printf("ID: %d\n", config.id);
  Serial.printf("Name: %s\n", config.name.c_str());
  Serial.printf("ON: %s\n", config.on ? "True" : "false");
  Serial.printf("Debug: %s\n", config.debug  ? "True" : "false");
  Serial.printf("WIFI SSID: %s\n", config.wifi.ssid.c_str());
  Serial.printf("WIFI Password: %s\n", config.wifi.password.c_str());
  Serial.printf("MQTT IP_server: %s\n", config.mqtt.IP_server.c_str());
  Serial.printf("MQTT Suscribe: %s\n", config.mqtt.sub.c_str());
  Serial.printf("MQTT Publisher: %s\n", config.mqtt.pub.c_str());
}

// Config
boolean load_config() {
  File configFile = SPIFFS.open(CONFIG, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  if (configFile.size() > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  StaticJsonDocument<600> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Error deserializing the configuration file");
    return false;
  }

  const unsigned int id = doc["id"];
  const char* name = doc["name"];
  const boolean on = doc["on"];
  const boolean debug = doc["debug"];
  config.id = id;
  config.name = name;
  config.on = on;
  config.debug = debug;

  const char* ssid = doc["wifi"]["ssid"];
  const char* password = doc["wifi"]["password"];
  config.wifi.ssid = ssid;
  config.wifi.password = password;

  const char* IP_server = doc["mqtt"]["IP_server"];
  const char* sub = doc["mqtt"]["sub"];
  const char* pub = doc["mqtt"]["pub"];
  config.mqtt.IP_server = IP_server;
  config.mqtt.sub = sub;
  config.mqtt.pub = pub;

  if (config.debug) {
    Serial.println("Load config ---");
    print_config();
  }

  return true;
}

boolean save_config() {
  StaticJsonDocument<500> doc;
  doc["id"] = config.id;
  doc["name"] = config.name;
  doc["on"] = config.on;
  doc["debug"] = config.debug;
  doc["wifi"]["ssid"] = config.wifi.ssid;
  doc["wifi"]["password"] = config.wifi.password;
  doc["mqtt"]["IP_server"] = config.mqtt.IP_server;  
  doc["mqtt"]["sub"] = config.mqtt.sub;
  doc["mqtt"]["pub"] = config.mqtt.pub;

  File configFile = SPIFFS.open(CONFIG, "w");
  if (!configFile) {
    Serial.println("Error opening configuration file for writing");
    return false;
  }
  serializeJson(doc, configFile);

  if (config.debug) {
    Serial.println("Save config ---");
    print_config();
  }

  return true;
}
