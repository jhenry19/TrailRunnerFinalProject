#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include "car.h"
#include "tree.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grey(128/255.0, 128/255.0, 128/255.0);
const color white(1, 1, 1);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color yellow(1, 1, 0);
const color brown(150/255.0, 30/255.0, 30/255.0);
const color lightGrey(200/255.0, 200/255.0, 200/255.0);
const color blueGrey(110/255.0, 127/255.0, 127/255.0);
const color orange(255/255.0, 165/255.0, 0);


vector<unique_ptr<Shape>> clouds;
Rect road;
Rect roadLine;
vector<Rect> buildings;
vector<color> buildingColors;
Rect user;
vector<unique_ptr<Car>> cars;
vector<int> carSpeed;

const int userStartY = 425;
bool userJumping;
bool speedUpJump; // Allows the user to make their user jump faster
int userJumpCount; // Used to gradually progress the user jumping
double jumpY; // Used in calculating the position of the user during a jump;

enum screen {intro, game};
screen currentScreen;

int score;
string scoreAsString; // used so variables aren't initialized in the display loop
bool collision;

void initClouds() {
    clouds.clear();
    dimensions cloudBottom(30, 30);
    // First cloud
    clouds.push_back(make_unique<Circle>(white, 300, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 330, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 320, 90, 20));
    clouds.push_back(make_unique<Rect>(white, 315, 105, cloudBottom));
    // Second cloud
    clouds.push_back(make_unique<Circle>(white, 100, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 130, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 120, 70, 20));
    clouds.push_back(make_unique<Rect>(white, 115, 85, cloudBottom));
}

void initRoad() {
    road.setCenter(250, 475);
    road.setSize(width, 75);
    road.setColor(grey);

    // The yellow line on the road
    roadLine.setWidth(width);
    roadLine.setHeight(7);
    roadLine.setColor(yellow);
    roadLine.setCenter(250, 472);
}

void initBuildings() {
    // Creates the list of colors that a building can be
    buildingColors.push_back(brown);
    buildingColors.push_back(lightGrey);
    buildingColors.push_back(blueGrey);
    buildingColors.push_back(purple);
    buildingColors.push_back(orange);

    buildings.clear();

    int totalBuildingWidth = 0;
    dimensions buildingSize;

    // Creates the buildings
    while (totalBuildingWidth < width + 200) {
        // Building height between 150-350
        buildingSize.height = rand() % 201 + 150;
        // Building width between 75-175
        buildingSize.width = rand() % 101 + 75;

        buildings.push_back(Rect(buildingColors[rand() % 5],
                                  totalBuildingWidth+(buildingSize.width/2) + 5,
                                  height - ((buildingSize.height/2) + 50),
                                 buildingSize));

        totalBuildingWidth += buildingSize.width + 15;
    }
}

void initUser() {
    user = Rect(white,50,userStartY);
    user.setHeight(50);
    user.setWidth(50);

    // Initial values set for user jumping
    userJumping = false;
    userJumpCount = 0;
    speedUpJump = false;
}

void initGame() {
    cars.clear();
    score = 0;
    scoreAsString = "0";
    collision = false;
}

void sendCar() {
    cars.push_back(make_unique<Car>(black, 500, 440, dimensions(75,40)));
    carSpeed.push_back(-((rand() % 15) + 3));
}

void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initClouds();
    initRoad();
    initBuildings();
    initUser();
    initGame();
    sendCar();
    currentScreen = intro;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(skyBlue.red, skyBlue.green, skyBlue.blue, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)
    
    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE
    
    /*
     * Intro screen
     */
    if (currentScreen == intro) {
        glColor3f(0, 0, 0);

        // Prints the message to the window
        string line1 = "Get Ready to Jump!";
        string line2 = "Use the up arrow to jump over the cars.";
        string line3 = "and use the down arrow to speed up your jump.";
        string line4 = "To begin, click anywhere on the screen.";
        string line5 = "To quit, press the escape key.";

        glRasterPos2i(width * .35, height * .3);
        for (const char &letter : line1) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
        glRasterPos2i(width * .17, height * .4);
        for (const char &letter : line2) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
        glRasterPos2i(width * .15, height * .5);
        for (const char &letter : line3) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
        glRasterPos2i(width * .17, height * .6);
        for (const char &letter : line4) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }
        glRasterPos2i(width * .25, height * .7);
        for (const char &letter : line5) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        Tree t = Tree(0,0);
        t.setSize(dimensions(100,300));

        cout << t.getCenterX() << endl;
        t.draw();

    }
    /*
     * Game screen
     */
    else if (currentScreen == game){ // if on game screen
        // Draws clouds
        for (unique_ptr<Shape> &s: clouds) {
            // #polymorphism
            s->draw();
        }

        //Draws Buildings
        for (Rect &r: buildings) {
            r.draw();
        }

        // Draws road
        road.draw();
        roadLine.draw();

        // Animation for user jumping. I tried this as a timer and the game started lagging like crazy, so it's better here
        if (userJumping) {
            if (speedUpJump) userJumpCount += 3;
            ++userJumpCount;
            // The physics for how the user jumps (it's a parabola)
            jumpY = userStartY + (.03 * (userJumpCount * userJumpCount) - 4 * userJumpCount);

            // If the user is on the ground the jump is stopped
            if (jumpY >= userStartY) {
                userJumping = false;
                speedUpJump = false;
                userJumpCount = 0;
                user.setCenterY(userStartY);
            } else { // else the user continues the jump
                user.setCenterY(jumpY);
            }
        }

        // Draws the user
        user.draw();

        // Draws the cars
        for (int i = 0; i < cars.size(); ++i) {
            cars[i]->draw();
        }

        // Shows the score in the top left corner
        glColor3f(0, 0, 0);
        glRasterPos2i(width * .9, height * .1);
        for (const char &letter : scoreAsString) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        // Displays info if there is a collision between the user and a car
        if (collision) {
            // Background box
            Rect box = Rect(color(1, 1, 1, .5),dimensions(width*.9, height * .4));
            box.setCenter(point2D(width * .5, height * .25));
            box.draw();

            glColor3f(0, 0, 0);
            string line1 = "You were hit by a car!";
            string line2 = "Your score was " + scoreAsString + ".";
            string line3 = "To play again, press space. To quit, press escape";

            glRasterPos2i(width * .32, height * .15);
            for (const char &letter : line1) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
            }
            glRasterPos2i(width * .35, height * .25);
            for (const char &letter : line2) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
            }
            glRasterPos2i(width * .1, height * .35);
            for (const char &letter : line3) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
            }
        }
    }

    glFlush();  // Render now
}

void cloudTimer(int dummy) {
    for (unique_ptr<Shape> &s : clouds) {
        // Move all the clouds to the left
        s->moveX(-1);
        // If a shape has moved off the screen
        if (s->getCenterX() < -20) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(520);
        }
    }
    glutTimerFunc(100, cloudTimer, dummy);
}

void buildingTimer(int dummy) {
    for (int i = 0; i < buildings.size(); ++i) {
        // Move all the  buildings to the left
        buildings[i].moveX(-2);
        // If a shape has moved off the screen
        if (buildings[i].getCenterX() < -(buildings[i].getWidth() / 2)) {
            // Set it to the right of the screen so that it passes through again
            int buildingOnLeft = (i == 0) ? buildings.size() - 1 : i - 1;
            buildings[i].setCenterX(buildings[buildingOnLeft].getCenterX() + buildings[buildingOnLeft].getWidth() / 2 + buildings[i].getWidth() / 2 + 5);
            buildings[i].setColor(buildingColors[rand() % 5]); // Resets to a random color
        }
    }
    // Getting rid of the redisplay call here makes the game run way smoother. The car timer runs the redisplay so there's no need to call it here
    glutTimerFunc(50, buildingTimer, dummy);
}

void carTimer(int dummy) {
    for (int i = 0; i < cars.size(); ++i) {
        cars[i]->moveX(carSpeed[i]); // Move each car according to its own speed

        // If a car has moved off the screen
        if (cars[i]->getRightX() <= 0) {
            cars.erase(cars.begin()); // removes the first element of the array
            carSpeed.erase(carSpeed.begin()); // removes the first element of the array

            ++score; // add to score
            scoreAsString = to_string(score);

            sendCar(); // sends another car
        }
        // If the car hits the user
        if (cars[i]->isOverlapping(user)) {
            collision = true;
        }
    }

    glutPostRedisplay();

    // Timer is paused if there is a collision
    if (!collision) glutTimerFunc(10, carTimer, dummy);
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    // Space bar restarts the game after a collision
    if (key == 32 && collision) {
        initClouds();
        initRoad();
        initBuildings();
        initUser();
        initGame();
        sendCar();
        glutTimerFunc(10, carTimer, 0); // restart car timer after collision
    }
}

void kbdS(int key, int x, int y) {
    // Had to break this out of the switch so that the value would reset when the down arrow isn't pressed
    if (key == GLUT_KEY_DOWN) {
        speedUpJump = true;
    }
    else speedUpJump = false;

    if (key == GLUT_KEY_UP) {
            userJumping = true;
    }
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    // Listens for a click on the intro screen to start the game
    if(state == GLUT_DOWN && currentScreen == intro) {
        currentScreen = game;

        // Starts the timers
        glutTimerFunc(0, cloudTimer, 0);
        glutTimerFunc(0, buildingTimer, 0);
        glutTimerFunc(0,carTimer, 0);
    }
    glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Jumper" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    
    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);
    
    // handles mouse click
    glutMouseFunc(mouse);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
