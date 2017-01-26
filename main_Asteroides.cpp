#include <Grapic.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sstream>

// ODEH Majd
// 11608951

using namespace grapic;

using namespace std;

const int DIMWX = 500;
const int DIMWY = 500;
const int MAX_ASTEROID = 100;

bool Start = true;
bool Level = false;
bool Game = false;
bool Retour = false;
bool Restart = false;


struct Vec2{
    float x;
    float y;
};

struct Asteroid{
    Vec2 pos;
    int radius;
    bool movement;
};

typedef Asteroid lune;

struct Particle{
    Vec2 p;
    Vec2 v;
    Vec2 f;
    Vec2 gravity;
    float m;
    float angle;
};

struct World{
    Particle soucoupe;
    int score;
    int lives;
    float level;
    Asteroid tab[MAX_ASTEROID];
    int nb_asteroid;
    bool crash;
    bool arrived;
    bool started;
    lune moon;
    Image s;
    Image soucoupe_sans_feu;
    Image soucoupe_feu;
    Image a;
    Image m;
    Image star;
    Image explosion;
    Image background;
};

Vec2 operator+(Vec2 a, Vec2 b){
    Vec2 c;
    c.x = a.x+b.x;
    c.y = a.y+b.y;
    return c;
}

Vec2 operator-(Vec2 a, Vec2 b){
    Vec2 c;
    c.x = a.x-b.x;
    c.y = a.y-b.y;
    return c;
}

Vec2 operator-(Vec2 a, float b){
    Vec2 c;
    c.x = a.x-b;
    c.y = a.y-b;
    return c;
}

Vec2 operator-(Vec2 a){
    Vec2 c;
    c.x = -a.x;
    c.y = -a.y;
    return c;
}

Vec2 operator*(float a, Vec2 c){
    Vec2 d;
    d.x = a*c.x;
    d.y = a*c.y;
    return d;
}

Vec2 operator*(Vec2 c, float a){
    Vec2 d;
    d.x = a*c.x;
    d.y = a*c.y;
    return d;
}

Vec2 operator/(Vec2 c, int a){
    Vec2 d;
    d.x = c.x/a;
    d.y = c.y/a;
    return d;
}

Vec2 operator/(float a, Vec2 c){
    Vec2 d;
    d.x = a/c.x;
    d.y = a/c.y;
    return d;
}

Vec2 operator*(Vec2 a, Vec2 b){
    Vec2 c;
    c.x = a.x*b.x-a.y*b.y;
    c.y = a.x*b.y+a.y*b.x;
    return c;
}

// Procedure to initialize the World when starting the game
void initWorld(World &Monde, int nb){
    Monde.soucoupe.p = {DIMWX/2,28};
    Monde.soucoupe.v = {0,0};
    Monde.soucoupe.f = {0,0};
    Monde.soucoupe.m = 0.5;
    Monde.soucoupe.gravity = {0, Monde.soucoupe.m*(-9.81)};
    Monde.soucoupe.angle = 0;

    srand(time(NULL));
    Monde.moon.pos.x = 45+rand()%410;
    Monde.moon.pos.y = 45+410;
    Monde.moon.radius = 45;

    Monde.nb_asteroid=nb;
    for(int i = 0; i<Monde.nb_asteroid; i++){
        Monde.tab[i].radius = 30+rand()%40;
        Monde.tab[i].pos.x = 35+rand()%465;
        Monde.tab[i].pos.y = 110+rand()%265;
        int random=rand()%2;
        if(random == 1){
            Monde.tab[i].movement = true;
        }else{
            Monde.tab[i].movement = false;
        }
    }

    Monde.s = image("data/soucoupe.png");
    Monde.soucoupe_sans_feu = image("data/soucoupe.png");
    Monde.soucoupe_feu = image("data/soucoupe_feu.png");
    Monde.a = image("data/asteroid.png");
    Monde.m = image("data/moon.png");
    Monde.star = image("data/star.png");
    Monde.explosion = image("data/explosion.png");
    Monde.background = image("data/background.png");
    Monde.score = 0;
    Monde.lives = 3;
    Monde.level = 0;
    Monde.crash = false;
    Monde.arrived = false;
    Monde.started = false;
}

// Procedure to reinitialize the World after losing or winning
void reinitWorld(World &Monde){
    Monde.soucoupe.p = {DIMWX/2,28};
    Monde.soucoupe.v = {0,0};
    Monde.soucoupe.f = {0,0};
    Monde.soucoupe.m = 0.5;
    Monde.soucoupe.gravity = {0,Monde.soucoupe.m*(-9.81)};
    Monde.soucoupe.angle = 0;

    for(int i = 0; i<Monde.nb_asteroid; i++){
        Monde.tab[i].radius = 30+rand()%40;
        Monde.tab[i].pos.x = 35+rand()%465;
        Monde.tab[i].pos.y = 110+rand()%265;
        int random=rand()%2;
        if(random == 1){
            Monde.tab[i].movement = true;
        }else{
            Monde.tab[i].movement = false;
        }
    }

    srand(time(NULL));
    Monde.moon.pos.x = 45+rand()%410;
    Monde.moon.pos.y = 45+410;
    Monde.moon.radius = 45;
    Monde.s = Monde.soucoupe_sans_feu;
    Monde.crash = false;
    Monde.arrived = false;
    Monde.started = false;
}

// Calculate the Vector between the saucer and mouse position
Vec2 forceLauncher(World Monde){
    int x, y;
    Vec2 force;
    mousePos(x,y);
    force.x = x-Monde.soucoupe.p.x;
    force.y = y-Monde.soucoupe.p.y;
    return force;
}

// Calculate the Angle between the saucer and mouse position in order to use the angle to rotate the saucer image while moving the mouse
float angle(World Monde){
    int x, y;
    mousePos(x,y);
    Vec2 mouse = forceLauncher(Monde);
    float angle;
    if(mouse.x == 0 && mouse.y == 0){
        return angle = 90;
    }
    if(x >= Monde.soucoupe.p.x && y >= Monde.soucoupe.p.y){
        angle = mouse.y/sqrt((mouse.x*mouse.x)+(mouse.y*mouse.y))*90;
        return angle;
    }
    if(x < Monde.soucoupe.p.x && y > Monde.soucoupe.p.y){
        angle = 180-(mouse.y/sqrt((mouse.x*mouse.x)+(mouse.y*mouse.y))*90);
        return angle;
    }
    if(x <= Monde.soucoupe.p.x && y <= Monde.soucoupe.p.y){
        angle = 180-(mouse.y/sqrt((mouse.x*mouse.x)+(mouse.y*mouse.y))*90);
        return angle;
    }
    if(x > Monde.soucoupe.p.x && y < Monde.soucoupe.p.y){
        angle = 360+(mouse.y/sqrt((mouse.x*mouse.x)+(mouse.y*mouse.y))*90);
        return angle;
    }
}

// Calculate the Distance between two points
float distance2points(Vec2 a, Vec2 b){
    float dist;
    dist = sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
    return dist;
}

// Function calculate if the saucer reached the moon and return True or False
bool ovniWin(World Monde){
    if(distance2points(Monde.soucoupe.p, Monde.moon.pos) < Monde.moon.radius){
        return true;
    }else{
        return false;
    }
}

// Function calculate if the saucer crashed into an asteroid and return True or False
bool collision(World Monde, int i){
    if(distance2points(Monde.soucoupe.p, Monde.tab[i].pos) < Monde.tab[i].radius/2){
        return true;
    }else{
        return false;
    }
}

// Function calculate the Gravitational Force if the saucer is near an asteroid
Vec2 influence(World Monde){
    Vec2 dist;
    Vec2 somme = {0,0};
    for(int i = 0; i<Monde.nb_asteroid; i++){
        if(distance2points(Monde.soucoupe.p, Monde.tab[i].pos) < Monde.tab[i].radius*2 && Monde.started == true){
            dist.x = Monde.tab[i].pos.x-Monde.soucoupe.p.x;
            dist.y = Monde.tab[i].pos.y-Monde.soucoupe.p.y;
            somme = somme+(dist*0.04*Monde.tab[i].radius);
        }
    }
    return somme;
}

// Procedure for asteroids Movement
void movement(World &Monde, int i){
    int direction;
    if(Monde.tab[i].movement == true){
        direction = 1;
    }else{
        direction = -1;
    }
    Monde.tab[i].pos.x = Monde.tab[i].pos.x+Monde.level*direction*Monde.tab[i].radius;
    if(Monde.tab[i].pos.x < -Monde.tab[i].radius && Monde.tab[i].movement == false){
        Monde.tab[i].pos.y = 110+rand()%265;
        Monde.tab[i].radius = 30+rand()%40;
        Monde.tab[i].pos.x = DIMWX+Monde.tab[i].radius;
    }
    if(Monde.tab[i].pos.x > DIMWX+Monde.tab[i].radius && Monde.tab[i].movement==true){
        Monde.tab[i].pos.x = -Monde.tab[i].radius;
        Monde.tab[i].radius = 30+rand()%40;
        Monde.tab[i].pos.y = 110+rand()%265;
    }
}

// Procedure to draw the World
void draw(World &Monde){
    float theta = elapsedTime();
    int x, y;
    mousePos(x,y);
    image_draw(Monde.background,0,0,DIMWX,DIMWY);
    for(int i = 0; i < Monde.nb_asteroid; i++){
        image_draw(Monde.a, Monde.tab[i].pos.x-(Monde.tab[i].radius/2), Monde.tab[i].pos.y-(Monde.tab[i].radius/2), Monde.tab[i].radius,Monde.tab[i].radius);
        movement(Monde, i);
    }
    image_draw(Monde.m, Monde.moon.pos.x-45, Monde.moon.pos.y-45, 90, 90, theta*15, 0);
    image_draw(Monde.s, Monde.soucoupe.p.x-30, Monde.soucoupe.p.y-28, 60, 56, 90-angle(Monde), 0);

    color(208,247,15);
    fontSize(20);
    print(20,50,"SCORE:");
    print(100,50,Monde.score);
    image_draw(Monde.star, 130, 50);

    print(20,20,"Lives:");
    for(int i = 0; i < Monde.lives; i++){
        image_draw(Monde.soucoupe_sans_feu, 80+(i*35), 15, 25, 25);
    }
    // The line below display the saucer direction toward mouse position
    // line(Monde.soucoupe.p.x, Monde.soucoupe.p.y, x, y);
}

// Procedure to Update the World when the user is playing
void update(World &Monde){
    if(isMousePressed(SDL_BUTTON_LEFT)){
        Monde.started = true;
        Monde.soucoupe.f = forceLauncher(Monde);
        Monde.soucoupe.v = (0.0015*Monde.soucoupe.f)+(0.002*Monde.soucoupe.gravity)+0.0002*influence(Monde);
        Monde.soucoupe.p = Monde.soucoupe.p+Monde.soucoupe.v;
        Monde.s = Monde.soucoupe_feu;
    }else{
        Monde.soucoupe.v = 0.03*Monde.soucoupe.gravity+0.0002*influence(Monde);
        Monde.soucoupe.p = Monde.soucoupe.p+Monde.soucoupe.v;
        Monde.s = Monde.soucoupe_sans_feu;
    }
    if(Monde.soucoupe.p.y < 28){
        Monde.soucoupe.p.y = 28;
    }

    if(Monde.arrived == true){
        delay(1200);
        reinitWorld(Monde);
    }

    if(ovniWin(Monde)==true && !Monde.arrived){
        color(208,247,15);
        fontSize(36);
        print(190,220, "BRAVO");
        Monde.score++;
        Monde.arrived = true;
    }

    if(Monde.crash == true){
        delay(1200);
        reinitWorld(Monde);
    }
    if(Monde.lives == 0){
        Game = false;
        Restart = true;
    }
    for(int i = 0; i < Monde.nb_asteroid; i++){
        if(collision(Monde, i) && !Monde.crash){
            Monde.s = Monde.explosion;
            image_draw(Monde.s,Monde.soucoupe.p.x-30, Monde.soucoupe.p.y-28, 60, 56, 90-angle(Monde), 0);
            color(208,247,15);
            fontSize(36);
            print(110,220, "MISSION FAILED");
            Monde.lives--;
            Monde.crash = true;
        }
    }
}

// Game Welcome screen
void start(World Monde){
    Vec2 soucoupe = {DIMWX/2-30,350};
    float theta = elapsedTime();
    soucoupe.y = soucoupe.y+sin(theta)*40;
    image_draw(Monde.background,0,0,DIMWX,DIMWY);
    if(Retour == false){
        image_draw(Monde.soucoupe_feu, soucoupe.x, soucoupe.y);
        if(sin(theta) > 0.8){
            Retour = true;
        }
    }else{
        image_draw(Monde.soucoupe_sans_feu, soucoupe.x, soucoupe.y);
        if(sin(theta) < -0.8){
            Retour = false;
        }
    }
    image_draw(Monde.a, 75, 400, 60, 56, -theta*25, 0);
    image_draw(Monde.a, 50, 300, 40, 37, theta*15, 0);
    image_draw(Monde.a, 400, 350, 40, 37, -theta*15, 0);
    image_draw(Monde.a, 450, 450, 50, 47, theta*20, 0);
    color(208,247,15);
    fontSize(28);
    print(125,200, "ASTEROIDS GAME");
    color(150,150,20);
    fontSize(18);
    print(40,135, "Use the LEFT MOUSE BUTTON to control the saucer.");
    color(220,90,20);
    fontSize(28);
    print(50,70, "Press SPACE to start the Game.");
    color(220,90,20);
    fontSize(12);
    print(10,10, "MAJD ODEH");
    if(isKeyPressed(SDLK_SPACE)){
        Start = false;
        Level = true;
    }
}

// Choose a Level screen
void level(World &Monde){
    Monde.lives = 3;
    Monde.score = 0;
    Monde.started = false;
    Vec2 soucoupe = {DIMWX/2-30,350};
    float theta = elapsedTime();
    soucoupe.y = soucoupe.y+sin(theta)*40;
    image_draw(Monde.background, 0, 0, DIMWX, DIMWY);
    if(Retour == false){
        image_draw(Monde.soucoupe_feu, soucoupe.x, soucoupe.y);
        if(sin(theta) > 0.8){
            Retour = true;
        }
    }else{
        image_draw(Monde.soucoupe_sans_feu, soucoupe.x, soucoupe.y);
        if(sin(theta) < -0.8){
            Retour = false;
        }
    }
    image_draw(Monde.a, 75, 400, 60, 56, -theta*25, 0);
    image_draw(Monde.a, 50, 300, 40, 37, theta*15, 0);
    image_draw(Monde.a, 400, 350, 40, 37, -theta*15, 0);
    image_draw(Monde.a, 450, 450, 50, 47, theta*20, 0);
    color(208,247,15);
    fontSize(28);
    print(110,200, "CHOOSE GAME LEVEL");
    color(150,150,20);
    fontSize(24);
    print(160,150, "Easy (press A)");
    print(160,120, "Medium (press B)");
    print(160,90, "Hard (press C)");
    color(220,90,20);
    fontSize(12);
    print(10,10, "MAJD ODEH");

    if(isKeyPressed(SDLK_a)){
        Monde.level = 0.001;
        Level = false;
        Game = true;
    }
    if(isKeyPressed(SDLK_b)){
        Monde.level = 0.005;
        Level = false;
        Game = true;
    }
    if(isKeyPressed(SDLK_c)){
        Monde.level = 0.009;
        Level = false;
        Game = true;
    }
}

// Restart screen (this screen appears when the player lose the game)
void restart(World &Monde){
    Vec2 soucoupe = {DIMWX/2-30,350};
    float theta = elapsedTime();
    image_draw(Monde.background, 0, 0, DIMWX, DIMWY);
    image_draw(Monde.a, 75, 400, 60, 56, -theta*25, 0);
    image_draw(Monde.a, 30, 330, 40, 37, theta*15, 0);
    image_draw(Monde.a, 430, 380, 40, 37, -theta*15, 0);
    image_draw(Monde.a, 200, 350, 70, 70, 0, 0);
    image_draw(Monde.soucoupe_feu, 250, 330, 60, 56, -45, 0);
    image_draw(Monde.explosion, 220, 330, 50, 50, 0, 0);
    color(208,247,15);
    fontSize(28);
    print(100,240, "YOU LOST THE GAME!");
    fontSize(24);
    color(220,90,20);
    print(140,190, "YOUR SCORE IS : ");
    print(330,190, Monde.score);
    color(150,150,20);
    fontSize(24);
    print(70,120, "Press SPACE to restart the game");
    print(160,70, "Press Q to Exit");
    color(220,90,20);
    fontSize(12);
    print(10,10, "MAJD ODEH");

    if(isKeyPressed(SDLK_SPACE)){
        Restart = false;
        Level = true;
    }
}

// Main Program
int main(int , char** )
{
    int astroids = 10;      //Asteroids Number
    bool stop = false;
	winInit("Asteroids", DIMWX, DIMWY);
    setKeyRepeatMode(false);
    backgroundColor( 0, 35, 57);

    World Monde;
    initWorld(Monde, astroids);

	while( !stop ){
        winClear();

        if(Start == true){
            start(Monde);
        }
        if(Level == true){
            level(Monde);
        }
        if(Game == true){
            draw(Monde);
            update(Monde);
        }
        if(Restart == true){
            restart(Monde);
        }
        stop = winDisplay();
    }
    winQuit();
	return 0;
}
