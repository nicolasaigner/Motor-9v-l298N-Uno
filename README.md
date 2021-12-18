# Motor-9v-l298N-Uno
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

![Esquema baseado no link abaixo](https://i1.wp.com/portal.vidadesilicio.com.br/wp-content/uploads/2017/05/Ponte-H-2.png?resize=737%2C552&ssl=1 "Esquema")


Referências: 

https://portal.vidadesilicio.com.br/ponte-h-l298n-controle-velocidade-motor/

https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
