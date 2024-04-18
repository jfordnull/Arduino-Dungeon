#include <TFT_HX8357.h>

/*  Bodmer display library; seemingly fast graphics primitives for ILI9488. Might not need this; we'll see */
TFT_HX8357 display = TFT_HX8357();

/* 16-bit Color codes */
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

/* Projection angles; for now, FOV = 60deg */
#define CELL_SIZE 64
#define WALL_HEIGHT 64
#define PROJECTION_WIDTH 480
#define PROJECTION_HEIGHT 320
#define ANGLE60 PROJECTION_WIDTH
#define ANGLE30 ANGLE60/2
#define ANGLE15 ANGLE30/2
#define ANGLE90 ANGLE30*3
#define ANGLE180 ANGLE90*2
#define ANGLE270 ANGLE90*3
#define ANGLE360 ANGLE180*2
#define ANGLE0 0
#define ANGLE5 ANGLE15/3
#define ANGLE10 ANGLE5*2

/* Trig tables */
float sin_table[ANGLE360+1];
float inv_sin_table[ANGLE360+1];
float cos_table[ANGLE360+1];
float inv_cos_table[ANGLE360+1];
float tan_table[ANGLE360+1];
float inv_tan_table[ANGLE360+1];

/* Step tables; to "step"/extrapolate intersection of raycast with gridlines */
float x_step[ANGLE360+1];
float y_step[ANGLE360+1];

/* Used to correct FOV distortion (beta wrt alpha) */
float beta_table[ANGLE60+1];

float arcToRad(float alpha){
  return((alpha*PI)/(float)ANGLE180);
}
 
/* Precompute all tables; one-time calculation; i represents our viewing angle (alpha) */
void computeTables(){
  for (int i=0; i<=ANGLE360; i++){
    float rad = arcToRad(i) + 0.0001f;
    sin_table[i] = sin(rad);
    inv_sin_table[i] = (1.0f/sin_table[i]);
    cos_table[i] = cos(rad);
    inv_cos_table[i] = (1.0f/cos_table[i]);
    tan_table[i] = tan(rad);
    inv_tan_table[i] = (1.0f/tan_table[i]);

    /* Coordinate system-> up = -Ya, down = +Ya, left = -Xa, right = +Xa*/
    y_step[i] = (float)(CELL_SIZE*tan_table[i]);
    x_step[i] = (float)(CELL_SIZE/tan_table[i]);

    /* Facing down */
    if (i >= ANGLE0 && i < ANGLE180){
      if (y_step[i]<0) y_step[i]=-y_step[i];
    }
    /* Facing up */
    else{
      if (y_step[i]>0) y_step[i]=-y_step[i];
    }
    /* Facing left */
    if (i >= ANGLE90 && i < ANGLE270){
      if (x_step[i]>0) x_step[i]=-x_step[i];
    }
    /* Facing right */
    else{
      if (x_step[i]<0) x_step[i]=-x_step[i];
    }
  }
  for (int i=-ANGLE30; i<=ANGLE30; i++){
    beta_table[i+ANGLE30] = (1.0f/cos(arcToRad(i)));
  }
}

/* Player */
int playerX = 200;
int playerY = 200;
int playerArc = ANGLE0;
/* Hard-coding for now, but projection plane distance = (Screen_width/2)/tan(FOV/2) */
int projectionPlaneDistance = 416;
/* Player height = wall_height/2 */
int playerHeight = 32;

/* Test map; consider other data structures */
char testMap[][9] ={"xoooooox",
                    "xoooooox",
                    "xoooooox",
                    "xoooooox",
                    "xoooooox",
                    "xoooooox",
                    "xoooooox",
                    "xoooooox",};


void setup() {
  display.init();
  display.setRotation(1);
  display.fillScreen(TFT_BLACK);
  computeTables();
}

void loop() {

}

