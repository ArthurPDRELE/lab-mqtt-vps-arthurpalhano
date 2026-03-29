#include <WiFi.h>
#include <PubSubClient.h>

// --- Configurações de Rede e Broker ---
const char* ssid = "Wokwi-GUEST"; 
const char* password = "";        
const char* mqtt_server = "broker.emqx.io"; 

// --- Pinos e Variáveis de Controle ---
const int pinoSensor = 23; 
int estadoAnterior = -1;
unsigned long tempoAbertura = 0;
bool alertaTempoExcedidoEnviado = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("\nConectando Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32_Estoque_Arthur")) {
      Serial.println("Conectado ao Broker!");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(pinoSensor, INPUT_PULLUP);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int estadoAtual = !digitalRead(pinoSensor); 

  // --- Lógica 1: Mudança de Estado ---
  if (estadoAtual != estadoAnterior) {
    // Envia apenas "0" ou "1" (Texto Puro)
    client.publish("empresa/estoque/porta/status", estadoAtual ? "1" : "0");
    
    Serial.print("Status enviado: ");
    Serial.println(estadoAtual);

    if (estadoAtual == 1) {
      tempoAbertura = millis();
      alertaTempoExcedidoEnviado = false;
    } else {
      tempoAbertura = 0;
    }
    
    estadoAnterior = estadoAtual;
    delay(200);
  }

  // --- Lógica 2: Alerta de Tempo Excedido ---
  if (estadoAtual == 1 && tempoAbertura > 0) {
    if (millis() - tempoAbertura >= 60000 && !alertaTempoExcedidoEnviado) {
      // Envia uma frase simples (Texto Puro)
      client.publish("empresa/estoque/sistema/alerta", "ALERTA: PORTA ABERTA HÁ MAIS DE 1 MINUTO");
      
      Serial.println("!!! ALERTA ENVIADO !!!");
      alertaTempoExcedidoEnviado = true;
    }
  }
}
