#include "graphics.h"
#include "car.h"
#include <iostream>
using namespace std;


Car::Car() : Shape(), size({0, 0}) {
}

Car::Car(dimensions size) : Shape() {
    setSize(size);
}

Car::Car(color fill) : Shape(fill), size({0, 0}) {
}

Car::Car(point2D center) : Shape(center), size({0, 0}) {
}

Car::Car(color fill, point2D center) : Shape(fill, center), size({0, 0}) {
}

Car::Car(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), size({0, 0}) {
}

Car::Car(double x, double y) : Shape(x, y), size({0, 0}) {
}

Car::Car(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), size({0, 0}) {
}

// todo: add tireRadius to other constructors
Car::Car(color fill, double x, double y) : Shape(fill, x, y), size({0, 0}) {
}

Car::Car(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), size({0, 0}) {
}

Car::Car(color fill, dimensions size) : Shape(fill) {
    setSize(size);
}

Car::Car(point2D center, dimensions size) : Shape(center) {
    setSize(size);
}

Car::Car(color fill, point2D center, dimensions size) : Shape(fill, center) {
    setSize(size);
}

Car::Car(double red, double green, double blue, double alpha, dimensions size) : Shape(red, green, blue, alpha) {
    setSize(size);
}

Car::Car(double x, double y, dimensions size) : Shape(x, y) {
    setSize(size);
}

Car::Car(double red, double green, double blue, double alpha, double x, double y, dimensions size) : Shape(red, green, blue, alpha, x, y) {
    setSize(size);
}

Car::Car(color fill, double x, double y, dimensions size) : Shape(fill, x, y) {
    setSize(size);
}

Car::Car(double red, double green, double blue, double alpha, point2D center, dimensions size) : Shape(red, green, blue, alpha, center) {
    setSize(size);
}

dimensions Car::getSize() const {
    return size;
}

double Car::getWidth() const {
    return size.width;
}

double Car::getHeight() const {
    return size.height;
}

double Car::getLeftX() const {
    return center.x - (size.width / 2.0);
}

double Car::getRightX() const {
    return center.x + (size.width / 2.0);
}

double Car::getTopY() const {
    return center.y - (size.height / 2.0);
}

double Car::getBottomY() const {
    return center.y + (size.height / 2.0);
}

void Car::setSize(dimensions size) {
    if (size.width >= 0 && size.height >= 0) {
        this->size = size;
    }
}

void Car::setSize(double width, double height) {
    setSize({width, height});
}

void Car::setWidth(double width) {
    setSize({width, size.height});
}

void Car::setHeight(double height) {
    setSize({size.width, height});
}

void Car::changeSize(double deltaWidth, double deltaHeight) {
    setSize({size.width + deltaWidth, size.height + deltaHeight});
}

void Car::changeWidth(double delta) {
    setSize({size.width + delta, size.height});
}

void Car::changeHeight(double delta) {
    setSize({size.width, size.height + delta});
}

const void Car::setRadius(double r) {
    tireRadius = r;
}

bool Car::isOverlapping(const Rect &r) const {
    // When one is to the left of the other
    if (r.getRightX() < this->getLeftX() || this->getRightX() < r.getLeftX()) {
        return false;
    }
        // When one is above the other
    else if (r.getBottomY() < this->getTopY() || this->getBottomY() < r.getTopY()) {
        return false;
    }
    return true;
}

void Car::draw() const {
    // Set drawing color to fill color
    glColor3f(fill.red, fill.green, fill.blue);
    // Draw Car body as Quads
    glBegin(GL_QUADS);
    glVertex2i(this->getLeftX(), this->getTopY());
    glVertex2i(this->getLeftX(), this->getBottomY());
    glVertex2i(this->getRightX(), this->getBottomY());
    glVertex2i(this->getRightX(), this->getTopY());

    glEnd();

    /*
     * Draws 2 tires on each Car
     */
    glColor3f(.75, .75, .75);
    // Left tire
    glBegin(GL_TRIANGLE_FAN);
    point2D wheelCenter = point2D(int(this->getLeftX() + getWidth() * 1 / 5), int(this->getBottomY()));
    // Draw center point
    glVertex2i(wheelCenter.x, wheelCenter.y);
    // Draw points on edge of circle

    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(wheelCenter.x + (tireRadius * cos(i)),
                   wheelCenter.y + (tireRadius * sin(i)));
    }
    glEnd();

    // Right tire
    glBegin(GL_TRIANGLE_FAN);
    // Draw center point
    wheelCenter = point2D(int(this->getRightX() - getWidth() * 1 / 5), int(this->getBottomY()));
    glVertex2i(wheelCenter.x, wheelCenter.y);
    // Draw points on edge of circle
    for (double z = 0; z < 2.0*PI+0.05; z += (2.0*PI)/360.0) {
        glVertex2i(wheelCenter.x + (tireRadius * cos(z)),
                   wheelCenter.y + (tireRadius * sin(z)));
    }
    glEnd();
}

