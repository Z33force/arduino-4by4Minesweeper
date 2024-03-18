/* @4 by 4 minesweeper on keypad
|| @version 1.0
|| @author Luuk Schuttevaer
||
|| @description
|| | simple minesweeper with buzzer connection to make it playable for the blind.
|| #
*/
#include <Keypad.h>

int bombField [4][4];
int openField [4][4];
int timesPressed = 0;
int lastInput = 0;
int openedTiles = 0;
const int mines = 4; // 4 mijnen aan te passen voor ander aantal mijnen

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = { // dingetje van de library
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {8, 9, 10, 11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  reset();
  pinMode(3, OUTPUT); // buzzer pin
}
  
void loop(){
  int customKey = customKeypad.getKey(); // dingetje van de library

  if (customKey){
    customKey = charToInt(customKey);
    //Serial.println(customKey); // wanneer de keys niet reageren zoals verwacht
  	clickResponse(customKey); // geef de klik door aan de functie die dat afhandeld
    checkWin(); // kijk na elke klik of er al gewonnen is
  }
}

void fillBombs(){ // vul de bommen in functie
  int i = 0;
  int randomnumber;
  int randomRow;
  int randomColumn;
  while(i < mines){
    randomnumber = random(0,15);
    Serial.println(randomnumber);
    randomRow = (randomnumber / 4) % 4; // haal uit random cijfer welke rij (ook mogelijk door los rij en column random te maken maar dit vond ik wel lollig)
    Serial.println(randomRow);
    randomColumn = randomnumber - (randomRow * 4); // haal column uit random nummer
    Serial.println(randomColumn);
    Serial.println("");

    if(bombField[randomRow][randomColumn] != 10){ // zorg voor altijd 6 mijnen
      i++;
    }

    bombField[randomRow][randomColumn] = 10; // vul het vakje in als bom
  }
}

void fillNumbers() // vul de vakjes met het aantal aanliggende bommen functie
{
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      if(bombField[x][y] == 10){
        if(x-1 >= 0 && y-1 >= 0 && bombField[x-1][y-1] != 10){bombField[x-1][y-1] += 1;}
        if(x-1 >= 0 && bombField[x-1][y] != 10){bombField[x-1][y] += 1;}
        if(x-1 >= 0 && y+1 < 4 && bombField[x-1][y+1] != 10){bombField[x-1][y+1] += 1;}
        if(y-1 >= 0 && bombField[x][y-1] != 10){bombField[x][y-1] += 1;}
        if(y+1 < 4 && bombField[x][y+1] != 10){bombField[x][y+1] += 1;}
        if(x+1 < 4 && y-1 >= 0 && bombField[x+1][y-1] != 10){bombField[x+1][y-1] += 1;}
        if(x+1 < 4 && bombField[x+1][y] != 10){bombField[x+1][y] += 1;}
        if(x+1 < 4 && y+1 < 4 && bombField[x+1][y+1] != 10){bombField[x+1][y+1] += 1;}
      }
    }
  }
}

void printField(){ // print het bomveld in serial functie
  for(int x = 0; x < 4; x++){ 
    for(int y = 0; y < 4; y++){
      if (y != 3){
        Serial.print(bombField[x][y]);
        Serial.print(" ");
      }
      else{
        Serial.println(bombField[x][y]);
      }
    }
  }
}

void printOpenField(){ // print het klikveld in serial functie
  for(int x = 0; x < 4; x++){ 
    for(int y = 0; y < 4; y++){
      if (y != 3){
        Serial.print(openField[x][y]);
        Serial.print(" ");
      }
      else{
        Serial.println(openField[x][y]);
      }
    }
  }
}

void clickResponse(int input){ // reageer op een klik
  int clickRow = (input / 4) % 4;
  int clickColumn = input - (clickRow * 4);

  if(timesPressed == 0){ // alleen de eerste keer altijd toelaten
    openField[clickRow][clickColumn] = 1;
    if(bombField[clickRow][clickColumn] == 10){ // dood dus reset veld
      for (int r = 0; r < 15; r++){
        Serial.println("boom");
        digitalWrite(3, HIGH);
        delay(10);
        digitalWrite(3, LOW);
        delay(10);
      }
      reset();
    }
    else if (bombField[clickRow][clickColumn] < 10){ // geef aan wat er in het vakje zit
      for (int i = 0; i < bombField[clickRow][clickColumn]; i++){
        Serial.println("beep");
        digitalWrite(3, HIGH);
        delay(100);
        digitalWrite(3, LOW);
        delay(100);
      }
      if (bombField[clickRow][clickColumn] == 0){
        Serial.println("langebeep");
        digitalWrite(3, HIGH);
        delay(500);
        digitalWrite(3, LOW);
      }
      Serial.println(bombField[clickRow][clickColumn]);
      timesPressed = 1; // er is een keer geklikt
    }
  }

  else{ // na de eerste keer klikken kun je alleen je geopende vakjes zien
    if(input == lastInput){ // twee keer achter elkaar geklikt is openen
      openField[clickRow][clickColumn] = 1;
      if(bombField[clickRow][clickColumn] == 10){ // dood dus reset veld
        for (int r = 0; r < 15; r++){
          Serial.println("boom");
          digitalWrite(3, HIGH);
          delay(10);
          digitalWrite(3, LOW);
          delay(10);
        }
        reset();
      }
      
      else if (bombField[clickRow][clickColumn] < 10){ // geef aan wat er in het vakje zit
        for (int i = 0; i < bombField[clickRow][clickColumn]; i++){
          Serial.println("beep");
          digitalWrite(3, HIGH);
          delay(100);
          digitalWrite(3, LOW);
          delay(100);
        }
        if (bombField[clickRow][clickColumn] == 0){
          Serial.println("langebeep");
          digitalWrite(3, HIGH);
          delay(500);
          digitalWrite(3, LOW);
        }
        Serial.println(bombField[clickRow][clickColumn]);
      }
    }

    else{
      if (openField[clickRow][clickColumn] == 1){ // als veld al eerder is geopend mag er getoond worden wat het is
        if (bombField[clickRow][clickColumn] < 10){
          for (int i = 0; i < bombField[clickRow][clickColumn]; i++){
            Serial.println("beep");
            digitalWrite(3, HIGH);
            delay(100);
            digitalWrite(3, LOW);
            delay(100);
        }
          if (bombField[clickRow][clickColumn] == 0){
            Serial.println("langebeep");
            digitalWrite(3, HIGH);
            delay(500);
            digitalWrite(3, LOW);
          }
          Serial.println(bombField[clickRow][clickColumn]);
        }
      }
    }
  }
  lastInput = input;
}

int charToInt(int input){ // zet de chars van de library om tot een int van 0 tm 16
  if(input < 65){
    input -= 48;
  }
  else{
    input -= 55;
  }
  return input;
}

void reset(){ // functie om het spel te resetten
  for(int x = 0; x < 4; x++){ //reset velden
    for(int y = 0; y < 4; y++){
      bombField[x][y] = 0;
      openField[x][y] = 0;
    }
  }
  randomSeed(analogRead(0)); // random seed door noice
  fillBombs(); // vul bomveld
  fillNumbers(); // vul de aanliggende getallen
  timesPressed = 0; // zet aantal clicks terug naar 0
  printField(); // print het bomveld
  printOpenField(); // print het geopende veld
  openedTiles = 0; // reset aantal geopende tiles
}

void checkWin(){ // deze functie checkt of het spel gewonnen is
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      openedTiles += openField[x][y];
    }
  }
  if (openedTiles == 16 - mines){
    Serial.println("Gewonnen!!!");
    digitalWrite(3, HIGH);
      delay(2000);
      digitalWrite(3, LOW);
    reset();
  }
  openedTiles = 0;
}