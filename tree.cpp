#include "tree.h"
#include <iostream>

using namespace std;


Tree::Tree() : Shape(), size({0, 0}) {
}

Tree::Tree(dimensions size) : Shape() {
    setSize(size);
}

Tree::Tree(color fill) : Shape(fill), size({0, 0}) {
}

Tree::Tree(point2D center) : Shape(center), size({0, 0}) {
}

Tree::Tree(color fill, point2D center) : Shape(fill, center), size({0, 0}) {
}

Tree::Tree(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), size({0, 0}) {
}

Tree::Tree(double x, double y) : Shape(x, y), size({0, 0}) {
}

Tree::Tree(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), size({0, 0}) {
}

Tree::Tree(color fill, double x, double y) : Shape(fill, x, y), size({0, 0}) {
}

Tree::Tree(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), size({0, 0}) {
}

Tree::Tree(color fill, dimensions size) : Shape(fill) {
    setSize(size);
}

Tree::Tree(point2D center, dimensions size) : Shape(center) {
    setSize(size);
}

Tree::Tree(color fill, point2D center, dimensions size) : Shape(fill, center) {
    setSize(size);
}

Tree::Tree(double red, double green, double blue, double alpha, dimensions size) : Shape(red, green, blue, alpha) {
    setSize(size);
}

Tree::Tree(double x, double y, dimensions size) : Shape(x, y) {
    setSize(size);
}

Tree::Tree(double red, double green, double blue, double alpha, double x, double y, dimensions size) : Shape(red, green, blue, alpha, x, y) {
    setSize(size);
}

Tree::Tree(color fill, double x, double y, dimensions size) : Shape(fill, x, y) {
    setSize(size);
}

Tree::Tree(double red, double green, double blue, double alpha, point2D center, dimensions size) : Shape(red, green, blue, alpha, center) {
    setSize(size);
}

dimensions Tree::getSize() const {
    return size;
}


double Tree::getWidth() const {
    return size.width;
}

double Tree::getHeight() const {
    return size.height;
}

double Tree::getLeftX() const {
    return center.x - (size.width / 2.0);
}

double Tree::getRightX() const {
    return center.x + (size.width / 2.0);
}

double Tree::getTopY() const {
    return center.y - (size.height / 2.0);
}

double Tree::getBottomY() const {
    return center.y + (size.height / 2.0);
}

void Tree::setSize(dimensions size) {
    if (size.width >= 0 && size.height >= 0) {
        this->size = size;
    }
}

void Tree::setSize(double width, double height) {
    setSize({width, height});
}

void Tree::setWidth(double width) {
    setSize({width, size.height});
}

void Tree::setHeight(double height) {
    setSize({size.width, height});
}

void Tree::changeSize(double deltaWidth, double deltaHeight) {
    setSize({size.width + deltaWidth, size.height + deltaHeight});
}

void Tree::changeWidth(double delta) {
    setSize({size.width + delta, size.height});
}

void Tree::changeHeight(double delta) {
    setSize({size.width, size.height + delta});
}

void Tree::draw() const {;

    // Draw trunk
    glColor3f(165.0/255, 42.0/255, 42.0/255);
    glBegin(GL_QUADS);
    glVertex2i(this->getCenterX() - (this->getWidth() * (TRUNK_WIDTH / 2)), this->getTopY() + (this->getBottomY() * TRUNK_LEAVES_SPLIT));
    glVertex2i(this->getCenterX() - (this->getWidth() * (TRUNK_WIDTH / 2)), this->getBottomY());
    glVertex2i(this->getCenterX() + (this->getWidth() * (TRUNK_WIDTH / 2)), this->getBottomY());
    glVertex2i(this->getCenterX() + (this->getWidth() * (TRUNK_WIDTH / 2)), this->getTopY() + (this->getBottomY() * TRUNK_LEAVES_SPLIT));
    glEnd();

    // Draw leaves
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    glVertex2i(this->getLeftX(), this->getTopY());
    glVertex2i(this->getLeftX(), this->getTopY() + (this->getBottomY() * TRUNK_LEAVES_SPLIT));
    glVertex2i(this->getRightX(), this->getTopY() + (this->getBottomY() * TRUNK_LEAVES_SPLIT));
    glVertex2i(this->getRightX(), this->getTopY());
    glEnd();


}
