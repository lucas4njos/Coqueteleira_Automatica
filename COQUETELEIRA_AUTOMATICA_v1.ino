//COQUETELEIRA AUTOMATICA
//29/06/2022
//Lucas G. dos Anjos
//Instituto Federal de Educação, Ciência e Tecnologia de São Paulo - Campus Salto


#include <LiquidCrystal_I2C.h>

//Inicializa o display no endereco 0x27, conforme Scan
LiquidCrystal_I2C lcd(0x27,16,2);

#define pinL 2
#define pinM 3
#define pinR 4 //define as entradas dos botões (L = left/esquerda, M = middle/meio e R = right/direita)

int ml = 0; //define quantos ml serão servidos (tempo de acionamento da bomba)
int drink = 0; //define qual motor será acionado (qual bebida)
int a = 0; //variável para ser utilizada nas estruturas de repetição secundárias
int total_ml = 0; //conta o total de ml e serve para travar cada dose em 300ml, independente da qtd de vezes que a bomba foi acionada
bool done=0; //variável de 0 ou 1, que serve para sair e entrar da repetição principal

void setup() {
  pinMode(pinL,INPUT_PULLUP);
  pinMode(pinM,INPUT_PULLUP);
  pinMode(pinR,INPUT_PULLUP); //input_pullup utiliza os resistores internos do arduino

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); //saídas para o módulo relé

  Serial.begin(9600); //início da porta serial

  lcd.init(); //início do display lcd
}

void Start(){
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   4 Penguins   ");
  lcd.setCursor(0,1);
  delay(1000);
  lcd.print("     Company    ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Loading.     ");
  delay(500);
  lcd.clear();
  lcd.print("   Loading..    ");
  delay(500);
  lcd.clear();
  lcd.print("   Loading...   ");
  delay(500);

  drink = 0;
  total_ml = 0;
}

void Finaliza(){
  lcd.clear();
  if (total_ml > 299){
    lcd.setCursor(0,0);
    lcd.print("   Copo Cheio!  ");
    lcd.setCursor(0,1);
    lcd.print("Retire seu copo ");
    delay(3000);
    a = 1;
    done = 1;  
  }
  while (a == 0){
    lcd.setCursor(0,0);
    lcd.print("Quer continuar?");
    lcd.setCursor(0,1);
    lcd.print("[1]SIM | NAO[3]");
    if (!digitalRead(pinL)){
      delay(150);
      a = 1;
    }
    else if (!digitalRead(pinR)){
      delay(150);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Retire Seu Copo ");
      lcd.setCursor(0,1);
      lcd.print("    Obrigado!   ");
      delay(3000);
      a = 1;
      done = 1;
    }
    delay(150);
  }
}

void Tela_Inicio(){
  lcd.setCursor(0,0);
  lcd.print("Escolha a Bebida");
  lcd.setCursor(0,1);
  lcd.print(" [1]  [2]  [3]");
}

void Print_Lcd(){
  if (ml<0){
    ml = 0;
    total_ml += 10;}
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bebida: ");
  lcd.print(drink);
  lcd.setCursor(0,1);
  lcd.print(ml);
  lcd.print("    ml");
}

void Full_Glass(){
  if (total_ml > 300){
    ml -= 10;
    total_ml -= 10;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Atencao! Limite ");
    lcd.print(drink);
    lcd.setCursor(0,1);
    lcd.print("Maximo de 300 ml");
    delay(3000);
  }
}

void Menu_1(){
  while (a == 0){
    if (!digitalRead(pinL)){
      delay(250);
      drink = 1;
      a = 1;}
    if (!digitalRead(pinM)){
      delay(250);
      drink = 2;
      a = 1;}
    if (!digitalRead(pinR)){
      delay(250);
      drink = 3;
      a = 1;}
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bebida: ");
  lcd.print(drink);
  lcd.setCursor(0,1);
  lcd.print("    ml");
}

void Menu_2(){
  while (a == 0){
  if (!digitalRead(pinL)){
    delay(250);
    ml -= 10;
    total_ml -= 10;
    Full_Glass();
    Print_Lcd();
    }
    else if (!digitalRead(pinR)){
    delay(250);
    ml += 10;
    total_ml += 10;
    Full_Glass();
    Print_Lcd();
    }
    else if (!digitalRead(pinM)){
    delay(250);
    a = 1;
    }
  }    
}

void Motor(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aguarde...");
  lcd.setCursor(0,1);
  lcd.print("Total ml: ");
  lcd.print(total_ml);  
  //1L / 60s -> 10 ml = 0,6s => delay(600). Como a unidade é de 10 em 10ml, delay(60), pois 60*10 = 600
  switch (drink){
    case 1:
      digitalWrite(8, HIGH);
      delay(ml * 60);
      digitalWrite(8, LOW);
      break;
    case 2:
      digitalWrite(9, HIGH);
      delay(ml * 60);
      digitalWrite(9, LOW);
      break;
    case 3:
      digitalWrite(10, HIGH);
      delay(ml * 60);
      digitalWrite(10, LOW);
      break;
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  Start();
  done=0;
  
  while (done == 0){
    Tela_Inicio();
    
    Menu_1();
    a=0;
    
    Menu_2();
    a=0;
    
    Motor();
    
    lcd.clear();
    ml = 0;
    
    delay(1000);
    
    Finaliza();
    a = 0;
  }  
}
  
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
