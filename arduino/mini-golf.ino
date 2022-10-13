/* EECS MINI GOLF */
/* EECS 183 WN 2019 */

#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POWER_POTENTIOMETER = 5;
const int ANGLE_POTENTIOMETER = 4;
const int BUTTON_PIN_NUMBER = 10;

// player's name is Tiger because he just won the Master's and is also the g.o.a.t.
const String GOAT = "Tiger";

//dimensions
const int WIDTH = 32;
const int HEIGHT = 16;

//course constants
const int COURSE_PAR = 38;
const int MAX_STROKES = 10;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void title_screen();
void splash();
void print_level(int level, int par);
void print_level_strokes(int strokes, int par);
void print_total_strokes(int strokes, int par);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);
const Color RAND(4, 0, 4);
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//BALL CLASS
/////////////////////////////////////////////////////////////////////////////////
class Ball {
  public:
    Ball() {
      x = 0;
      y = 0;
      dx = 0;
      dy = 0;
      is_hit = false;
    }
    //setters
    void set(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    void set_x(int x_arg) {
      x = x_arg;
    }
    void set_y(int y_arg) {
      y = y_arg;
    }
    void set_dx(int dx_arg) {
      dx = dx_arg;
    }
    void set_dy(int dy_arg) {
      dy = dy_arg;
    }
    void set_direction (int d_x, int d_y) {
      dx = d_x;
      dy = d_y;
    }
    
    //getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_dx() const {
      return dx;
    }
    int get_dy() const {
      return dy;
    }
    bool is_moving () const {
      return is_hit;
    }
    void draw() {
      matrix.drawPixel(x, y, PURPLE.to_333());
    }

    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
    }
    //resets values to defaults
    void reset() {
      x = 0;
      y = 0;
      dx = 0;
      dy = 0;
      is_hit = false;
    }
    
    ////////////////////
    // set angle function
    ////////////////////
    // sets angle according to right potentiometer 
    // uses intervals of 120 t
    void set_angle(int angle_value) {
      if (angle_value >= 0 && angle_value <= 120) {
          dx = 0;
          dy = 1;
        }
        else if (angle_value <= 240) {
          dx = 1;
          dy = 1;
        }
        else if (angle_value <= 360) {
          dx = 1;
          dy = 0;
        }
        else if (angle_value <= 480) {
          dx = 1;
          dy = -1;
        }
        else if (angle_value <= 600) {
          dx = 0;
          dy = -1;
        }
        else if (angle_value <= 720) {
          dx = -1;
          dy = -1;
        }
        else if(angle_value <= 840) {
          dx = -1;
          dy = 0;
        }
        else if (angle_value <= 960) {
          dx = -1;
          dy = 1;
        }
        else if (angle_value > 960) {
          dx = 0;
          dy = 1;
        }
    }

    void swing() {
      is_hit = true;
    }

    void stopped() {
      is_hit = false;
    }
    void border() {
      if (x + dx >= WIDTH - 1 || x + dx <= 0) {
        dx *= -1;
      }
      if (y + dy >= HEIGHT - 1 || y + dy <= 0) {
        dy *= -1;
      }
    }

    // collision detection for obstacles
    void in_obs(int obsX, int obsY, int rows, int cols) {
      if ((x + dx >= obsX && x + dx < obsX + cols) &&
          (y + dy >= obsY && y + dy < obsY + rows)) {
         //vertical   
         if (dx != 0 && dy == 0) {
          dx *= -1;
         }
         //horizontal
         else if (dx == 0 && dy != 0) {
          dy *= -1; 
         }
         //corners
         else if ((x + dx == obsX && y + dy == obsY) && dx == 1 && dy == 1) {
          dx = -1;
          dy = -1; 
         }
         else if ((x + dx == obsX && y + dy == obsY + rows - 1) && dx == 1 && dy == -1) {
          dx = -1;
          dy = 1;
         }
         else if ((x + dx == obsX + cols - 1 && y + dy == obsY) && dx == -1 && dy == 1) {
          dx = 1;
          dy = -1;
         }
         else if ((x + dx == obsX + cols - 1 && y + dy == obsY + rows - 1) && dx == -1 && dy == -1) {
          dx = 1;
          dy = 1;
         }
         //left hit   
         else if (x < obsX || x  > obsX + cols - 1) {
          dx *= -1;
         }
         else if (y < obsY || y - dy > obsY + rows - 1) {
          dy *= -1;
         }
      }
    }

    ////////////////////////////////////////////////////////////////
    //BALL MOVE FUNCTION
    ////////////////////////////////////////////////////////////////
    void move() {
      x += dx;
      y += dy;
    }

  private:
  int x;
  int y;
  int dx;
  int dy;
  bool is_hit;
}; // End Ball class
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//GUIDE CLASS
/////////////////////////////////////////////////////////////////////////////////
class Guide {
  public:
    Guide() {
      x = 0;
      y = 0;
      dx = 0;
      dy = 0;
      power = 0;
    }

    void set(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    void set_x(int x_arg) {
      x = x_arg;
    }
    void set_y(int y_arg) {
      y = y_arg;
    }
    void set_dx(int dx_arg) {
      dx = dx_arg;
    }
    void set_dy(int dy_arg) {
      dy = dy_arg;
    }
    void set_direction(int dx_arg, int dy_arg) {
      dx = dx_arg;
      dy = dy_arg;
    }
    
    //getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_dx() const {
      return dx;
    }
    int get_dy() const {
      return dy;
    }
    int get_power() const {
      return power;
    }

    ////////////////////
    //set angle function
    ////////////////////
    void set_angle(int angle_value) {
      if (angle_value >= 0 && angle_value <= 120) {
          dx = 0;
          dy = 1;
      }
      else if (angle_value <= 240) {
        dx = 1;
        dy = 1;
      }
      else if (angle_value <= 360) {
        dx = 1;
        dy = 0;
      }
      else if (angle_value <= 480) {
        dx = 1;
        dy = -1;
      }
      else if (angle_value <= 600) {
        dx = 0;
        dy = -1;
      }
      else if (angle_value <= 720) {
        dx = -1;
        dy = -1;
      }
      else if(angle_value <= 840) {
        dx = -1;
        dy = 0;
      }
      else if (angle_value <= 960) {
        dx = -1;
        dy = 1;
      }
      else if (angle_value > 960) {
        dx = 0;
        dy = 1;
      }
    }
    
    ////////////////////
    //set power function
    ////////////////////
    //reads in user power from potentiometer
    void set_power(int power_value) {
      if (power_value >= 0 && power_value < 200) {
        power = 1;
      }
      else if (power_value < 400) {
        power = 2;
      }
      else if (power_value < 600) {
        power = 3;
      }
      else if (power_value < 800) {
        power = 4;
      }
      else if (power_value <= 1023) {
        power = 5;
      }
    }

    //draws meter with size based on power(1-5 pixels)
    void draw() {
      int dx2 = dx;
      int dy2 = dy;
      for (int i = 0; i < power; i++) {
        matrix.drawPixel(x + dx2, y + dy2, RED.to_333());
        dx2 += dx;
        dy2 += dy;
      }
    }

    //erases guider
    void erase() {
      int dx2 = dx;
      int dy2 = dy;
      for (int i = 0; i < power; i++) {
        matrix.drawPixel(x + dx2, y + dy2, BLACK.to_333());
        dx2 += dx;
        dy2 += dy;
      }
    }

  private:
  int x;
  int y;
  int dx;
  int dy;
  int power;
}; // end Guide class
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//HAZARD CLASS
/////////////////////////////////////////////////////////////////////////////////
class Hazard {
  public:
    Hazard() {
      x = 0;
      y = 0;
      rows = 0;
      cols = 0;
      hazard_name = "";
    }

    //non-default constructor
    Hazard(int x_arg, int y_arg, int rows_arg, int cols_arg, String name_arg) {
      x = x_arg;
      y = y_arg;
      rows = rows_arg;
      cols = cols_arg;
      hazard_name = name_arg;
    }

    //sets private data members
    void initialize(int x_arg, int y_arg, int rows_arg, int cols_arg, String name_arg) {
      x = x_arg;
      y = y_arg;
      rows = rows_arg;
      cols = cols_arg;
      hazard_name = name_arg;
    }

    //getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_rows() const {
      return rows;
    }
    int get_cols() const {
      return cols;
    }
    String get_hazard_name() const {
      return hazard_name;
    }

    //draw hazard on board depending on name
    void draw() {
      if (hazard_name == "water") {
        for (int j = x; j < x + cols; j++) {
          for (int i = y; i < y + rows; i++) {
            matrix.drawPixel(j, i, BLUE.to_333());
          }
        }
      }
      if (hazard_name == "sand") {
        for (int j = x; j < x + cols; j++) {
          for (int i = y; i < y + rows; i++) {
            matrix.drawPixel(j, i, YELLOW.to_333());
          }
        }
      }
      if (hazard_name == "obstacle") {
        for (int j = x; j < x + cols; j++) {
          for (int i = y; i < y + rows; i++) { 
            matrix.drawPixel(j, i, WHITE.to_333());
          }
        }
      }
    }
    //resets member variables
    void reset() {
      x = 0;
      y = 0;
      rows = 0;
      cols = 0;
      hazard_name = "";
    }
    //erases hazard by drawing black 
    void erase() {
      for (int j = x; j < x + cols; j++) {
        for (int i = y; i < y + rows; i++) {
          matrix.drawPixel(j, i, BLACK.to_333());
        }
      }
    }

  private:
  int x;
  int y;
  int rows;
  int cols;
  String hazard_name;
}; // end Hazard class
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//TUNNEL CLASS
/////////////////////////////////////////////////////////////////////////////////
class Tunnel {
  public:
    Tunnel() {
      x_start = 0;
      y_start = 0;
      x_end = 0;
      y_end = 0;
      dx = 0;
      dy = 0;
    }
    //non-default
    Tunnel(int x_start_arg, int y_start_arg, int x_end_arg, int y_end_arg, int d_x, int d_y) {
      x_start = x_start_arg;
      y_start = y_start_arg;
      x_end = x_end_arg;
      y_end = y_end_arg;
      dx = d_x;
      dy = d_y;
    }
    
    //setter
    void initialize(int x_start_arg, int y_start_arg, int x_end_arg, int y_end_arg, int d_x, int d_y) {
      x_start = x_start_arg;
      y_start = y_start_arg;
      x_end = x_end_arg;
      y_end = y_end_arg;
      dx = d_x;
      dy = d_y;
    }
    
    //getters
    int get_x_start() const {
      return x_start;
    }
    int get_y_start() const {
      return y_start;
    }
    int get_x_end() const {
      return x_end;
    }
    int get_y_end() const {
      return y_end;
    }
    int get_dx() const {
      return dx;
    }
    int get_dy() const {
      return dy;  
    }
    //draw red for entrance and orange for exit
    void draw() {
      matrix.drawPixel(x_start, y_start, RED.to_333());
      matrix.drawPixel(x_end, y_end, ORANGE.to_333());
    }
    
    //reset function
    void reset() {
      x_start = 0;
      y_start = 0;
      x_end = 0;
      y_end = 0;
      dx = 0;
      dy = 0;
    }
    //erase function
    void erase() {
      matrix.drawPixel(x_start, y_start, BLACK.to_333());
      matrix.drawPixel(x_end, y_end, BLACK.to_333());
    }
    
  private:
  int x_start;
  int y_start;
  int x_end;
  int y_end;
  int dx;
  int dy;
}; // end Tunnel class
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//HOLE CLASS
/////////////////////////////////////////////////////////////////////////////////
class Hole {
  public: 
    Hole(){
      x = 0;
      y = 0;
    }
    //non-default
    Hole(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }

    //setter
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    
    //getters
    int get_x() const{
      return x;
    }
    int get_y() const {
      return y;
    }
    //draw hole as 2x2 green
    void draw() {
      matrix.drawPixel(x, y, GREEN.to_333());
      matrix.drawPixel(x + 1, y, GREEN.to_333());
      matrix.drawPixel(x, y + 1, GREEN.to_333());
      matrix.drawPixel(x + 1, y + 1, GREEN.to_333());
    }

    //reset function
    void reset() {
      x = 0;
      y = 0;
    }
    //erase function: draws black where hole was
    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x + 1, y, BLACK.to_333());
      matrix.drawPixel(x, y + 1, BLACK.to_333());
      matrix.drawPixel(x +1, y + 1, BLACK.to_333());
    }

  private:
  int x;
  int y;
}; // end Hole
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//LEVEL CLASS
/////////////////////////////////////////////////////////////////////////////////
class Level {
  public:
    Level() {
      par = 0;
    }
  
    Level(int par_arg) {
      par = par_arg;
    }

    void initialize(int par_arg) {
      par = par_arg;
    }

    int get_par() const {
      return par;
    } 

    void reset() {
      par = 0;
    }

    private:
    int par;
}; // end Level class
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//GOLFER CLASS
/////////////////////////////////////////////////////////////////////////////////
class Golfer {
  public:
    Golfer(){
      player = GOAT;
      total_strokes = 0;
      level_strokes = 0;
    }

    //getters
    String get_player() const {
      return player;
    }
    int get_total_strokes() const {
      return total_strokes;
    }
    int get_level_strokes() const {
      return level_strokes;
    }

    //increments both sets of strokes
    void swing() {
      total_strokes++;
      level_strokes++;
    }
    //resets level strokes
    void reset() {
      level_strokes = 0;
    }

  private:
  String player;
  int total_strokes;
  int level_strokes;
};
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//GAME CLASS
/////////////////////////////////////////////////////////////////////////////////
class Game {
  public:
    Game(){
      current_level = 1;
      swing_check = true;
      time = 0;
      //higher value = slower ball movement
      ball_speed = 0;
      ball_time = 0;      
    }
    /////////////////////////////////////////////////////////////////////////////////
    //SETUP FUNCTION
    /////////////////////////////////////////////////////////////////////////////////
    void setupGame() {
      print_level(current_level, pars[current_level - 1]);
      draw_level();
    }
    /////////////////////////////////////////////////////////////////////////////////
    //UPDATE FUNCTION
    /////////////////////////////////////////////////////////////////////////////////
    void update(int power_value, int angle_value, bool button_pressed) { 
      time = millis();

      /////////////////////////////////////////////////////////////////////////////////
      //PLAYER SWING MODE
      /////////////////////////////////////////////////////////////////////////////////      
      // continuously draw guide for player while ball is stopped and button is not pressed
      if (button_pressed == 0 && ball.is_moving() == 0 && swing_check == 1) {         
        guide.erase();
        guide.set_x(ball.get_x());
        guide.set_y(ball.get_y());
        guide.set_angle(angle_value);
        guide.set_power(power_value);
        guide.draw();
        ball.draw();
        draw_border();
        if (nWaters > 0) {
          draw_waters();
        }
        if (nSands > 0) {
          if (in_sand() == 0) {
            draw_sands();
          }
        }
        if (nObstacles > 0) {
          draw_obstacles();
        }
        draw_hole();             
      }
            
      /////////////////////////////////////////////////////////////////////////////////
      //BALL MOVE MODE
      /////////////////////////////////////////////////////////////////////////////////
      //when player presses button move the ball
      else if (button_pressed == 1 || ball.is_moving() == 1) {
        if (swing_check == 1) {
          golfer.swing();
          ball.swing();
          guide.erase();
          draw_border();
          if (nObstacles > 0) {
            draw_obstacles();
          }
          if (nWaters > 0) {
            draw_waters();
          }
          if (in_sand() == 0) {
            draw_sands();
          }
          //draw_tunnels();
          ball.set_angle(angle_value);
          set_ball_speed(guide.get_power());
          ball_time = time;
          swing_check = false;
        }

        //ball movement and speed reduction mechanics
        if (time >= ball_time) {
          ball.erase();
          ball.move();
          ball.draw();
          if (in_tunnel() == 1) {
            if (ball_speed >= 500) {
              ball_speed = 200;
            }
            else {
              ball_speed = ball_speed;
            }
          }
          else if (in_sand() == 1) {
            ball_speed *= 1.5;
          }
          else {
            ball_speed *= 1.1;
          }
          ball_time += ball_speed; 
          if (ball_speed >= 500) {
            ball.erase();
            ball.set(ball.get_x(), ball.get_y());
            ball.set_direction(0,0);
            ball.stopped();
            swing_check = true;
            ball.draw();
            delay(100);
          }
        }
        
        //check level's water hazards, sand traps, obstacles, tunnels, and hole
        if (in_water() == 1) {
          splash();
          golfer.swing();
          ball.erase();
          ball.reset();
          draw_level();  
          swing_check = true;
        }
        if (nObstacles > 0) {
          for (int i = 0; i < nObstacles; i++) {
            ball.in_obs(obstacles[i].get_x(), obstacles[i].get_y(), obstacles[i].get_rows(), obstacles[i].get_cols());
          }
        }
        ball.border();
        if (in_hole() == 1) {
            print_level_strokes(golfer.get_level_strokes(), pars[current_level - 1]);
            swing_check = true;
            reset_level();
            current_level++;
            if (current_level >= 10) {
              print_total_strokes(golfer.get_total_strokes(), COURSE_PAR);
              game_over();
            }
            else{
              setupGame();
            }
        }
      }

      //max stroke sequence
      if (golfer.get_level_strokes() >= 10) {
        print_level_strokes(golfer.get_level_strokes(), pars[current_level - 1]);
        swing_check = true;
        reset_level();
        current_level++;
        if (current_level >= 10) {
          print_total_strokes(golfer.get_total_strokes(), COURSE_PAR);
          game_over();
        }
        else { 
          setupGame();
        }
      }
    }
//////////////////////////////////////////////////////////////////////////////////////
    
  private:
    int current_level;
    unsigned long time;
    unsigned long ball_speed;
    unsigned long ball_time;
    Ball ball;
    Golfer golfer;
    Hole hole;
    Guide guide;
    int nTunnels;
    int nWaters;
    int nSands;
    int nObstacles;
    Tunnel tunnels[4];
    Hazard sands[3];
    Hazard waters[3];
    Hazard obstacles[6];
    Level level;
    int pars[9] = {3, 3, 3, 4, 5, 4, 5, 5, 6};
    bool swing_check;

    //draws border on edge of board
    void draw_border() {
      for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
          if (nTunnels > 0) {
            for (int i = 0; i < nTunnels; i++) {
              matrix.drawPixel(tunnels[i].get_x_start(), tunnels[i].get_y_start(), RED.to_333());
              matrix.drawPixel(tunnels[i].get_x_end(), tunnels[i].get_y_end(), ORANGE.to_333());
            }
          }
          if (y == 0 || y == HEIGHT - 1) {
            matrix.drawPixel(x, y, WHITE.to_333());           
          }
          else if (y > 0 && y < 15 && (x == 0 || x == 31)) {
            matrix.drawPixel(x, y, WHITE.to_333());         
          }
        }
      }
    }

    //sets ball speed based on power
    void set_ball_speed(int power) {
      if (power == 5) {
        ball_speed = 25;
      }
      else if (power == 4) {
        ball_speed = 50;
      }
      else if (power == 3) {
        ball_speed = 100;
      }
      else if (power == 2) {
        ball_speed = 200;
      }
      else if (power == 1) {
        ball_speed = 400;
      }
    }

/////////////////////////////////////////////////////////////////////////////////
//CHECKER FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////
    //checks if ball is hit in water hazard
    //resets ball to original location and penalizes player 1 stroke
    bool in_water() {
      bool result = false;
      if (nWaters > 0) {
        for (int i = 0; i < nWaters; i++) {
          if ((ball.get_x() >= waters[i].get_x()) &&
              (ball.get_x() < (waters[i].get_x() + waters[i].get_cols())) &&
              (ball.get_y() >= waters[i].get_y()) &&
              (ball.get_y() < (waters[i].get_y() + waters[i].get_rows()))) {
            result = true;      
          }
        } 
      }
      return result;
    }

    //tunnel mechanics
    bool in_tunnel() {
      bool result = false;
      if (nTunnels > 0) {
        for (int i = 0; i < nTunnels; i++) {
          if (((ball.get_x() + ball.get_dx()) == tunnels[i].get_x_start()) &&
              ((ball.get_y() + ball.get_dy()) == tunnels[i].get_y_start())) {
              ball.erase();
              ball.set_direction(tunnels[i].get_dx(), tunnels[i].get_dy());
              ball.set(tunnels[i].get_x_end(), tunnels[i].get_y_end());  
              ball.draw();
              result = true;
          }
        }
      }
      return result;
    }

    //sand trap mechanics
    bool in_sand() {
      bool result = false;
      if (nSands > 0) {
        for (int i = 0; i < nSands; i++) {
          if ((ball.get_x() >= sands[i].get_x()) &&
              (ball.get_x() < (sands[i].get_x() + sands[i].get_cols())) &&
              (ball.get_y() >= sands[i].get_y()) &&
              (ball.get_y() < (sands[i].get_y() + sands[i].get_rows()))) {
              result = true;  
              }
        }
      }
      return result;
    }

    bool in_hole() {
      bool result = false;
      if ((ball.get_x() == hole.get_x() && ball.get_y() == hole.get_y()) ||
          (ball.get_x() == (hole.get_x() + 1) && ball.get_y() == hole.get_y()) ||
          (ball.get_x() == hole.get_x() && ball.get_y() == (hole.get_y() + 1)) ||
          (ball.get_x() == (hole.get_x() + 1) && ball.get_y() == (hole.get_y() + 1))) {
          result = true;      
      }
      return result;
    }

/////////////////////////////////////////////////////////////////////////////////
//CLEAR LEVEL FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////
    //fills board black 
    void clear_board(){
      matrix.fillScreen(BLACK.to_333());  
    }

    void reset_level() {
      clear_board();
      level.reset();
      hole.reset();
      golfer.reset();
      ball.reset();
      for (int i = 0; i < nWaters; i++) {
        waters[i].reset();
      }
      for (int i = 0; i < nSands; i++) {
        sands[i].reset();
      }
      for (int i = 0; i < nObstacles; i++) {
        obstacles[i].reset();
      }
      for (int i = 0; i < nTunnels; i++) {
         tunnels[i].reset();
      }
    }
    
/////////////////////////////////////////////////////////////////////////////////
//DRAW LEVEL FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////
    //DRAW HOLE
    //////////////////////////////////
    void draw_hole() {
      if (current_level == 1) {
        hole.initialize(2, 7);
      }
      if (current_level == 2) {
        hole.initialize(29, 13);     
      }
      if (current_level == 3) {
        hole.initialize(3, 3);    
      }
      if (current_level == 4) {
        hole.initialize(29, 13);
      }
      if (current_level == 5) {
        hole.initialize(4, 11);
      }
      if (current_level == 6) {
        hole.initialize(26, 10);
      }
      if (current_level == 7) {
        hole.initialize(6, 12);
      }
      if (current_level == 8) {
        hole.initialize(4, 2);
      }
      if (current_level == 9) {
        hole.initialize(12, 6);
      }
      hole.draw();
    }
    
    //////////////////////////////////
    //DRAW WATER HAZARDS
    //////////////////////////////////
    //draws level's water hazards on board and sets chars to WATER
    void draw_waters() {
      if (current_level == 1) {
        nWaters = 0;
      }
      if (current_level == 2) {
        nWaters = 1;
        waters[0].initialize(4, 4, 8, 24, "water");
      }
      if (current_level == 3) {
        nWaters = 0;
      }
      if (current_level == 4) {
        nWaters = 3;
        waters[0].initialize(2, 7, 3, 3, "water");
        waters[1].initialize(6, 11, 2, 12, "water");
        waters[2].initialize(27, 13, 2, 2, "water");
      }
      if (current_level == 5) {
        nWaters = 2;
        waters[0].initialize(23, 10, 4, 4, "water");
        waters[1].initialize(11, 13, 2, 3, "water");
      }
      if (current_level == 6) {
        nWaters = 1;
        waters[0].initialize(15, 12, 3, 4, "water");
      }
      if (current_level == 7) {
        nWaters = 2;
        waters[0].initialize(8, 8, 3, 2, "water");
        waters[1].initialize(10, 12, 3, 4, "water");
      }
      if (current_level == 8) {
        nWaters = 0;
      }
      if (current_level == 9) {
        nWaters = 1;
        waters[0].initialize(14, 11, 3, 4, "water");
      }
      for (int i = 0; i < nWaters; i++) {
        waters[i].draw();
      }
    }

    //////////////////////////////////
    //DRAW SAND TRAPS
    //////////////////////////////////
    //draws sands and sets chars to SAND for each level
    void draw_sands() {
      if (current_level == 1) {
        nSands = 0;
      }
      if (current_level == 2) {
        nSands = 0;
      }
      if (current_level == 3) {
        nSands = 0;
      }
      if (current_level == 4) {
        nSands = 2;
        sands[0].initialize(18, 8, 5, 6, "sand");
        sands[1].initialize(10, 3, 3, 3, "sand");
      }
      if (current_level == 5) {
        nSands = 1;
        sands[0].initialize(12, 3, 5, 4, "sand"); 
      }
      if (current_level == 6) {
        nSands = 3;
        sands[0].initialize(12, 3, 2, 5, "sand");
        sands[1].initialize(21, 3, 4, 5, "sand");
        sands[2].initialize(22, 9, 4, 3, "sand");
      }
      if (current_level == 7) {
        nSands = 2;
        sands[0].initialize(28, 1, 14, 3, "sand"); 
        sands[1].initialize(1, 9, 2, 3, "sand");
      }
      if (current_level == 8) {
        nSands = 3;
        sands[0].initialize(28, 9, 4, 3, "sand");
        sands[1].initialize(5, 6, 4, 5, "sand");
        sands[2].initialize(13, 11, 2, 2, "sand");
      }
      if (current_level == 9) {
        nSands = 1;
        sands[0].initialize(14, 3, 3, 4, "sand");
      }
      for (int i = 0; i < nSands; i++) {
        sands[i].draw();
      }
    }

  //////////////////////////////////
  //DRAW OBSTACLES
  //////////////////////////////////
   //draws obstacles and sets board chars to '#'
   void draw_obstacles() {
    if (current_level == 1) {
      nObstacles = 4;
      obstacles[0].initialize(2, 4, 1, 2, "obstacle");
      obstacles[1].initialize(7, 0, 7, 1, "obstacle");
      obstacles[2].initialize(7, 9, 7, 1, "obstacle");
      obstacles[3].initialize(11, 6, 4, 1, "obstacle");
    }
    if (current_level == 2) {
      nObstacles = 0;
    }
    if (current_level == 3) {
      nObstacles = 1;
      obstacles[0].initialize(7, 0, 8, 18, "obstacle");
    }
    if (current_level == 4) {
      nObstacles = 1;
      obstacles[0].initialize(27, 7, 1, 3, "obstacle");
    }
    if (current_level == 5) {
      nObstacles = 4;
      obstacles[0].initialize(6, 8, 1, 3, "obstacle");
      obstacles[1].initialize(8, 9, 5, 1, "obstacle");
      obstacles[2].initialize(20, 0, 8, 1, "obstacle");
      obstacles[3].initialize(0, 8, 1, 3, "obstacle");
    }
    if (current_level == 6) {
      nObstacles = 1;
      obstacles[0].initialize(8, 0, 10, 1, "obstacle");
    }
    if (current_level == 7) {
      nObstacles = 2;
      obstacles[0].initialize(14, 0, 9, 2, "obstacle");
      obstacles[1].initialize(20, 7, 9, 2, "obstacle");
    }
    if (current_level == 8) {
      nObstacles = 6;
      obstacles[0].initialize(12, 0, 5, 1, "obstacle");
      obstacles[1].initialize(12, 4, 1, 10, "obstacle");
      obstacles[2].initialize(21, 9, 5, 1, "obstacle");
      obstacles[3].initialize(18, 5, 3, 1, "obstacle");
      obstacles[4].initialize(26, 7, 9, 2, "obstacle");
      obstacles[5].initialize(17, 13, 1, 4, "obstacle");
    }
    if (current_level == 9) {
      nObstacles = 4;
      obstacles[0].initialize(26, 6, 1, 6, "obstacle");
      obstacles[1].initialize(22, 10, 1, 6, "obstacle");
      obstacles[2].initialize(10, 0, 16, 2, "obstacle");
      obstacles[3].initialize(20, 0, 16, 2, "obstacle");    
    }
    for (int i = 0; i < nObstacles; i++) {
        obstacles[i].draw();
      }
   }

  //////////////////////////////////
  //DRAW TUNNELS
  //////////////////////////////////
   void draw_tunnels() {
      if (current_level == 1) {
        nTunnels = 1;
        tunnels[0].initialize(30, 0, 3, 0, 0, -1);
      }
      if (current_level == 2) {
        nTunnels = 0;
      }
      if (current_level == 3) {
        nTunnels = 0;
      }
      if (current_level == 4) {
        nTunnels = 1;
        tunnels[0].initialize(13, 0, 30, 0, 0, -1);
      }
      if (current_level == 5) {
        nTunnels = 0;
      }
      if (current_level == 6) {
        nTunnels = 1;
        tunnels[0].initialize(15, 0, 31, 11, -1, 0);
      }
      if (current_level == 7) {
        nTunnels = 1;
        tunnels[0].initialize(30, 0, 0, 6, 1, 0);
      }
      if (current_level == 8) {
        nTunnels = 1;
        tunnels[0].initialize(29, 15, 5, 15, 0, -1);
      }
      if (current_level == 9) {
        nTunnels = 4;
        tunnels[0].initialize(26, 15, 5, 15, 0, -1);
        tunnels[1].initialize(5, 0, 15, 15, 0, -1);
        tunnels[2].initialize(8, 0, 20, 7, -1, 0);
        tunnels[3].initialize(2, 0, 15, 0, 0, 1);
      }
      for (int i = 0; i < nTunnels; i++) {
        tunnels[i].draw();
      }  
   }


  //////////////////////////////////
  //DRAW BALL
  //////////////////////////////////
  void draw_ball() {
    if (current_level == 1) {
      ball.set(30, 8);
      ball.draw();
    }
    if (current_level == 2) {
      ball.set(1, 1);
      ball.draw();
    }
    if (current_level == 3) {
      ball.set(27, 2);
      ball.draw();
    }
    if (current_level == 4) {
      ball.set(2, 13);
      ball.draw();
    }
    if (current_level == 5) {
      ball.set(28, 12);
      ball.draw();
    }
    if (current_level == 6) {
      ball.set(4, 2);
      ball.draw();
    }
    if (current_level == 7) {
      ball.set(26, 12);
      ball.draw();
    }
    if (current_level == 8) {
      ball.set(14, 2);
      ball.draw();
    }
    if (current_level == 9) {
      ball.set(24, 1);
      ball.draw();
    }
  }
    
  //////////////////////////////////
  //DRAW LEVEL
  //////////////////////////////////
  void draw_level() {
    draw_obstacles();
    draw_border();
    draw_hole();
    draw_waters();
    draw_sands();
    draw_tunnels();
    draw_ball();
    if (current_level == 1) {       
       
      level.initialize(pars[0]);       
    }
    if (current_level == 2) {     
        
      level.initialize(pars[1]);    
    }
    if (current_level == 3) { 
                   
      level.initialize(pars[2]);       
    }
    if (current_level == 4) {      
            
      level.initialize(pars[3]);                  
    }
    if (current_level == 5) {      
          
      level.initialize(pars[4]);        
    }
    if (current_level == 6) {      
       
      level.initialize(pars[5]);     
    }
    if (current_level == 7) {
          
      level.initialize(pars[6]);      
    }
    if (current_level == 8) { 
        
      level.initialize(pars[7]);      
    }
    if (current_level == 9) {
                
      level.initialize(pars[8]);        
    }
  }   
};
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//SETUP & LOOP
/////////////////////////////////////////////////////////////////////////////////
//global variable that represents the Mini Golf game
Game game;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  title_screen();
  game.setupGame();
}

void loop() {
  // put your main code here, to run repeatedly:
  int power_value = analogRead(POWER_POTENTIOMETER);
  int angle_value = analogRead(ANGLE_POTENTIOMETER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

  game.update(power_value, angle_value, button_pressed);
}
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//PRINTERS
/////////////////////////////////////////////////////////////////////////////////

//displays title screen
void title_screen() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(LIME.to_333());
  matrix.setTextSize(1);  
  matrix.setCursor(0,0);
  matrix.print("EECS");
  matrix.setCursor(0,9);
  matrix.print("GOLF:");
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
}

//prints message if player hits it in the water
void splash() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(BLUE.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(0,0);
  matrix.print("WATER.....");
  delay(2000);
  matrix.fillScreen(BLACK.to_333());
}

//displays current level and par
void print_level(int level, int par) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(GREEN.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(0,0);
  matrix.print("HOLE");
  matrix.setCursor(26,0);
  matrix.print(level);
  matrix.setCursor(0,9);
  matrix.print("PAR ");
  matrix.setCursor(21,9);
  matrix.print(par);
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
}

//displays strokes taken to complete level
//displays golf term associated with amount of strokes
void print_level_strokes(int strokes, int par) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(WHITE.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(0,0);
  matrix.print("SCORE");
  matrix.setCursor(12,9);
  matrix.print(strokes);
  delay(3000);
  matrix.fillScreen(BLACK.to_333());
  if (strokes == 1) {
    matrix.setCursor(0,0);
    matrix.print("HOLE-");
    matrix.setCursor(0,9);
    matrix.print("IN-1!");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
  else if (par - strokes == 2) {
    matrix.setCursor(0,0);
    matrix.print("EAGLE");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
  else if (par - strokes == 1) {
    matrix.setCursor(0,0);
    matrix.print("BIRD-IE");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
  else if (par - strokes == 0) {
    matrix.setCursor(0,0);
    matrix.print("PAR");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
  else if (strokes - par == 1) {
    matrix.setCursor(0,0);
    matrix.print("BOGEY");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
  else if (strokes - par == 2) {
    matrix.setCursor(0,0);
    matrix.print("2x");
    matrix.setCursor(0,9);
    matrix.print("BOGEY");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
  else if (strokes == MAX_STROKES) {
    matrix.setCursor(0,0);
    matrix.print("YOURE");
    matrix.setCursor(0,9);
    matrix.print("BAD");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(0,0);
    matrix.print("HaHaHaHaHa");
    delay(3000);
    matrix.fillScreen(BLACK.to_333());
  }
}

//displays player score at end of game
void print_total_strokes(int strokes, int par) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(0,0);
  matrix.print("FINAL");
  matrix.setCursor(0,9);
  matrix.print("SCORE");
  delay(2000);
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(9,4);
  matrix.print(strokes - par);
  delay(2000);
  matrix.fillScreen(BLACK.to_333());
}

//displays congratulating message
void game_over() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(AQUA.to_333());
  matrix.setTextSize(1);
  matrix.setCursor(0,0);
  matrix.print("GREAT");
  matrix.setCursor(0,9);
  matrix.print("JOB");
  delay(2000);
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(0,0);
  matrix.print("TIGER");
  matrix.setCursor(0,9);
  matrix.print("U WON");
  delay(2000);
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(0,0);
  matrix.print("EECS");
  matrix.setCursor(0,9);
  matrix.print("GOLF!");
  //giggity
  delay(69696969);
  matrix.fillScreen(BLACK.to_333());
}
