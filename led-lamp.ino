#include <Adafruit_NeoPixel.h>


#define PIN 6
#define LAMPS 144
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

struct Spark {
  int pos;
  int vel;
  int acc;
  int duration;
  int time;
  int hueDeg;
  int sat;
  int val;
  int hueDeg2;
  int sat2;
  int val2;
  int hueDeg3;
  int sat3;
  int val3;
};
#define NUM_SPARKS 20
Spark sparks[NUM_SPARKS];

int degTo16(int32_t deg) {
  return deg * 65536 / 360;
}

int percentTo255(int32_t percent) {
  return percent * 255 / 100;
}

int lerp(int min, int max, int t, int range) {
  return min + (int32_t)(max - min) * t / range;
}

int euclideanModulo(int n, int m) {
  return ((n % m) + m) % m;
}

float euclideanModulo(float n, float m) {
  return fmod((fmod(n, m) + m), m);
}

int lerp360(int min, int max, int t, int range) {
  if (abs(min - max) > 180) {
    max = max + 180;
  }
  min = euclideanModulo(min, 360);
  max = euclideanModulo(max, 360);
  return lerp(min, max, t, range);
}

// an API that uses ints. This makes most math much easier, no warnings, no errors
// for example. Imaging you have an object that moves down the strip coveving 10 pixels
// 
//   for (int i = -10; i < strip.numPixels(); ++i) {
//     strip.fill(0xFF0000, i, 10);
//     strip.show()
//     delay(100);
//   }
//
// That code will fail. With i < 0 it gets promoted a large positive number
// which means the first 10 frames will show nothing and then suddently at
// 10 led object will appera
//
// Similary you might do something like
//
//   if (pos < strip.numPixels() - otherPos) {
//      ...
//
// If pos or otherPos are ever negative this line will fail
// because `strip.numPixels()` is a uint16_t and so the entire
// expression gets converted to unsigned math.
//
// Of course if you turn on warnings you'll see the issue and you
// can work around it but why? Just use ints in the first place!

struct LED {
  static void init() {
    strip.begin();
    strip.setBrightness(255);    // overall brightness can be 0 to 255
    strip.show();                // Initialize all pixels to 'off'
  };
  static void clear() {
    strip.clear();
  };
  static void setPixel(int i, uint32_t color) {
    strip.setPixelColor(i, color);
  };
  static void fill(uint32_t color, int start, int len) {
    if (start < 0) {
      len += start;
      start = 0;
    }
    if (len > 0) {
      strip.fill(color, start, len);
    }    
  };
  static void setPixelCoverage(float start, float coverage, uint32_t color) {
    LED::setPixel(start, LED::ColorRGB(
        ((color >> 16) & 0xFF) * coverage,
        ((color >>  8) & 0xFF) * coverage,
        ((color >>  0) & 0xFF) * coverage));
    
  }
  static void fill(uint32_t color, float start, float len) {
    /*
          |   .    |       |   .    |
          |   <----|-------|--->    |
     */
    float end = start + len;
    float startCoverage = (len >= 1.0f ? 1.0f : euclideanModulo(end, 1.0f)) - euclideanModulo(start, 1.0f);
    LED::setPixelCoverage(start, startCoverage, color);
    LED::fill(color, (int)ceilf(start), (int)(floorf(end) - ceilf(start)));
    if (len > 1.0) {
      LED::setPixelCoverage(start + len, euclideanModulo(end, 1.0f), color);
    }
  }
  // hue in 0 to 360
  static uint32_t ColorHSV(int hueDegrees, int sat, int val) {
    return LED::ColorHSV16(degTo16(hueDegrees), sat, val);
  };
  // hue in Neopixel's 0 to 65535
  static uint32_t ColorHSV16(uint16_t hue16, int sat, int val) {
    return strip.ColorHSV(hue16, sat, val);
  };
  static uint32_t ColorRGB(int r, int g, int b) {
    return strip.Color(r, g, b);
  };
  static void show() {
    strip.show();
  };
  static const int numPixels = LAMPS;
  private:
    static Adafruit_NeoPixel strip;
};

Adafruit_NeoPixel LED::strip = Adafruit_NeoPixel(LAMPS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  memset(&sparks, 0, sizeof(sparks));
  LED::init();
}

void clearDelay(int duration) {
  LED::clear();
  LED::show();
  delay(duration);
}

/* Ideas:

x fix breakout particles
O do sakura better
x make sparkles more and pick random hue +/1 15
x fix hue lerp in particles to be circular
O fix traffic light to 1 light
x make inchwork brighter in middle when short
O grow inchwork as it eats?
O add acceleration / deceleration to traffic?
O asteroids?
x pong with paddles getting closer?
O count 1, 2, 3 down strip?
x fix drips not waiting for last drip
O shooting stars? (is that same as fireworks just without explosion?) maybe it's a roman candle and like reverse drip it builds then shoots
O drop ball with bounce (similar to dip)
x jump up platforms
O sand clock
x fix binary clock bomb so it always fits
O make snow collect?
x fade in breakout and/or traffic (and fade it out)
O pulsing power pellet for pacman?
O lava lamp

x heartbeat
O snow (pile up?)
O rain (splash?)
x fireflies
x ripples
x twinkles
x fire
x fireworks
O cherry blossoms
x bomb (sparker tip)  
x time bomb (binary clock, bomb gets brighter) 
x breakout
x traffic (with stop light, cars are 2 or 3 lights)
x fuseBomb
x inchworm
x drips

*/

void loop() {
  #if 1
//  testSparks();
    //lavaLamp();
  #else
    pacman();          clearDelay(1000);
    ripples();         clearDelay(1000);
    timeBomb();        clearDelay(1000);
    bounce();          clearDelay(1000);
    heartbeat(30);     clearDelay(1000);
    snow();            clearDelay(1000);
    inchworm();
    inchworm();        clearDelay(1000);
    fireworks(30);     clearDelay(1000);
    drips();           clearDelay(1000);
    fireflies();       clearDelay(1000);
    mario();           clearDelay(1000);
    fuseBomb();        clearDelay(1000);
    breakout();        clearDelay(1000);
    cherryBlossoms();  clearDelay(1000);
    fire();            clearDelay(1000);
    traffic(120 * 30); 
    twinkles();        clearDelay(1000);
    pong();            clearDelay(1000);
  #endif
}

void addSpark(int pos, int vel, int acc, int duration, int hueDeg, int sat, int val, int hueDeg2, int sat2, int val2) {
  addSpark2(pos, vel, acc, duration, hueDeg, sat, val, hueDeg + (hueDeg2 - hueDeg) / 2, sat + (sat2 - sat) / 2, val + (val2 - val) / 2, hueDeg2, sat2, val2);
}

void addSpark2(int pos, int vel, int acc, int duration, int hueDeg, int sat, int val, int hueDeg2, int sat2, int val2, int hueDeg3, int sat3, int val3) {
  int minTime = sparks[0].time;
  int ndx = 0;
  for (int i = 1; i < NUM_SPARKS; ++i) {
    if (sparks[i].time < minTime) {
      minTime = sparks[i].time;
      ndx = i;
    }
  }

  Spark& spark = sparks[ndx];
  spark.pos = pos * 128;
  spark.vel = vel;
  spark.acc = acc;
  spark.duration = duration;
  spark.time = duration;
  spark.hueDeg = hueDeg;
  spark.sat = sat;
  spark.val = val;
  spark.hueDeg2 = hueDeg2;
  spark.sat2 = sat2;
  spark.val2 = val2;
  spark.hueDeg3 = hueDeg3;
  spark.sat3 = sat3;
  spark.val3 = val3;
}

bool processSparks() {
  bool busy = false;
  for (auto& spark : sparks) {
    if (spark.time <= 0) continue;
    busy = true;
    int hue;
    int sat;
    int val;
    int halfDuration = spark.duration / 2;
    if (spark.time < halfDuration) {
      hue = lerp360(spark.hueDeg3, spark.hueDeg2, spark.time, halfDuration);
      sat = lerp(spark.sat3, spark.sat2, spark.time, halfDuration);
      val = lerp(spark.val3, spark.val2, spark.time, halfDuration);
    } else {
      int time = spark.time - halfDuration;
      hue = lerp360(spark.hueDeg2, spark.hueDeg, time, halfDuration);
      sat = lerp(spark.sat2, spark.sat, time, halfDuration);
      val = lerp(spark.val2, spark.val, time, halfDuration);
    }
    LED::setPixel(spark.pos >> 7, LED::ColorHSV16(degTo16(hue), sat, val));  
    spark.vel += spark.acc;
    spark.pos += spark.vel;
    --spark.time;
  }
  return busy;
}

void testSparks() {
  int t = 0;
  for(;;++t) {
    LED::clear();
    LED::setPixel(0, 0x800000);
    LED::setPixel(1, 0x008000);
    if (t % 40 == 0) { //random(8) == 0) {
      addSpark(
        LED::numPixels / 2,     // pos
        400,                 // vel
        -15,                 // acc, 
        50,                  // duration
        60,                  // hue,
        255,                 // sat,
        255,                 // val
        60,                  // hue2
        255,                 // sat2
        0);                   // val2
    }
    processSparks();
    LED::show();
    delay(5);
  }  
}

void lavaLamp() {
  struct Blob {
    float start;
    float size;
    float growDir;
    float vel;
    int hue;
    int targetHue;
  };
  Blob blobs[3];
  float minSize = 3;
  float maxSize = 20;

  for (auto& blob : blobs) {
    blob.hue = random(360);
    blob.targetHue = blob.hue;
    blob.start = random(1, LED::numPixels - 7);
    blob.size = random(3, 6);
    blob.vel = random(-150, 150) / 2000.0f;
    blob.growDir = random(2) ? -1 : 1;
  }
  float growVel = 0.01;

  while (true) {
    LED::clear();
    for (auto& blob : blobs) {
      blob.start += blob.vel;
      if (blob.start < 0.0f) {
        blob.vel = random(1, 30) / 100.0f;
      } else if ((blob.start + blob.size) >= LED::numPixels) {
        blob.vel = -random(1, 30) / 100.0f;
      }
      blob.size += blob.growDir * growVel;
      if (blob.size <= minSize) { blob.growDir = 1; }
      else if (blob.size >= maxSize) { blob.growDir = -1; }
      blob.hue = lerp(blob.hue, blob.targetHue, 1, 30);
      LED::fill(LED::ColorHSV(blob.hue, 255, 255), blob.start, blob.size);
    }
    LED::show();
    delay(5);
  }
  
}

void bounce() {
  struct Ball {
    float pos;
    float vel;
    int hue;
  };
  Ball balls[20];
  for(auto& ball : balls) {
    ball.pos = -1;
  }
  int numBalls = 24;
  int numActive = 0;
  float gravity = -0.05f;
  float fudge = 0.8f;
  while (numBalls || numActive) {
    numActive = 0;
    LED::clear();
    for (auto& ball : balls) {
      if (ball.pos >= 0) {
        ++numActive;
        ball.vel += gravity;
        ball.pos += ball.vel;
        if (ball.pos < 0) {
          ball.pos = -ball.pos;
          ball.vel = -ball.vel * fudge;
          if (ball.vel < 1.0f) {
            ball.pos = -1;
          }
        }
        LED::setPixel(ball.pos, LED::ColorHSV(ball.hue, 255, 255));
      }
    }
    
    if (numBalls && numActive < (int)ARRAY_SIZE(balls) && random(200) == 0) {
      --numBalls;
      for (auto& ball : balls) {
        if (ball.pos < 0) {
          ball.pos = LED::numPixels;
          ball.vel = 0;
          ball.hue = random(360);
          break;
        }
      }
    }
    LED::show();
    delay(5);
  }
}

void mario() {
  struct Platform {
    int pos;
    bool enemy;
  };
  Platform platforms[15];
  int p = 0;
  for (auto& platform : platforms) {
    p += random(10, 20);
    platform.pos = p;
    platform.enemy = random(4) == 0;
  }
  for (int i = 0; i < 100; ++i) {
    LED::clear();
    float s = sin(i / 100.f * PI * 0.5f);
    for (auto& platform : platforms) {
      LED::setPixel(platform.pos * s, 0xFFFFFF);
    }
    LED::show();
    delay(5);
  }
  
  float pos = 0.0f;
  int state = 0;
  int wait = 100;
  float vel = 0.0f;
  float accel = -0.05f;
  int platformTargetNdx = 0;
  int frame = 0;
  while (state != 0 || pos < LED::numPixels) {
    ++frame;
    LED::clear();
    switch (state) {
      case 0: // waiting
        --wait;
        if (wait <= 0) {
          state = 1;
          vel = 1.5f;
        }
        break;
      case 1: // jumping
        vel += accel;
        pos += vel;
        Platform& platform = platforms[platformTargetNdx];
        int targetHeight = platform.pos;
        if (vel < 0 && pos <= targetHeight) {
          if (platform.enemy) {
            platform.enemy = false;
            addSpark(
              platform.pos,
              300,   // vel
              -10,   // acc
              120,    // dur
              120,   // h
              255,   // s
              255,   // v
              120,
              255,
              255);
          }
          pos = targetHeight + 1;
          state = 0;
          wait = random(80, 200);
          ++platformTargetNdx;
        }
        break;
    }

    for (auto& platform : platforms) {
      LED::setPixel(platform.pos, 0xFFFFFF);
      if (platform.enemy) {
        int val = sin(frame * 0.04f + platform.pos) * 0.5 * 128 + 127;
        LED::fill(LED::ColorHSV(120, 255, val), platform.pos + 2, 3);
      }
    }
    LED::fill(0x0000FF, pos, 3.0f);
    LED::fill(0xFF0000, pos + 5.0f, 3.0f);
    processSparks();
    LED::show();
    delay(5);
  }
  for (auto& platform : platforms) {
    addSpark(
      platform.pos,  // pos
      0,         // vel
      -2,        // acc
      150,       // dur
      0,         // h
      0,         // s
      255,       // v
      0,
      0,
      128);
  }
  bool hasSparks = true;
  while (hasSparks) {
    LED::clear();
    hasSparks = processSparks();
    LED::show();
    delay(5);
  }
}

void pong() {
  int top = LED::numPixels - 1;
  int bottom = 0;
  int ball = LED::numPixels / 2;
  int dir = random(2) ? -1 : 1;

  int frame = 0;
  while (top > bottom) {
    ++frame;
    LED::clear();

    if (frame % 16 == 0) {
      --top;
      ++bottom;
    }

    ball += dir;
    if (ball <= bottom) {
      dir = 1;
    }
    if (ball >= top) {
      dir = -1;
    }

    LED::setPixel(top, 0xFFFFFF);
    LED::setPixel(bottom, 0xFFFFFF);
    float sat = powf(1.0f - (top - bottom) / (float)LED::numPixels, 5.0f);
    LED::setPixel(ball, LED::ColorHSV(0, 255 * sat, 255));
    
    LED::show();
    delay(5);
  }
  
  explosion(LED::numPixels / 2);
}
void drips() {
  int numDrips = 10;
  int duration = 0;
  int timer = 0;
  int splashTimer = 0;
  bool haveSparks = true;
  while (numDrips || timer || haveSparks) {
    LED::clear();
    if (timer <= 0 && numDrips) {
      duration = random(1000, 3000);
      timer = duration;
      --numDrips;
    }

    if (timer > 0) {
      int max_val = lerp(255, 0, timer, duration);
      LED::setPixel(LED::numPixels - 1, LED::ColorHSV(200, 255, max_val));
      timer -= 5;
      if (timer <= 0) {
        addSpark(
          LED::numPixels - 1, // pos
          0,                  // vel
          -1,                 // acc
          200,                // duration
          200,                // hue
          255,
          255,
          200,
          255,
          255);
        splashTimer = 180;
      }
    }

    if (splashTimer > 0) {
      splashTimer -= 1;
      if (splashTimer == 0) {
        for (int i = 0; i < 6; ++i) {
          addSpark2(
            0,    // pos
            random(50, 100),    // vel
            -1,                 // acc
            200,                // duration
            200,                // hue
            255,
            128,
            200,
            255,
            128,
            200,
            255,
            0);
        }
      }
    }
    
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }
}

void fireflies() {
  int numFireflies = 30;
  int pos = -1;
  int dir = 1;
  int duration = 0;
  while (numFireflies) {
    LED::clear();
    if (duration == 0) {
      if (random(100) == 0) {
        pos = random(20, LED::numPixels - 20);
        duration = 20;
        dir = random(2) ? -1 : 1;
      }
    } else {
      pos += dir;
      LED::setPixel(pos, LED::ColorHSV(50, 200, 255));
      --duration;
      if (duration == 0) {
        --numFireflies;
      }
    }
    LED::show();
    delay(5);
  }  
}

void twinkles() {
  int numTwinkles = 60;
  int baseHue = random(360);
  bool haveSparks = true;
  while (numTwinkles || haveSparks) {
    LED::clear();
    if (numTwinkles && random(15) == 0) {
      --numTwinkles;
      int hue = baseHue + random(30);
      addSpark2(
          random(LED::numPixels),     // pos
          0,                // vel
          0,                // acc, 
          100,              // duration
          hue,              // hue,
          255,              // sat,
          128,              // val,
          hue,              // hue2,
          128,              // sat2,
          255,              // val2,
          hue,
          255,
          0);
    }
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }  
}

void ripples() {
  int numRipples = 60;
  bool haveSparks = true;
  int frame = 0;
  while (numRipples || haveSparks) {
    ++frame;
    LED::clear();
    if (numRipples && frame % 10 == 0) {
      --numRipples;
      int hue = random(200, 220);
      int pos = random(LED::numPixels);
      for (int i = 0; i < 6; ++i) {
        int vel = random(25, 45) * (random(2) ? -1 : 1);
        addSpark2(
            pos,     // pos
            vel,              // vel
            vel / 50,         // acc, 
            40 ,              // duration
            hue,              // hue,
            255,              // sat,
            128,              // val,
            hue,               // hue2,
            255,              // sat2,
            255,              // val2,
            hue,
            255,
            0);
      }
    }
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }  
}

void breakout() {
  int maxBricks = 6;
  int numBricks = maxBricks;
  bool haveSparks = true;
  int brickSize = 6;
  int pos = 0;
  int vel = 70;

  for (numBricks = 0; numBricks < maxBricks; ++numBricks) {
    int target = LED::numPixels - (numBricks + 1) * brickSize;
    {
      int hue = lerp(0, 360, numBricks, maxBricks);
      for (int p = 0; p < target; p += 2) {
        LED::clear();
        LED::fill(LED::ColorHSV(hue, 255, 255), p, brickSize);
        for (int i = 0; i < numBricks; ++i) {
          int hue = lerp(0, 360, i, maxBricks);
          LED::fill(LED::ColorHSV(hue, 255, 255), LED::numPixels - (i + 1) * brickSize, brickSize);
        }        
        LED::show();
        delay(5);
      }
    }
  }
  delay(1000);
  while (numBricks || haveSparks) {
    LED::clear();
    for (int i = 0; i < numBricks; ++i) {
      int hue = lerp(0, 360, i, maxBricks);
      LED::fill(LED::ColorHSV(hue, 255, 255), LED::numPixels - (i + 1) * brickSize, brickSize);
    }

    int bottomBrickPos = LED::numPixels - (numBricks + 1) * brickSize;
    pos += vel;
    int effectivePos = pos >> 7;
    if (vel > 0) {
      if (effectivePos >= bottomBrickPos) {
        vel = -vel;
        --numBricks;
        for (int i = 0; i < 6; ++i) {
          int vel = random(50, 100); //(random(2) ? 1 : -1);
          int hue = lerp(0, 360, numBricks, maxBricks);
          addSpark2(
            effectivePos,     // pos
            vel,              // vel
            -2,               // acc, 
            100,              // duration
            hue,              // hue,
            255,              // sat,
            255,              // val,
            hue,              // hue2,
            255,              // sat2,
            255,              // val2,
            hue,
            255,
            0);
           
        }
      }
    } else {
      if (effectivePos <= 0) {
        vel = -vel;
      }
    }
    LED::fill(LED::ColorHSV(60, 0, 255), effectivePos, 2);
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }
}

void timeBomb() {
  int pos = random(LED::numPixels / 8, LED::numPixels / 2);
  int speed = random(70, 120);
  int size = 3;
  int space = 5;
  int maxCount = 128;
  int count = maxCount;
  int ticker = 0;
  int flash = 0;
  while (count) {
    LED::clear();
    int c = count;
    for (int b = 0; b < 8; ++b) {
      if (c & 1) {
        LED::fill(0xFF0000, pos + (size + space) * (b + 1), size);
      }
      c = c >> 1;
    }
    //flash += maxCount - count;
    ++flash;
    int val = lerp(64, 255, maxCount - count - 1, maxCount);
    int hue = lerp(240, 60, maxCount - count - 1, maxCount);
    LED::fill(LED::ColorHSV(hue, 255, val), pos - 6, 6);
    LED::show();
    delay(5);
    ticker += 5;
    if (ticker >= speed) {
      ticker -= speed;
      --count;
    }
  }
  explosion(pos);
}

void explosion(int pos) {
    for (int i = 0; i < NUM_SPARKS; ++i) {
    int maxVel = random(100, 300);
    int vel = random(0, maxVel) * (random(2) ? 1 : -1);
    int hue = random(30, 60);          
    addSpark2(
         pos,                // pos
         vel,                // vel
         -vel / 80,          // acc, 
         200,                // duration
         hue,                // hue,
         0,                  // sat,
         255,                // val,
         hue,                // hue2,
         255,                // sat2,
         255,                // val2,
         hue,                // hue3
         255,                // sat3,
         0);                 // val3
  }
  bool haveSparks = true;
  while (haveSparks) {
    LED::clear();
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }

}
void heartbeat(int beats) {
  int pos = LED::numPixels * 2 / 3;
  for (int b = 0; b < beats; ++b) {
    for (int p = 0; p < 2; ++p) {
      int duration = p == 0
         ? 45
         : 90;
      int maxLen = p == 0
         ? 20
         : 15;
      for (int i = 0; i < duration; ++i) {
        LED::clear();
        int val = lerp(128, 64, i, duration);
        int len = lerp(maxLen, 10, i, duration);
        LED::fill(LED::ColorHSV16(0, 255, val), pos - len, len * 2);
        LED::show();
        delay(5);
      }
    }
  }
}

void traffic(int frames) {
  int stopPos = random(LED::numPixels / 4, LED::numPixels * 3 / 4);

  struct Car {
    int pos;
    int dir;
    int hue;
    int pause;
  };
  Car cars[6];
  for (auto& car : cars) {
    car.dir = random(2) ? -1 : 1;
    car.pos = car.dir == 1
      ? -random(10, 160)
      : random(LED::numPixels + 10, LED::numPixels + 160);
    car.hue = degTo16(random(360));
    car.pause = 0;
  }

  int car_len = 6;
  int frame = 0;  
  bool haveCars = true;
  while (haveCars) {
    ++frame;
    haveCars = false;
    LED::clear();

    uint16_t f = frame % 2000;
    int light = f > 1400
       ? 0
       : f > 1200
          ? 1
          : 2;
    int flicker = frame & 4 ? 255 : 200;
    LED::setPixel(stopPos    , LED::ColorHSV16(degTo16(0), 255, light == 0 ? flicker : 128));
    LED::setPixel(stopPos - 1, LED::ColorHSV16(degTo16(0), 255, light == 0 ? flicker : 128));
    LED::setPixel(stopPos - 2, LED::ColorHSV16(degTo16(0), 255, light == 0 ? flicker : 128));
    LED::setPixel(stopPos - 5, LED::ColorHSV16(degTo16(60), 255, light == 1 ? flicker : 128));
    LED::setPixel(stopPos - 6, LED::ColorHSV16(degTo16(60), 255, light == 1 ? flicker : 128));
    LED::setPixel(stopPos - 7, LED::ColorHSV16(degTo16(60), 255, light == 1 ? flicker : 128));
    LED::setPixel(stopPos - 10, LED::ColorHSV16(degTo16(130), 255, light == 2 ? flicker : 128));
    LED::setPixel(stopPos - 11, LED::ColorHSV16(degTo16(130), 255, light == 2 ? flicker : 128));
    LED::setPixel(stopPos - 12, LED::ColorHSV16(degTo16(130), 255, light == 2 ? flicker : 128));

    for (auto& car : cars) {
      bool blocked = light != 2 && (
         (car.dir ==  1 && car.pos == stopPos - 16) ||
         (car.dir == -1 && car.pos - car_len == stopPos + 3));
      if (!blocked) {
        for (auto& other : cars) {
          if (&car != &other && car.dir == other.dir) {
            if (car.dir > 0) {
              if (car.pos == other.pos - (car_len + 3)) {
                blocked = true;
              }
            } else {
              if (car.pos - car_len - 3 == other.pos) {
                blocked = true;
              }
            }
          }
        }
      }
      bool out_of_bounds = false;
      if (blocked) {
        car.pause = 100;
      } else if (car.pause) {
        --car.pause;
      } else {
        if (frame % 6 == 0) {
          if (car.dir == 1) {
            ++car.pos;
            out_of_bounds = car.pos > LED::numPixels + car_len;
          } else {
            --car.pos;
            out_of_bounds = car.pos < -car_len;
          }
        }
      }
      if (out_of_bounds) {
        if (frame < frames) {
          car.dir = random(2) ? -1 : 1;
          car.pos = car.dir == 1
            ? -random(10, 160)
            : random(LED::numPixels + 10, LED::numPixels + 160);
          car.hue = degTo16(random(360));
          car.pause = 0;
        }
      } else {
        haveCars = true;
      }
      LED::fill(LED::ColorHSV16(car.hue, 255, blocked ? 128 : 200), car.pos - car_len, car_len);
    }
    
    LED::show();
    delay(5);
  }
}

void fuseBomb() {
  // fade in
  uint32_t white;
  for(int br = 10; br < 100; ++br) {
    white = LED::ColorRGB(br - 10, br, br + 20);
    for(int i=0; i < LED::numPixels; ++i) {
        LED::setPixel(i, i % 4 == 0 ? white : 0); 
    }
    LED::show();
    delay(5);
  }
  int frame = 0;
  bool haveSparks = true;
  for (int height = LED::numPixels - 1; height >= 0 || haveSparks;) {
    for(int i = 0; i < LED::numPixels; ++i) {
       LED::setPixel(i, (i % 4u == 0 && i < height) ? white : 0); 
    }
    frame++;
    LED::setPixel(height, LED::ColorHSV16(degTo16(random(30, 60)), 255, 255));
    LED::setPixel(height, LED::ColorHSV16(degTo16(random(30, 60)), 255, 255));
    LED::setPixel(height + 1, 0);
    if (height > 0) {
      LED::fill(0xFF0000, 0, 3);
      if (random(5) == 0) {
        int vel = random(100, 400) * (random(2) ? 1 : -1);
        addSpark(
          height,    // pos
          vel,                // vel
          -vel / 25,           // acc, 
          25,                 // duration
          60,                 // hue,
          255,                // sat,
          255,                // val,
          30,                 // hue2,
          255,                // sat2,
          128);                // val2,
      }
    }
    if (frame % 8 == 0) {
      --height;
      if (height == 0) {
        for (int i = 0; i < NUM_SPARKS; ++i) {
          int maxVel = random(100, 300);
          int vel = random(0, maxVel);
          int hue = random(30, 60);          
          addSpark2(
               0,                  // pos
               vel,                // vel
               -vel / 80,           // acc, 
               200,                // duration
               hue,                // hue,
               0,                  // sat,
               255,                // val,
               hue,                // hue2,
               255,                // sat2,
               255,                // val2,
               hue,                // hue3
               255,                // sat3,
               0);                 // val3
        }
      }
    }
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }
  delay(1000);
}

void fireworks(int numFireworks) {
  struct Firework {
    int height;
    int hue;
    int duration;
    int time;
  };
  Firework fireworks[3];

  memset(&fireworks, 0, sizeof(fireworks));
  for (auto& firework : fireworks) {
    firework.time = -1;
  }

  bool hasSparks = true;
  bool hasFireworks = true;
  while (numFireworks || hasSparks || hasFireworks) {
    LED::clear();
    hasFireworks = false;
    for (auto& firework : fireworks) {
      if (firework.time < 0) {
        if (numFireworks && random(500) == 0) {
          firework.height = random(LED::numPixels / 2, LED::numPixels);
          firework.hue = random(360);
          firework.duration = random(200, 400);
          firework.time = firework.duration;
          --numFireworks;
        }
      } else {
        hasFireworks = true;
        int pos = sin((firework.duration - firework.time) / (float)firework.duration * PI * 0.5f) * firework.height;
        int hue = firework.hue;
        LED::setPixel(pos, LED::ColorHSV16(degTo16(hue), 255, 255));
        if (random(10) == 0) {
          int h = random(hue, hue + 20);
          addSpark(
            pos,                // pos
              0,                // vel
             -2,                // acc, 
            25,                 // duration
            h,                  // hue,
            255,                // sat,
            255,                // val,
            h,                  // hue2,
            255,                // sat2,
            40);                // val2,
         };
         --firework.time;
         if (firework.time < 0) {
           int numSparks = random(10, 20);
           int maxVel = random(100, 300);
           for (int i = 0; i < numSparks; ++i) {
             int vel = random(-maxVel, maxVel);
             hue = random(hue - 20, hue + 20);
             addSpark2(
               pos,                // pos
               vel,                // vel
               -vel / 80,          // acc, 
               100,                // duration
               hue,                // hue,
               0,                  // sat,
               255,                // val,
               hue,                // hue2,
               255,                // sat2,
               255,                // val2,
               hue,                // hue3
               255,                // sat3,
               0);                 // val3
          }
        }
      }
    }
    hasSparks = processSparks();
    LED::show();
    delay(5);
  }
}

void forwardSetPixel(int pos, uint32_t color) {
  LED::setPixel(pos, color);  
}

void reverseSetPixel(int pos, uint32_t color) {
  LED::setPixel(reverseIndex(pos), color);
}

int forwardIndex(int pos) {
  return pos;
}

int reverseIndex(int pos) {
  return LED::numPixels - pos - 1;
}

typedef void (*PixelSetFn)(int pos, uint32_t color);
typedef int (*IndexFn)(int pos);

void inchworm() {
  int head = 0;
  int tail = -10;
  bool reverse = random(2);
  PixelSetFn setPixelColor = reverse ? reverseSetPixel : forwardSetPixel;
  IndexFn index = reverse ? reverseIndex : forwardIndex;

  // last food is sentinal
  int foodPos[4] = {1000, 1000, 1000, 1000};
  int numFoods = random(1, ARRAY_SIZE(foodPos) - 1);
  int top = random(40, LED::numPixels - 20);
  int length = 10;
  for (int i = 0; i < numFoods; ++i) {
    foodPos[i] = top;
    top += random(top / 4, top - 5);
  }

  int target = 0;
  bool go = true;
  while (tail < LED::numPixels) {
    LED::clear();
    for (int i = target; i < numFoods; ++i) {
      setPixelColor(foodPos[i], LED::ColorHSV16(degTo16(360 - 30), 255, 255)); //lerp(0, 255, foodAmount[i], foodEnergy)));
    }
    for (int i = tail; i < head - 1; ++i) {
      int range = head - tail;
      float zeroToOne = sin((i - tail) / (float)range * PI) * 0.5f + 0.5f;
      int val = pow(zeroToOne, 5.0f) * 255;
      //val = lerp(val, 64, range, length);
      setPixelColor(i, LED::ColorHSV16(degTo16(110), 200, val));
    }
    setPixelColor(head - 1, LED::ColorHSV16(degTo16(0), 255, 255));
    setPixelColor(head, LED::ColorHSV16(degTo16(0), 255, 255));
    if (head + 2 == foodPos[target]) {
      addSpark2(
        index(foodPos[target++]),  // pos
        0,                  // vel
        0,                  // acc, 
        100,                // duration
        330,                // hue,
        255,                // sat,
        255,                // val,
        330,                // hue2,
        0,                  // sat2,
        255,                // val2,
        330,                // hue3
        255,                // sat3,
        0);                 // val3      
      while(processSparks()) {
        LED::show();
        delay(5);    
      }
      length += 10;
    } else if (go) {
      ++head;
      if (head - tail > length + 10) {
        go = false;
        LED::show();
        delay(150);
      }
    } else {
      ++tail;
      if (head - tail < length) {
        go = true;
        LED::show();
        delay(150);
      }
    }
    LED::show();
    delay(100);
  }
}

void fire() {
  int maxHeight = LED::numPixels / 2;
  int height = 0;
  int frames = 1000;
  bool haveSparks = true;
  for(int t = 0; t < frames || haveSparks; ++t) {
    LED::clear();
    if (t & 8) {
      if (t > frames - height * 8) {
        height -= 1;
      } else {
        height = min(maxHeight, height + 1);
      }
    }
    int effectiveHeight = random(height - 3, height + 3);
    for (int p = 0; p < effectiveHeight; ++p) {
      int hue = lerp(0, degTo16(60), p, effectiveHeight);
      int sat = random(180, 255);
      int val = random(64, 128);
      sat = lerp(255, sat, p, effectiveHeight);
      LED::setPixel(p, LED::ColorHSV16(hue, sat, val));
    }
    if (random(15) == 0) {
      addSpark(
        effectiveHeight,    // pos
        200,                // vel
        -7,                 // acc, 
        25,                 // duration
        60,                 // hue,
        255,                // sat,
        255,                // val,
        0,                  // hue2,
        255,                // sat2,
        40);                // val2,
    }
    haveSparks = processSparks();
    LED::show();
    delay(5);
  }
}

void precipition(int totalDrops, int minSpeed, int maxSpeed, int minHue, int maxHue, int sat, int value) {
  #define NUM_DROPS 20
  struct RainDrop {
    int pos;
    int speed;
    uint32_t color;
  };
  RainDrop drops[NUM_DROPS];
  LED::clear();
  for(auto& drop : drops) {
    drop.pos = random(LED::numPixels, LED::numPixels * 2) * 128;
    drop.speed = random(minSpeed, maxSpeed);
    drop.color = LED::ColorHSV16(random(minHue, maxHue), sat, value);
  }
  int cnt;
  do {
    cnt = 0;
    for (auto & drop : drops) {
      int pixel = drop.pos >> 7;
      LED::setPixel(pixel, 0);
      LED::setPixel(pixel + 1, 0);
    }
    for (auto & drop : drops) {
      drop.pos -= drop.speed;
      int pixel = drop.pos >> 7;
      LED::setPixel(pixel, drop.color);
      LED::setPixel(pixel + 1, drop.color);
      if (drop.pos < 0) {
        if (totalDrops) {
          --totalDrops;
          drop.pos = random(LED::numPixels, LED::numPixels + 30) * 128;
        } else {
          drop.speed = 0;
          ++cnt;
        }
      }
    }
    LED::show();
    delay(5);
  } while(cnt < NUM_DROPS);
}

/*
void rain() {
  precipition(
      600,               // totalDrops
      400,                // minSpeed
      800,                // maxSpeed
      degTo16(219 - 40),  // minHue
      degTo16(219 +  0),  // maxHue
      percentTo255(30),   // saturation
      percentTo255(20));  // value
}
*/

void cherryBlossoms() {
  precipition(
      50,                 // totalDrops
      5,                  // minSpeed
      10,                 // maxSpeed
      degTo16(0),         // minHue
      degTo16(0),         // maxHue
      percentTo255(30),   // saturation
      percentTo255(100)); // value
}

void snow() {
  precipition(
      100,                // totalDrops
      10,                 // minSpeed
      20,                 // maxSpeed
      degTo16(219 - 40),  // minHue
      degTo16(219 +  0),  // maxHue
      percentTo255(17),   // saturation
      percentTo255(100)); // value
}


void pacman() {
  uint32_t pac = LED::ColorHSV(60, 255, 255);
  uint32_t blinky = LED::ColorHSV(0, 255, 255);
  uint32_t pinky = LED::ColorHSV(0, 128, 255);
  uint32_t inky = LED::ColorHSV(180, 255, 255);
  uint32_t clyde = LED::ColorHSV(30, 255, 255);
  uint32_t scared = LED::ColorHSV(240, 255, 255);


  // fade in dots
  for (int val = 0; val < 170; ++val) {
    for (int i = 0; i < LED::numPixels; ++i) {
      LED::setPixel(i, i % 4 == 0 ? LED::ColorHSV(0, 0, val) : 0);
    }
    LED::show();
    delay(5);
  }

  delay(1500);

  // eat dots
  for (int i = 0; i <= LED::numPixels; ++i) {
    setLedAndTurnOfNeighbors(i, pac);
    LED::show();
    delay(30);
  }

  delay(1500);
  
  // get chased
  int duration = 30 * 144 / 5;
  for (int i = 0; i <= duration; ++i) {
    setLedAndTurnOfNeighbors(lerp(LED::numPixels, -84, i, duration), pac);
    setLedAndTurnOfNeighbors(lerp(LED::numPixels + 10,  -80, i, duration), pinky);
    setLedAndTurnOfNeighbors(lerp(LED::numPixels + 20,  -60, i, duration), inky);
    setLedAndTurnOfNeighbors(lerp(LED::numPixels + 30,  -40, i, duration), blinky);
    setLedAndTurnOfNeighbors(lerp(LED::numPixels + 40,  -20, i, duration), clyde);
    LED::show();
    delay(5);
  }

  delay(1500);

  // chase
  int ghosts[] = {0 * 128, -8 * 128, -16 * 128, -24 * 128};
  int pos = -50 * 128;
  while(pos / 128 < LED::numPixels) {
    int pause = 5;
    LED::clear();
    pos += 42;
    LED::setPixel(pos / 128, pac);
    for (auto& ghost : ghosts) {
      if (ghost / 128 < LED::numPixels) {
        ghost += 30;
        bool eaten = ghost / 128 == pos / 128;
        LED::setPixel(ghost / 128, eaten ? LED::ColorHSV(180, 255, 255) : scared);
        if (eaten) {
          ghost = LED::numPixels * 128;
          pause = 1000;
        }
      }
    }
    LED::show();
    delay(pause);
  }

  delay(1500);
}

void setLedAndTurnOfNeighbors(int pos, uint32_t color) {
  LED::setPixel(pos, color); 
  LED::setPixel(pos + 1, 0);
  LED::setPixel(pos - 1, 0);
}
