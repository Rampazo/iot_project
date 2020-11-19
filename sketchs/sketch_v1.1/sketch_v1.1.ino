#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include <IRremoteESP8266.h>

#define TMPPin A0 // Sensor Temperatura (TMP36)
IRsend irsend(14); // LED IR (TIL 32)

/////////////////////
// Led IR Emitter
////////////////////

int sizeLine = 68; // Tamanho da Linha RAW(68 BLOCKS)
int frequency = 32; // Frequência do Sinal IR (32KHz)

// Comando Liga/Desliga
unsigned int PO[68] = {9000,4550,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,1650,650,1650,650,1650,650,1700,650,1700,650,1700,650,550,650,1700,650,1700,650,1700,650,550,650,550,650,550,650,550,600,600,600,600,600,600,600,600,600,1700,600,1700,600,1700,600,1700,600,1700,600,1700,600}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// Comando Aumentar Temperatura
unsigned int TMORE[68] = {9050,4500,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,1700,650,1700,650,1700,650,1700,650,1700,650,1700,650,550,600,1700,600,600,600,1700,600,1700,600,600,600,1700,600,600,600,600,600,600,600,1700,600,600,600,600,600,1700,600,600,600,1700,600,1700,600,1700,600}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// Comando Diminuir Temperatura
unsigned int TLESS[68] = {9000,4550,600,550,600,550,600,550,600,550,600,550,600,550,600,550,600,550,600,1700,600,1700,600,1700,600,1700,600,1700,600,1700,600,550,600,1700,600,550,600,1700,600,550,600,1700,600,1700,600,550,600,550,600,550,600,1700,600,550,600,1700,600,550,600,550,600,1700,600,1700,650,1650,650}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES


/////////////////////
// WiFi Connection
////////////////////

const char* SSID = "Shark";
const char* PASSWORD = "a1b2c3d4e5";
WiFiClient wifiClient;                        


/////////////////////
// MQTT Server
////////////////////

const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;

#define ID_MQTT  "ThinkpadT420_Terminal"
#define TOPIC_PUBLISH "Temp_gab"
#define TOPIC_SUBSCRIBE "control_msg"
PubSubClient MQTT(wifiClient);

/////////////////////
// Declare Functions
////////////////////

void keepConnection();
void connectWiFi();
void connectMQTT();
void sendPayload();
void receivePayload(char* topic, byte* payload, unsigned int length);

/////////////////////
// Start Service
////////////////////

void setup() {    
  irsend.begin();
  Serial.begin(115200);

  connectWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(receivePayload); 
}

void loop() {
  keepConnection();
  sendPayload();
  MQTT.loop();
}

/////////////////////
// Functions
////////////////////

void keepConnection() {
    if (!MQTT.connected()) {
       connectMQTT(); 
    }
    
    connectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void connectWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
        
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);  
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP()); 
}

void connectMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void sendPayload() {
  
  String temperatureString = "";
  
  int tmpValue = analogRead(TMPPin); // Ler Temperatura
  float voltage = tmpValue * 3.3;// Convertendo a leitura em voltagem
  voltage /= 1024.0;
  
  float temperatureC = (voltage - 0.5) * 100 ;  // Convertendo para graus Celsius
  
  //  temperatureString = " " + String(temperatureC) + " C  ";
  
  MQTT.publish(TOPIC_PUBLISH, String(temperatureC).c_str());
  delay(1000);
}

void receivePayload(char* topic, byte* payload, unsigned int length) {
    
    String msg;

    // Obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    if (msg == "p"){
      irsend.sendRaw(PO,sizeLine,frequency); // Parâmetros necessários para envio de sinal IR
      Serial.println("Comando enviado: Liga / Desliga");
      delay(50);
    }
    if (msg == "m"){
      irsend.sendRaw(TMORE,sizeLine,frequency);  // Parâmetros necessários para envio de sinal IR
      Serial.println("Comando enviado: Aumentar Temperatura");
      delay(50);
    }
    if (msg == "l"){
      irsend.sendRaw(TLESS,sizeLine,frequency);  // Parâmetros necessários para envio de sinal IR
      Serial.println("Comando enviado: Diminuir Temperatura");
      delay(50);
    }
}
