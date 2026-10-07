#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite spr = TFT_eSprite(&tft);

#define BUTTON_PIN 0 
// 0 =  Tear, 1 = Anger, 2 = Sparkles
int emotion = 0; 
int frame = 0;
bool lastButtonState = HIGH;

//  Color Palette
#define C_LIGHTBLUE 0x643F 
#define C_BROWN     0x2000
#define C_RED       0xC000 
#define C_YELLOW    0xFFE0 

void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  tft.init();
  tft.setRotation(1); 
  spr.createSprite(240, 135); 
}

void drawGiantSweat(int x, int y, int radius) {
  //  White Outline
  spr.fillCircle(x, y, radius + 4, TFT_WHITE);
  spr.fillTriangle(x - radius - 3, y, x + radius + 3, y, x, y - radius * 2.5 - 5, TFT_WHITE);
  
  //  Blue Drop
  spr.fillCircle(x, y, radius, C_LIGHTBLUE);
  spr.fillTriangle(x - radius + 1, y, x + radius - 1, y, x, y - radius * 2.5, C_LIGHTBLUE);
  

  spr.fillCircle(x + radius/2, y - radius/4, radius/3.5, TFT_WHITE);
  spr.fillCircle(x + radius/2.2, y + radius/2.5, radius/5, TFT_WHITE);
}


void drawAngerArm(int cx, int cy, float angleDeg, int size, uint16_t color, int thickness) {
  float rad = angleDeg * PI / 180.0;
  float radLeft = (angleDeg - 35) * PI / 180.0;
  float radRight = (angleDeg + 35) * PI / 180.0;


  int jX = cx + cos(rad) * (size * 0.4);
  int jY = cy + sin(rad) * (size * 0.4);

  int e1X = cx + cos(radLeft) * size;
  int e1Y = cy + sin(radLeft) * size;
  int e2X = cx + cos(radRight) * size;
  int e2Y = cy + sin(radRight) * size;

  for (float t = 0; t <= 1.0; t += 0.05) {
      spr.fillCircle(jX + (e1X - jX)*t, jY + (e1Y - jY)*t, thickness, color);
      spr.fillCircle(jX + (e2X - jX)*t, jY + (e2Y - jY)*t, thickness, color);
  }
  spr.fillCircle(jX, jY, thickness, color); // Round off the joint
}

void drawSparkle(int x, int y, int size) {
  spr.fillTriangle(x, y - size, x - size/3, y, x + size/3, y, C_YELLOW);
  spr.fillTriangle(x, y + size, x - size/3, y, x + size/3, y, C_YELLOW);
  spr.fillTriangle(x - size, y, x, y - size/3, x, y + size/3, C_YELLOW);
  spr.fillTriangle(x + size, y, x, y - size/3, x, y + size/3, C_YELLOW);
  spr.fillCircle(x, y, size/4, TFT_WHITE);
}

void loop() {
  bool currentBtn = digitalRead(BUTTON_PIN);
  if (currentBtn == LOW && lastButtonState == HIGH) {
    emotion = (emotion + 1) % 3; 
    frame = 0;
    delay(200); 
  }
  lastButtonState = currentBtn;

  spr.fillSprite(TFT_BLACK);

  //Tear
  if (emotion == 0) {
    int slide = (frame % 80) - 20;
    drawGiantSweat(150, 60 + (slide/2), 22);        
  }
  
  // Anger Mark
  else if (emotion == 1) {
    int throb = sin(frame * 0.2) * 8;
    int cx = 120, cy = 67;
    int size = 26 + throb;
    
    float angles[4] = {325, 55, 145, 235};
    
    for(int i = 0; i < 4; i++) {
        drawAngerArm(cx, cy, angles[i], size, C_BROWN, 7);
    }

    for(int i = 0; i < 4; i++) {
        drawAngerArm(cx, cy, angles[i], size, C_RED, 4);
    }
  }
  
  // Starts
  else if (emotion == 2) {
    int s1 = 45 + sin(frame * 0.15) * 12;
    int s2 = 28 + cos(frame * 0.1) * 8;
    
    drawSparkle(85, 45, s1);
    drawSparkle(160, 85, s2);
  }

  spr.pushSprite(0, 0);
  frame++;
  delay(30); 
}