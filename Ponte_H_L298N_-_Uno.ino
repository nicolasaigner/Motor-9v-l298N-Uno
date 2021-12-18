/*
  Controle de velocidade e rotação de um motor DC 9V (Motor de impressora, no meu caso).
  Está sendo utilizado apenas um motor na saída B.

  O código aceita comandos como:
    - Esquerda = a;
    - Direita = d;
    - Parar = s;
    - Velocidade = 0 à 255, sendo 0 a menor velocidade (no caso do meu motor o mínimo dele é 50, então foi configado que qual velocidade menor que 50 ele coloca em 50, teste alterando essa velocidade no código abaixo);
    - Tempo de rotação em segundos = 0 ou não passar o parâmetro (Infinito), 1 é um segundo, meio segundo, 0.5 e assim por diante;
    - Exemplo de comando a ser enviado no serial no loop();

  Hardware Necessário:
    - Ponte H L298N;
    - Jumpers Macho x Macho;
    - Jumpers Macho x Fêmea;
    - Arduino UNO;
    - Fonte 9V 2A;
    - Protoboard (Não obrigatório, mas ajuda);
    - Cabo USB para o Arduíno

  Esquema da pinagem:
    - Arduino:
      - Pino Digital 3 (A1);
      - Pino Digital 4 (A2);
      - Pino Digital 5 (A3);
      - GND (A4);
    
    - Ponte H L298N:
      - Pino ENB (P1);
      - Pino IN3 (P2);
      - Pino IN4 (P3);
      - +12 (P4);
      - GND (P5);
      - OUT3 (P6);
      - OUT4 (P7);

    - Fonte 9V:
      - Positivo (F1);
      - Negativo (F2);

    - Motor 9V:
      - Positivo (M1);
      - Negativo (M2);

    - Ligação:
      - Arduino -> Ponte H
        - A1 -> P1;
        - A2 -> P2;
        - A3 -> P3;
      - Fonte -> Ponte H
        - F1 -> P4;
        - F2 -> P5;
      - Fonte -> Arduino
        - F2 -> A4;

      - Ponto H -> Motor
        - P6 -> M1;
        - P7 -> M2;


      Referência do Código: https://www.filipeflop.com/blog/motor-dc-arduino-ponte-h-l298n/
*/
int ENB = 3; // Output para o controle de velocidade do motor;
int IN3 = 4; // Output para a direção 
int IN4 = 5; // Output para a direção

int MIN_SPEED = 50; // Velocidade mínima aceita pelo meu motor de 9V;
int MAX_SPEED = 255; // Máximo que a porta consegue enviar para o motor;

void setup() {
  // Confiração dos pinos como saída;
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  // Variáveis auxiliares só para testar o motor;
  int speedMotor = 50; // Menor velocidade possível do motor;
  float timeRotation = 0.5; // Meio segundo para rotacionar o motor;

  // Teste do motor;
  rightRotation(speedMotor, timeRotation); // Roda para a direita na velocidade 50 por meio segundo;
  delay(200); // Pausa rápida;
  leftRotation(speedMotor, timeRotation); // Roda para a esquerda na velocidade 50 por meio segundo;
  delay(200); // Pausa rápida;
  stopMotor(); // Para o motor;

  Serial.begin(9600); // Inicialização do Serial;

  Serial.println("Ready..."); // Mensagem no Monitor Serial indicando que o Arduino está pronto;
}

void loop() {
  if (Serial.available()) { // Se houver algum comando enviado no serial, executa o script;
    String rxString = ""; // Variável que vai receber a concatenação de chars;
    char* strArr[16]; // Variável que recebe o que foi enviado no Serial;

    // Concatenação da String;
    while (Serial.available()) {
      delay(2);
      char ch = Serial.read();
      rxString += ch; 
    }

    /* 
     *  Exemplo de comando enviado para rotacionar para esquerda na velocidade 100 por 1 segundo: "a,100,1" (Sem as aspas, por favor)
     *  
     *  Exemplo de comando enviado para rotacionar para direita na velocidade 100 por 1 segundo: "d,100,1" (Sem as aspas, por favor)
     *  
     *  Exemplo de comando enviado para parar o motor: "s" (Sem as aspas, por favor)
     *  
     *  Exemplo de comando enviado para rotacionar para direita na velocidade 100 inititamente: "d,120" (Sem as aspas, por favor)
     *  
     *  Exemplo de comando enviado para rotacionar para esquerda na menor velocidade do motor: "a" (Sem as aspas, por favor)
     *  
     *  a = Esquerda
     *  d = Direita
     *  s = Parar
     *  
     *  100 = Velocidade (0 à 255);
     *  1 = 1 Segundo (Pode ser vazio ou 0 (zero) também, vai rotacionar até receber um novo comando ou desligar da energia);
    */
    String rotation = getValue(rxString, ',', 0); // Pega a primeira posição da string divida por vírgula (,);
    String velocityString = getValue(rxString, ',', 1); // Pega a segunda posição da string divida por vírgula (,);
    String timerString = getValue(rxString, ',', 2); // Pega a terceira posição da string divida por vírgula (,);
    int velocity = velocityString.toInt(); // Converte a velocidade para Int;
    float timer = timerString.toFloat(); // Converte o tempo para Float;
    
    if (velocity < MIN_SPEED) {
      velocity = MIN_SPEED; // Aqui você faz o teste para saber a velocidade mínima que seu motor vai suportar, basta alterar a variável MIN_SPEED no código acima;
    } else if (velocity > MAX_SPEED) { // Caso for informado um valor de velocidade maior que 255 ele limita ao máxímo de 255;
      velocity = MAX_SPEED;
    }

    // Debugs;
    Serial.print("rotation = ");
    Serial.print(rotation);
    Serial.print(" ");
    Serial.print("velocity = ");
    Serial.print(velocity);
    Serial.print(" ");
    Serial.print("timer = ");
    Serial.println(timer);
    
    switch (rotation[0]) { // Converte a string para char e verifica nos cases, qualquer coisa diferente não faz nada;
      case 'd': // Tecla D, caso queira alterar a tecla, basta trocar aqui
      // case 'j': // Ou descomente essa linha e toque a letra aqui, no caso está sendo recomendado a tecla J;
        stopMotor(); // Para o motor;
        delay(200); // Pausa rápida;
        rightRotation(velocity, timer); // Rotaciona para a direita na velocidade determinada e tempo determinado;
        break;
      case 'a': // Tecla A, caso queira alterar a tecla, basta trocar aqui
      // case 'l': // Ou descomente essa linha e toque a letra aqui, no caso está sendo recomendado a tecla L;
        stopMotor(); // Para o motor;
        delay(200); // Pausa rápida;
        leftRotation(velocity, timer); // Rotaciona para a esquerda na velocidade determinada e tempo determinado;
        break;
      case 's': // Tecla S, caso queira alterar a tecla, basta trocar aqui
      // case 'k': // Ou descomente essa linha e toque a letra aqui, no caso está sendo recomendado a tecla K;
        stopMotor(); // Para o motor;
        break;
      default:
        break;
    }
  }
}

// Função para rotacionar para a direita;
void rightRotation(int vel, float timer) {

  analogWrite(ENB, vel); // Envia a velocidade informada do Serial para o P1;
  digitalWrite(IN3, HIGH); // Aciona o P2;
  digitalWrite(IN4, LOW); // Desativa o P3;
  if (timer > 0.00) { // Caso o tempo for maior que 0, entra aqui;
    timer = timer * 1000; // Multiplica o número por 1000 para receber o valor em milissegundos;
    delay(timer); // Mater o motor ligado com o tempo em segundos determinado no Serial;
    stopMotor(); // Para o motor;
  }

}

// Função para rotacionar para a esquerda;
void leftRotation(int vel, float timer) {

  analogWrite(ENB, vel); // Envia a velocidade informada do Serial para o P1;
  digitalWrite(IN3, LOW); // Desativa o P2;
  digitalWrite(IN4, HIGH); // Aciona o P3;
  
  if (timer > 0.00) { // Caso o tempo for maior que 0, entra aqui;
    timer = timer * 1000; // Multiplica o número por 1000 para receber o valor em milissegundos;
    delay(timer); // Mater o motor ligado com o tempo em segundos determinado no Serial;
    stopMotor(); // Para o motor;
  }
}

// Função para parar o motor;
void stopMotor() {
  digitalWrite(IN3, LOW); // Desativa o P2;
  digitalWrite(IN4, LOW); // Desativa o P3;
}

// Função para retornar os valores da string separado por vírgula;
// Referência do código: https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
String getValue(String data, char separator, int index) {
    int found = 0; 
    int strIndex[] = { 0, -1 }; 
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
