#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#define M_PI 3.14159265358979323846
using namespace std;

// can be used for circle rx==ry
void utils_ellipse(float cx, float cy, float rx, float ry, int segm = 100) {
    glBegin(GL_POLYGON);
    for (int i=0; i<segm; i++) {
         float angle=2.0f*M_PI*float(i)/float(segm);
        float x=cx+rx*cosf(angle);
        float y=cy+ry*sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void utils_render_bitmap_string(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c=string; *c!='\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

class Meteor {
private:
    float x, y, size;
    int sides;
    vector<float> radii;
public:
    Meteor(float x, float y, float size, int sides): x(x), y(y), size(size), sides(sides) {
        if (sides<3) sides=3;
        radii.resize(sides);
        for (int i=0; i<sides; ++i)
            radii[i]=size*(0.7f+static_cast<float>(rand())/RAND_MAX*0.6f);
    }

    void draw() {
        float angleStep=2.0f*M_PI/sides;
        glBegin(GL_POLYGON);
        for (int i = 0; i < sides; i++) {
            float angle=i*angleStep;
            float vx=x+radii[i]*cos(angle);
            float vy=y+radii[i]*sin(angle);
            glVertex2f(vx, vy);
        }
        glEnd();
    }

    void fall(float sp) { y-=sp; }

    float get_x() const { return x; }
    float get_y() const { return y; }
    float get_size() const { return size; }
};

class SpaceShip {
private:
    float x, y;
    float speed;

public:
    bool booster=false;
    SpaceShip(float x, float y, float speed): x(x), y(y), speed(speed) {}

    void draw() {
        glColor3f(0.3255, 0.0588, 0.1137);
        body();
        wings(1);
        wings(-1);
    }

    void body() {
        glColor3f(0.3255, 0.0588, 0.1137);
        utils_ellipse(x+0, y+30, 14, 14);

        glBegin(GL_POLYGON);
            glVertex2f(x+(-14), y+(-20));
            glVertex2f(x+14, y+(-20));
            glVertex2f(x+14, y+30);
            glVertex2f(x+(-14), y+30);
        glEnd();

        glBegin(GL_POLYGON);
            glVertex2f(x+(-14), y+(-20));
            glVertex2f(x+(-20), y+(-30));
            glVertex2f(x+(20), y+(-30));
            glVertex2f(x+14, y+(-20));
        glEnd();

        glColor3f(0.2471, 0.0157, 0.0627);
        utils_ellipse(x+0, y+(-30), 20, 4);

        if (booster) {
            glColor3f(1, 0, 0);
            boost(1, 0);
            glColor3f(1, 0.678, 0);
            boost(0.75, -7.5);
            glColor3f(1, 0, 0);
        }
    }

    void wings(int d) {
        glColor3f(0.4196, 0.5255, 0.6471);
        glBegin(GL_POLYGON);
            glVertex2f(x+d*(-40), y+5);
            glVertex2f(x+d*(-40), y+(-20));
            glVertex2f(x+d*(-14), y+(-15));
            glVertex2f(x+d*(-14), y+20);
        glEnd();

        glColor3f(0.4431, 0.6392, 0.8667);
        glBegin(GL_POLYGON);
            glVertex2f(x+d*(-40), y+10);
            glVertex2f(x+d*(-40), y+(-25));
            glVertex2f(x+d*(-42), y+(-25));
            glVertex2f(x+d*(-42), y+10);
        glEnd();
    }

    void boost(float s, float dy) {
        glBegin(GL_POLYGON);
            glVertex2f(x+(-20)*s, y+(-30)*s+dy);
            glVertex2f(x+(20)*s, y+(-30)*s+dy);
            glVertex2f(x+(13)*s, y+(-44)*s+dy);
            glVertex2f(x+(10)*s, y+(-42)*s+dy);
            glVertex2f(x+(7)*s, y+(-50)*s+dy);
            glVertex2f(x+(4)*s, y+(-47)*s+dy);
            glVertex2f(x+(1)*s, y+(-54)*s+dy);
            glVertex2f(x+(-2)*s, y+(-45)*s+dy);
            glVertex2f(x+(-5)*s, y+(-44)*s+dy);
            glVertex2f(x+(-7)*s, y+(-47)*s+dy);
            glVertex2f(x+(-10)*s, y+(-41)*s+dy);
            glVertex2f(x+(-13)*s, y+(-44)*s+dy);
        glEnd();
    }


    void up() { if (y<570) y += speed; }
    void down() { if (y>60) y -= speed; }
    void left() { if (x>70) x -= speed; }
    void right() { if (x<570) x += speed; }

    float get_x() const { return x; }
    float get_y() const { return y; }
};

SpaceShip spaceship(320, 320, 10);
vector<Meteor> m;
bool keys[256]={ false };
bool game_started=false;
bool game_over=false;
int diff=5;  // smaller the diff -> more density
int score=0;

void reset_game() {
    spaceship=SpaceShip(320, 320, 10);
    m.clear();
    score=0;
    game_over=false;
    game_started=false;
}

void update() {
    if (keys['w']) spaceship.up();
    if (keys['s']) spaceship.down();
    if (keys['a']) spaceship.left();
    if (keys['d']) spaceship.right();

    if (keys['r'] && game_over) reset_game();

    for (auto& meteor : m) {
        meteor.fall(10.5f);

        // Collision detection
        float dx=meteor.get_x()-spaceship.get_x();
        float dy=meteor.get_y()-spaceship.get_y();
        float distance=sqrt(dx*dx+dy*dy);
        if (distance<meteor.get_size()) {
            game_over=true;
            break;
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    keys[key]=true;
    if (key=='w') spaceship.booster=true;
    if (key=='w' || key=='a' || key=='s' || key=='d') game_started=true;
}

void keyboard_up(unsigned char key, int x, int y) {
    keys[key]=false;
    if (key=='w') spaceship.booster=false;
}

void border() {
    glColor3f(0.4431, 0.6392, 0.8667);
    glBegin(GL_POLYGON);
        glVertex2f(0, 620);
        glVertex2f(640, 620);
        glVertex2f(640, 640);
        glVertex2f(0, 640);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(640, 0);
        glVertex2f(640, 20);
        glVertex2f(0, 20);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(20, 0);
        glVertex2f(20, 640);
        glVertex2f(0, 640);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(620, 0);
        glVertex2f(640, 0);
        glVertex2f(640, 640);
        glVertex2f(620, 640);
    glEnd();
}
void gameover_screen() {
    glColor3f(1, 0, 0);
    utils_render_bitmap_string(240, 380, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER!");

    glColor3f(0, 1, 0);
    utils_render_bitmap_string(260, 360, GLUT_BITMAP_HELVETICA_12, "[Press R to Restart]");

    glColor3f(1, 1, 1);
    utils_render_bitmap_string(260, 250, GLUT_BITMAP_TIMES_ROMAN_24, "Score");

    char score_message[50];
    snprintf(score_message, sizeof(score_message), "%d", score);
    utils_render_bitmap_string(340, 250, GLUT_BITMAP_TIMES_ROMAN_24, score_message);
}
void game_start_screen() {
    glColor3f(1, 1, 0);
    utils_render_bitmap_string(217, 400, GLUT_BITMAP_TIMES_ROMAN_24, "METEOR ESCAPE ");

    glColor3f(0, 1, 0);
    utils_render_bitmap_string(30, 100, GLUT_BITMAP_HELVETICA_12, "Press ");

    glColor3f(1.0f, 0.6f, 0.2f);
    utils_render_bitmap_string(65, 100, GLUT_BITMAP_HELVETICA_12, "[W, A, S, D]");

    glColor3f(0, 1, 0);
    utils_render_bitmap_string(133, 100, GLUT_BITMAP_HELVETICA_12, " to Move and Avoid Meteors to Survive");
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!game_started) game_start_screen();
    if (!game_over) {
        spaceship.draw();
        for (auto& meteor : m) {
            glColor3f(0.8f, 0.4f, 0.0f);
            meteor.draw();
        }
        utils_render_bitmap_string(510, 40, GLUT_BITMAP_HELVETICA_18, ("Score: "+to_string(score)).c_str());
    } else {
        gameover_screen();
    }
    glColor3f(1, 1, 1);
    utils_render_bitmap_string(30, 60, GLUT_BITMAP_HELVETICA_10, "COMPUTER GRAPHICS ");
    glColor3f(1, 1, 0);
    utils_render_bitmap_string(30, 40, GLUT_BITMAP_HELVETICA_10, "</> Aditya Gupta");
    border();
    glutSwapBuffers();
}

void resize(int w, int h) {
    glOrtho(0, w, 0, h, -1, 1);
}

void timer(int value) {
    if (game_started && !game_over) {
        if (rand()%diff==0) {
            float rx=rand()%640;
            float ry=640+rand()%100;
            float r_size=20+rand()%30;
            int r_sides=6+rand()%5;
            m.emplace_back(rx, ry, r_size, r_sides);
            score++;
        }
    }

    update();
    glutTimerFunc(25, timer, 0); // 16ms = ~60 FPS (adjust to your needs)

}

int main( int argc, char **argv ) {
    srand(static_cast<unsigned>(time(0)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize( 640, 640 );
    glutCreateWindow("METEOR ESCAPE ");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutTimerFunc(4, timer, 0);

    glutMainLoop();
    return 0;
}
