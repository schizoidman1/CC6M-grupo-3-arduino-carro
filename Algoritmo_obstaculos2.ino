#include <Ultrasonic.h>

// define o nome do sensor e a quais pinos (trig, echo) está ligado
Ultrasonic UE(13, 12);  // esquerda 
Ultrasonic UD(11, 10);  // direita
Ultrasonic UF(9, 8);    // frente

int Aenable = 7;
int Ain1 = 6;
int Ain2 = 5;
int Bin1 = 4;
int Bin2 = 3;
int Benable = 2;

int direction = 0; // 0 : frente, 1 : esquerda, 2 : direita, 3 : costas
/*
Entender esse número como direção que a frente do Micromouse aponta em relação à pista.
Os movimentos de rotação à direita e à esquerda são de 90º.
0 + 0 = 0
0 + 1 = 1   > estando de frente, vira a esquerda
0 + 2 = 2   > estando de frente, vira a esquerda
1 + 1 = 3   > vira duas vezes à esquerda, ficando de costas
2 + 2 = 3   > vira duas vezes à direita, ficando de costas 
1 + 2 = 0   > vira à esquerda, depois à direita, ficando de frente
2 + 1 = 0   > vira à direita, depois à esquerda, ficando de frente
3 + 1 = 2   > estando de costas, vira à esquerda, ficando virado à direita
3 + 2 = 1   > estando de costas, vira à direita, ficando virado à esquerda
*/
int speed = 200;

void setup() {
  pinMode(Aenable, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(Benable, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);

  Serial.begin(9600);
  while(!Serial);
}

int _speed(int distance) {
  if(distance >= 10) {
    return speed;
  }
  else {
    return 0;
  }
}

int turn_direction(int turn) {  // turn sempre deve ser 1 (virar a esquerda) ou 2 (virar a direita)
  if(direction == 0) return turn;

  if(direction == 1) { 
    if(turn == 1) return 3;
    else return 0;
  }

  if(direction == 2) {
    if(turn == 1) return 0;
    else return 3;
  }

  if(direction == 3) {
    if(turn == 1) return 2;
    else return 1;
  }
}

void turn_left() {
    analogWrite(Aenable, speed);  
    analogWrite(Benable, speed);
    // left wheel rotates backward
    digitalWrite(Ain1, LOW);
    digitalWrite(Ain2, HIGH);
    // right wheel rotates forward
    digitalWrite(Bin1, HIGH);
    digitalWrite(Bin2, LOW);

    direction = turn_direction(1); 
    delay(2500);
    digitalWrite(Ain2, LOW);
    digitalWrite(Bin1, LOW);
}

void turn_right() {
    analogWrite(Aenable, speed);  
    analogWrite(Benable, speed);
    // left wheel rotates forward
    digitalWrite(Ain1, HIGH);
    digitalWrite(Ain2, LOW);
    // right wheel rotates backward
    digitalWrite(Bin1, LOW);
    digitalWrite(Bin2, HIGH);

    direction = turn_direction(2); 
    delay(2500);
    digitalWrite(Ain1, LOW);
    digitalWrite(Bin2, LOW);
}

void move_forward() {
    int sp= _speed(UF.Ranging(CM));
    analogWrite(Aenable, sp);  
    analogWrite(Benable, sp);
    // left wheel rotates forward
    digitalWrite(Ain1, HIGH);
    digitalWrite(Ain2, LOW);
    // right wheel rotates forward
    digitalWrite(Bin1, HIGH);
    digitalWrite(Bin2, LOW);

    delay(2500);
    digitalWrite(Ain1, LOW);
    digitalWrite(Bin1, LOW);  
}

void loop() {
  if(direction == 3) {
    Serial.print("3 > ");
    if(UE.Ranging(CM) >= 20) {
      Serial.println("1");
      turn_left();
    }
    else if(UD.Ranging(CM) >= 20) {
      Serial.println("2");
      turn_right();
    }
    else if(UF.Ranging(CM) < 20) {
      Serial.println("3");
      turn_right();
      turn_right();
    }
    else {
      Serial.println("0");
      move_forward();
    }
  }
  else if(direction == 2) {
    Serial.print("2 > ");
    if(UE.Ranging(CM) >= 20) {
      Serial.println("1");
      turn_left();
    }
    else if (UF.Ranging(CM) >= 20) {
      Serial.println("0");
      move_forward();
    }
    else if (UD.Ranging(CM) < 20) {
      Serial.println("3");
      turn_right();
      turn_right();
    }
    else {
      Serial.println("2");
      turn_right();
    }  
  }
  else if(direction == 1) {
    Serial.print("1 > ");
    if(UD.Ranging(CM) >= 20) {
      Serial.println("2");
      turn_right();
    }
    else if(UF.Ranging(CM) >= 20) {
      Serial.println("0"); 
      move_forward();
    }
    else if(UE.Ranging(CM) < 20) {
      Serial.println("3");
      turn_right();
      turn_right();
    }
    else {
      Serial.println("1");
      turn_left();
    }
  }
  else { // direction == 0
    Serial.print("0 > ");
    if(UF.Ranging(CM) >= 20) {
      Serial.println("0");
      move_forward();
    }
    else if(UD.Ranging(CM) >= 20) {
      Serial.println("2");
      turn_right();
    }
    else if(UE.Ranging(CM) >= 20) {
      Serial.println("1");
      turn_left();
    }
    else {
      Serial.println("3");
      turn_right();
      turn_right();
    }
  }
}
