#include <IRremoteESP8266.h> //INCLUSÃO DE BIBLIOTECA

IRsend irsend(14); //ENVIO DO SINAL IR / PORTA GPIO14(D5)

int tamanho = 68; //TAMANHO DA LINHA RAW(68 BLOCOS)
int frequencia = 32; //FREQUÊNCIA DO SINAL IR(32KHz)

 
// BOTÃO LIGA / DESLIGA
unsigned int LD[68] = {9000,4550,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,1650,650,1650,650,1650,650,1700,650,1700,650,1700,650,550,650,1700,650,1700,650,1700,650,550,650,550,650,550,650,550,600,600,600,600,600,600,600,600,600,1700,600,1700,600,1700,600,1700,600,1700,600,1700,600}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// BOTÃO AUMENTAR TEMPERATURA
unsigned int TMAIS[68] = {9050,4500,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,550,650,1700,650,1700,650,1700,650,1700,650,1700,650,1700,650,550,600,1700,600,600,600,1700,600,1700,600,600,600,1700,600,600,600,600,600,600,600,1700,600,600,600,600,600,1700,600,600,600,1700,600,1700,600,1700,600}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// BOTÃO DIMINUIR TEMPERATURA
unsigned int TMENOS[68] = {9000,4550,600,550,600,550,600,550,600,550,600,550,600,550,600,550,600,550,600,1700,600,1700,600,1700,600,1700,600,1700,600,1700,600,550,600,1700,600,550,600,1700,600,550,600,1700,600,1700,600,550,600,550,600,550,600,1700,600,550,600,1700,600,550,600,550,600,1700,600,1700,650,1650,650}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES

void setup() {
  irsend.begin(); //INICIALIZA A FUNÇÃO
  Serial.begin(115200); //INICIALIZA A PORTA SERIAL
}

void loop() {
  char c = Serial.read(); //VARIÁVEL RESPONSÁVEL POR RECEBER O CARACTER DIGITADO NA JANELA SERIAL

  if (c == 'w'){ //SE CARACTER DIGITADO FOR IGUAL A "w", FAZ
    irsend.sendRaw(LD,tamanho,frequencia);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
    Serial.println("Comando enviado: Liga / Desliga");
    delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
  }
  if (c == 'd'){ //SE CARACTER DIGITADO FOR IGUAL A "d", FAZ
    irsend.sendRaw(TMAIS,tamanho,frequencia); // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
    Serial.println("Comando enviado: Aumentar Temperatura");
    delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
  }
  if (c == 'a'){ //SE CARACTER DIGITADO FOR IGUAL A "a", FAZ
    irsend.sendRaw(TMENOS,tamanho,frequencia); // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
    Serial.println("Comando enviado: Diminuir Temperatura");
    delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
  }

}
