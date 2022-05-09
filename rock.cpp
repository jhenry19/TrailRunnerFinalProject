#include "graphics.h"
#include "rock.h"
#include <iostream>
using namespace std;


Rock::Rock() : Shape(), size({0, 0}) {
}

Rock::Rock(dimensions size) : Shape() {
    setSize(size);
}

Rock::Rock(color fill) : Shape(fill), size({0, 0}) {
}

Rock::Rock(point2D center) : Shape(center), size({0, 0}) {
}

Rock::Rock(color fill, point2D center) : Shape(fill, center), size({0, 0}) {
}

Rock::Rock(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), size({0, 0}) {
}

Rock::Rock(double x, double y) : Shape(x, y), size({0, 0}) {
}

Rock::Rock(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), size({0, 0}) {
}

Rock::Rock(color fill, double x, double y, int r) : Shape(fill, x, y), size({0, 0}) {
    radius = r;
}

Rock::Rock(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), size({0, 0}) {
}

Rock::Rock(color fill, dimensions size) : Shape(fill) {
    setSize(size);
}

Rock::Rock(point2D center, dimensions size) : Shape(center) {
    setSize(size);
}

Rock::Rock(color fill, point2D center, dimensions size) : Shape(fill, center) {
    setSize(size);
}

Rock::Rock(double red, double green, double blue, double alpha, dimensions size) : Shape(red, green, blue, alpha) {
    setSize(size);
}

Rock::Rock(double x, double y, dimensions size) : Shape(x, y) {
    setSize(size);
}

Rock::Rock(double red, double green, double blue, double alpha, double x, double y, dimensions size) : Shape(red, green, blue, alpha, x, y) {
    setSize(size);
}

Rock::Rock(color fill, double x, double y, dimensions size) : Shape(fill, x, y) {
    setSize(size);
}

Rock::Rock(double red, double green, double blue, double alpha, point2D center, dimensions size) : Shape(red, green, blue, alpha, center) {
    setSize(size);
}

dimensions Rock::getSize() const {
    return size;
}

double Rock::getWidth() const {
    return size.width;
}

double Rock::getHeight() const {
    return size.height;
}

double Rock::getLeftX() const {
    return center.x - (size.width / 2.0);
}

double Rock::getRightX() const {
    return center.x + (size.width / 2.0);
}

double Rock::getTopY() const {
    return center.y - (size.height / 2.0);
}

double Rock::getBottomY() const {
    return center.y + (size.height / 2.0);
}

void Rock::setSize(dimensions size) {
    if (size.width >= 0 && size.height >= 0) {
        this->size = size;
    }
}

void Rock::setSize(double width, double height) {
    setSize({width, height});
}

void Rock::setWidth(double width) {
    setSize({width, size.height});
}

void Rock::setHeight(double height) {
    setSize({size.width, height});
}

void Rock::changeSize(double deltaWidth, double deltaHeight) {
    setSize({size.width + deltaWidth, size.height + deltaHeight});
}

void Rock::changeWidth(double delta) {
    setSize({size.width + delta, size.height});
}

void Rock::changeHeight(double delta) {
    setSize({size.width, size.height + delta});
}

void Rock::setRadius(double r) {
    radius = r;
}

bool Rock::isOverlapping(const Rect &r) const {
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

void Rock::draw() const {
    // Set drawing color to fill color
    glColor3f(fill.red, fill.green, fill.blue);

    // Draw Rock
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(getCenterX(), getCenterY()); // Draws center of rock
    // Draw center point
    // Draw points on edge of circle
    for (double z = 0; z < 2.0*PI+0.05; z += (2.0*PI)/360.0) {
        glVertex2i(getCenterX() + (radius * cos(z)),
                   getCenterY() + (radius * sin(z)));
    }
    glEnd();
}

