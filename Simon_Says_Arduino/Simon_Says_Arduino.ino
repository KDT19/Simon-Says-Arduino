// Definim pinii
const int ledPins[] = {2, 3, 4, 5, 6};
const int buttonPins[] = {8, 9, 10, 11, 12};
const int buzzerPin = 7;

// Frecvențele pentru butoane/LED-uri
const int tones[] = {262, 330, 392, 494, 523}; 

const int MAX_LEVEL = 10;
int sequence[MAX_LEVEL]; // Aici stocăm toată secvența de 10 pași
int currentLevel = 0;    // Nivelul curent (0 înseamnă 1 pas, 9 înseamnă 10 pași)
int gameSpeed = 500;     // Viteza inițială

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // Rămânem pe varianta stabilă
  }
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0)); 
}

void loop() {
  // La început de joc, generăm din start toți cei 10 pași
  if (currentLevel == 0) {
    for (int i = 0; i < MAX_LEVEL; i++) {
      sequence[i] = random(0, 5);
    }
    playStartSequence();
    delay(1000);
  }
  
  // 1. Placa îți arată secvența până la nivelul la care ai ajuns
  playSequence(currentLevel);
  
  // 2. Așteaptă să apeși butoanele
  if (!waitForPlayer(currentLevel)) {
    // Ai greșit
    gameOver(); 
  } else {
    // Ai apăsat corect toată secvența!
    delay(300);
    currentLevel++; 
    
    if (currentLevel >= MAX_LEVEL) {
      gameWon(); // Ai terminat jocul
    } else {
      levelUpAnimation(); // <-- Efectul nou de trecere la nivelul următor
      
      // Creștem puțin viteza pentru suspans
      if (gameSpeed > 200) {
        gameSpeed -= 25; 
      }
      delay(500); // Pauză înainte să înceapă noul nivel
    }
  }
}

// Funcția care îți arată luminile
void playSequence(int lvl) {
  for (int i = 0; i <= lvl; i++) {
    int currentLed = sequence[i];
    digitalWrite(ledPins[currentLed], HIGH); 
    tone(buzzerPin, tones[currentLed]);      
    delay(gameSpeed);
    
    digitalWrite(ledPins[currentLed], LOW);  
    noTone(buzzerPin);                       
    delay(250);                              
  }
}

// Funcția care citește și validează ce butoane apeși
bool waitForPlayer(int lvl) {
  for (int i = 0; i <= lvl; i++) {
    bool buttonPressed = false;
    int expectedButton = sequence[i];
    
    while (!buttonPressed) {
      for (int btn = 0; btn < 5; btn++) {
        
        if (digitalRead(buttonPins[btn]) == LOW) { // Buton apăsat
          delay(30); // Debounce
          
          if (digitalRead(buttonPins[btn]) == LOW) {
            
            digitalWrite(ledPins[btn], HIGH); 
            tone(buzzerPin, tones[btn]);      
            
            // Așteptăm să ridici degetul
            while (digitalRead(buttonPins[btn]) == LOW) {
              delay(10);
            }
            
            digitalWrite(ledPins[btn], LOW);
            noTone(buzzerPin);
            delay(50); // Debounce la ridicare
            
            if (btn == expectedButton) {
              buttonPressed = true;
              break; 
            } else {
              return false; // Buton greșit, GAME OVER
            }
          }
        }
      }
      delay(10); 
    }
  }
  return true; // Ai completat corect toți pașii
}

// EFECT NOU: Trecerea la un nivel nou
void levelUpAnimation() {
  for (int i = 0; i < 2; i++) { // Clipește de 2 ori
    for (int j = 0; j < 5; j++) digitalWrite(ledPins[j], HIGH);
    tone(buzzerPin, 800); // Sunet ascuțit/vesel
    delay(100);
    
    for (int j = 0; j < 5; j++) digitalWrite(ledPins[j], LOW);
    noTone(buzzerPin);
    delay(100);
  }
}

// Efect de Game Over
void gameOver() {
  tone(buzzerPin, 150); // Sunet grav
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) digitalWrite(ledPins[j], HIGH);
    delay(200);
    for (int j = 0; j < 5; j++) digitalWrite(ledPins[j], LOW);
    delay(200);
  }
  noTone(buzzerPin);
  currentLevel = 0; // Reset
  gameSpeed = 500;
  delay(1000);
}

// Efect de final (Nivelul 10 complet)
void gameWon() {
  int winTones[] = {262, 330, 392, 523, 392, 523}; 
  for (int i = 0; i < 6; i++) {
    tone(buzzerPin, winTones[i]);
    for (int j = 0; j < 5; j++) digitalWrite(ledPins[j], HIGH);
    delay(150);
    for (int j = 0; j < 5; j++) digitalWrite(ledPins[j], LOW);
    delay(150);
  }
  noTone(buzzerPin);
  currentLevel = 0; // Reset pentru un joc nou
  gameSpeed = 500;
  delay(2000);
}

// Efectul de pornire a jocului
void playStartSequence() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], HIGH);
    tone(buzzerPin, tones[i]);
    delay(100);
    digitalWrite(ledPins[i], LOW);
    noTone(buzzerPin);
  }
}