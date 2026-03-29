# lab-mqtt-vps-arthurpalhano
**Nome:** Arthur Palhano do Rosario  
**Curso:** Técnico em Eletrônica

# Projeto de Monitoramento de Acesso: Estoque Restrito

## 1. Definição do Problema
O projeto resolve a vulnerabilidade de segurança em áreas de armazenamento restrito. O risco real é o acesso não supervisionado e o esquecimento de portas abertas. O sistema garante a rastreabilidade: quem abriu e se a porta ficou aberta por tempo excessivo.

## 2. Escopo do Monitoramento
* **Grandeza:** Estado binário (Aberto/Fechado).
* **Sensor:** Magnético de Contato (Reed Switch).
* **Inteligência de Tempo:** Alerta automático caso a porta fique aberta por mais de 60 segundos.

## 3. Arquitetura da Solução
1. **Dispositivo:** ESP32 + Sensor Reed Switch.
2. **Rede:** Protocolo MQTT via Wi-Fi.
3. **Broker:** Mosquitto MQTT instalado em VPS DigitalOcean.
4. **Aplicação:** Terminal SSH da VPS (Subscriber).

## 4. Especificação Técnica
| Item | Detalhes |
| :--- | :--- |
| **Microcontrolador** | ESP32 |
| **Sensor** | Reed Switch (Digital) |
| **Frequência** | Por evento |
| **Formato Payload** | Texto Simples (String/Integer) |

## 5. Estrutura de Tópicos MQTT
* `empresa/estoque/porta/status`: Envia `0` (fechado) ou `1` (aberto).
* `empresa/estoque/sistema/alerta`: Envia a frase de alerta de tempo excedido.

## 6. Resultados Esperados
* **Monitoramento:** Logs no terminal confirmando a abertura/fechamento.
* **Alerta:** Aviso visual no terminal caso a porta exceda 1 minuto aberta.
