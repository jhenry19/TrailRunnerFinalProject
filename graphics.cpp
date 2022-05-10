#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include "rock.h"
#include "tree.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
int colorVectorIndex = 0;
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

vector<color> colorVector = {white, purple, black, yellow, brown, orange, lightGrey};
string textboxText;

vector<unique_ptr<Shape>> clouds;
Rect trail;
Rect textbox;
Rect leftColorButton;
Rect rightColorButton;
Rect easyButton;
Rect mediumButton;
Rect hardButton;
vector<Tree> trees;
Rect user;
Rect userCopy;
vector<unique_ptr<Rock>> rocks;
vector<int> rockSpeed;

// For the user to jump
const int userStartY = 425;
const double GRAVITY = -20;
const double INIT_VELOCITY = 80;
double deltaY;
double userVelocity;
bool userJumping;
bool longJump;
bool onGround;
double userJumpCount; // Used to gradually progress the user jumping
double jumpY; // Used in calculating the position of the user during a jump;

//For the user to duck
bool userDucking;

enum screen {intro, avatar, game};
screen currentScreen;

enum difficulty {easy, medium, hard};
difficulty currentDifficulty;

int score;
string scoreAsString; // used so variables aren't initialized in the display loop
bool collision;

bool validInput; // Used for name input validation

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

void initTrail() {
    trail.setCenter(250, 475);
    trail.setSize(width, 75);
    trail.setColor(grey);
}

void initTrees() {

    trees.clear();

    int totalTreeWidth = 0;
    dimensions treeSize;

    // Creates the buildings
    while (totalTreeWidth < width + 200) {
        // Tree height between 300-400
        treeSize.height = rand() % 201 + 200;
        // Building width between 100-150
        treeSize.width = rand() % 51 + 50;

        trees.push_back(Tree(totalTreeWidth + (treeSize.width/2) + 5,
                             height - ((treeSize.height/2) + 50),
                                 treeSize));

        totalTreeWidth += treeSize.width + 15;
    }
}

void initUser() {
    user = Rect(colorVector[colorVectorIndex],50,userStartY);
    user.setHeight(50);
    user.setWidth(50);

    // Initial values set for user jumping
    userJumping = false;
    longJump = false;
    onGround = true;
    userJumpCount = 0;
    userVelocity = 0;
}

void initGame() {
    rocks.clear();
    score = 0;
    scoreAsString = "0";
    collision = false;
}

void sendRock(){
    int radius;
    int speed;
    switch (currentDifficulty) {
        case easy:
            radius = rand() % 30 + 15;
            speed = -((rand() % 15) + 5);
            break;
        case medium:
            radius = rand() % 35 + 15;
            speed = -((rand() % 20) + 5);
            break;
        case hard:
            radius = rand() % 40 + 15;
            speed = -((rand() % 25) + 10);
            break;
    }

    rocks.push_back(make_unique<Rock>(black, 500, 440, radius));
    rockSpeed.push_back(speed);
}

void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initClouds();
    initTrail();
    initTrees();
    initUser();
    initGame();
    sendRock();
    currentScreen = intro;
    textboxText = "";
    validInput = true;

    user.setColor(colorVector[colorVectorIndex % colorVector.size()]);
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
        string line2 = "Use the up arrow to jump over the rocks.";
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
    }

    /*
     * Avatar making screen
     */
    if (currentScreen == avatar) {
        //Prints string to screen
        glColor3f(0,0,0);
        string line1 = "Avatar Creation";
        glRasterPos2i(width * .40, height * .15);
        for (const char &letter : line1) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        //todo user selects name
        //Prints string to screen
        string line2 = "Please input your name:  ";
        glRasterPos2i(width * .22, height * .25);
        for (const char &letter : line2){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        //Create textbox
        dimensions textBoxSize(150,22);
        textbox.setSize(textBoxSize);
        textbox.setColor(1.0,1.0,1.0,.5);
        textbox.setCenterX(385);
        textbox.setCenterY(118);
        textbox.draw();

        //Write user input in textbox
        glColor3f(0,0,0); // need to change due to white box above
        for (const char &letter : textboxText){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        //Textbox instructions
        string line3 = "(Input must be string of letters no longer than 10 characters)";
        glRasterPos2i(width * .30, height * .29);
        for (const char &letter : line3){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, letter);
        }

        if (!validInput) {
            glColor3f(1,0,0);
            string invalidInputText = "Invalid name. Please try again.";
            glRasterPos2i(width * .37, height * .32);
            for (const char &letter : invalidInputText){
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, letter);
            }
        }

        //todo user selects color
        //Print string
        glColor3f(0,0,0);
        //Prints string to screen
        string line4 = "Choose your avatar's color:";
        glRasterPos2i(width * .28, height * .40);
        for (const char &letter : line4){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        //Create color toggle buttons
        dimensions buttonsize1(30,30);
        leftColorButton.setSize(buttonsize1);
        leftColorButton.setColor(0, 0, 0, .5);
        leftColorButton.setCenterX(350);
        leftColorButton.setCenterY(270);
        leftColorButton.draw();

        rightColorButton.setSize(buttonsize1);
        rightColorButton.setColor(0, 0, 0, .5);
        rightColorButton.setCenterX(150);
        rightColorButton.setCenterY(270);
        rightColorButton.draw();

        userCopy = Rect(colorVector[colorVectorIndex % colorVector.size()], 250, 270);
        userCopy.setHeight(50);
        userCopy.setWidth(50);
        userCopy.draw();
        
        //todo user selects difficulty
        //Print string
        glColor3f(0,0,0);
        //Prints string to screen
        string line5 = "Choose the difficulty:";
        glRasterPos2i(width * .32, height * .70);
        for (const char &letter : line5){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        //Create difficulty buttons
        dimensions buttonsize2(70,40);
        easyButton.setSize(buttonsize2);
        easyButton.setColor(0, 1, 0, 1);
        easyButton.setCenterX(100);
        easyButton.setCenterY(400);
        easyButton.draw();
        glColor3f(0,0,0);
        string line6 = "Easy";
        glRasterPos2i(width * .16, height * .81);
        for (const char &letter : line6){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        mediumButton.setSize(buttonsize2);
        mediumButton.setColor(yellow);
        mediumButton.setCenterX(250);
        mediumButton.setCenterY(400);
        mediumButton.draw();
        glColor3f(0,0,0);
        string line7 = "Medium";
        glRasterPos2i(width * .43, height * .81);
        for (const char &letter : line7){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        hardButton.setSize(buttonsize2);
        hardButton.setColor(1, 0, 0, 1);
        hardButton.setCenterX(400);
        hardButton.setCenterY(400);
        hardButton.draw();
        glColor3f(0,0,0);
        string line8 = "Hard";
        glRasterPos2i(width * .76, height * .81);
        for (const char &letter : line8){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
        }

        //default difficulty if none selected
        currentDifficulty = easy;
        

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
        for (Tree &t: trees) {
            t.draw();
        }

        // Draws trail
        trail.draw();

        // Animation for user jumping. I tried this as a timer and the game started lagging like crazy, so it's better here
        if (userJumping) {
            userJumpCount += .1;
            // The physics for how the user jumps
            deltaY = (userVelocity * userJumpCount) + (GRAVITY * (userJumpCount * userJumpCount) / 2);
            jumpY = userStartY - (deltaY);

            if (!longJump) {
                userVelocity += (GRAVITY * userJumpCount) * .05;
            }
            else{
                userVelocity += (GRAVITY * userJumpCount) * .005;
            }

            // If the user is on the ground the jump is stopped
            if (jumpY > userStartY) {
                userJumping = false;
                userJumpCount = 0;
                userVelocity = 0;
                onGround = true;
                user.setCenterY(userStartY);
            } else { // else the user continues the jump
                user.setCenterY(jumpY);
            }
        }

        // Draws the user
        user.draw();

        // Draws the rocks
        for (int i = 0; i < rocks.size(); ++i) {
            rocks[i]->draw();
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
            string line1 = "You were squashed by a rock!";
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

void treeTimer(int dummy) {
    for (int i = 0; i < trees.size(); ++i) {
        // Move all the  buildings to the left
        trees[i].moveX(-2);
        // If a shape has moved off the screen
        if (trees[i].getCenterX() < -(trees[i].getWidth() / 2)) {
            // Set it to the right of the screen so that it passes through again
            int buildingOnLeft = (i == 0) ? trees.size() - 1 : i - 1;
            trees[i].setCenterX(trees[buildingOnLeft].getCenterX() + trees[buildingOnLeft].getWidth() / 2 + trees[i].getWidth() / 2 + 5);
        }
    }
    // Getting rid of the redisplay call here makes the game run way smoother. The car timer runs the redisplay so there's no need to call it here
    glutTimerFunc(50, treeTimer, dummy);
}

void rockTimer(int dummy) {
    for (int i = 0; i < rocks.size(); ++i) {
        rocks[i]->moveX(rockSpeed[i]); // Move each car according to its own speed

        // If a car has moved off the screen
        if (rocks[i]->getRightX() <= 0) {
            rocks.erase(rocks.begin()); // removes the first element of the array
            rockSpeed.erase(rockSpeed.begin()); // removes the first element of the array

            ++score; // add to score
            scoreAsString = to_string(score);

            sendRock(); // sends another car
        }
//         If the car hits the user
        if (rocks[i]->isOverlapping(user)) {
            collision = true;
        }
    }

    glutPostRedisplay();

    // Timer is paused if there is a collision
    if (!collision) glutTimerFunc(10, rockTimer, dummy);
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
        initTrail();
        initTrees();
        initUser();
        initGame();
        sendRock();
        glutTimerFunc(10, rockTimer, 0); // restart car timer after collision
    }

    if (currentScreen == avatar){
        if (key == 127) { // Backspace pressed
            if (textboxText.length() > 0) {
                textboxText = textboxText.substr(0, textboxText.length() - 1);
            }
        }
        else if (key == 13) { // Submit
            validInput = true;

            // Checks length
            if (textboxText.length() > 10) {
                validInput = false;
            }

            // Checks for digits
            for (char c : textboxText) {
                if (isdigit(c)) validInput = false;
            }

            if (validInput) {
                currentScreen = game;

                // Starts the timers
                glutTimerFunc(0, cloudTimer, 0);
                glutTimerFunc(0, treeTimer, 0);
                glutTimerFunc(0, rockTimer, 0);
            }
        }
        else {
            textboxText += char(key); // Converts ascii int to character
        }
    }
    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN && onGround) {
        userDucking = true;
    }
    else if (key == GLUT_KEY_DOWN) {
        longJump = false;
    }

    if (key == GLUT_KEY_UP && onGround) {
            userJumping = true;
            userVelocity = INIT_VELOCITY;
            onGround = false;
            longJump = true;
    }
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    // Listens for a click on the intro screen to start the game
    if(state == GLUT_DOWN && currentScreen == intro) {
        currentScreen = avatar;
    }

    else if (state == GLUT_DOWN && currentScreen == avatar) { //Looking for button clicks on avatar screen

        Rect cursorBox = Rect(x,y, dimensions(1, 1)); // used to check if overlapping

        //Clicked on right color toggle button
        if (cursorBox.isOverlapping(rightColorButton)){
            ++colorVectorIndex;
            user.setColor(colorVector[colorVectorIndex % colorVector.size()]);
            userCopy.setColor(colorVector[colorVectorIndex % colorVector.size()]);
        }
        //Clicked on left color toggle button
        else if (cursorBox.isOverlapping(leftColorButton)){
            --colorVectorIndex;
            user.setColor(colorVector[colorVectorIndex % colorVector.size()]);
            userCopy.setColor(colorVector[colorVectorIndex % colorVector.size()]);
        }
        //Clicked on easy button
        else if (cursorBox.isOverlapping(easyButton)) {
            currentDifficulty = easy;
        }

        //Clicked on medium button
        else if (cursorBox.isOverlapping(mediumButton)) {
            currentDifficulty = medium;
        }

        //Clicked on hard button
        else if (cursorBox.isOverlapping(hardButton)) {
            currentDifficulty = hard;
        }
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
    wd = glutCreateWindow("Trail Runner" /* title */ );
    
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
