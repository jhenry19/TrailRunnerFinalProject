#ifndef RUNNER_CAR_H
#define RUNNER_CAR_H

#include "shape.h"
#include "rect.h"


class Rock : public Shape {
private:
    dimensions size;
    double radius = 20;
public:
    /* Constructors */
    Rock();
    explicit Rock(dimensions size);
    explicit Rock(color fill);
    explicit Rock(point2D center);
    Rock(color fill, point2D center);
    Rock(double red, double green, double blue, double alpha);
    Rock(double x, double y);
    Rock(double red, double green, double blue, double alpha, double x, double y);
    Rock(color fill, double x, double y, int radius);
    Rock(double red, double green, double blue, double alpha, point2D center);
    Rock(color fill, dimensions size);
    Rock(point2D center, dimensions size);
    Rock(color fill, point2D center, dimensions size);
    Rock(double red, double green, double blue, double alpha, dimensions size);
    Rock(double x, double y, dimensions size);
    Rock(double red, double green, double blue, double alpha, double x, double y, dimensions size);
    Rock(color fill, double x, double y, dimensions size);
    Rock(double red, double green, double blue, double alpha, point2D center, dimensions size);

    /* Destructor */
    virtual ~Rock() = default;

    /* Getters */
    dimensions getSize() const;
    double getWidth() const;
    double getHeight() const;
    double getLeftX() const override;
    double getRightX() const override;
    double getTopY() const override;
    double getBottomY() const override;

    /* Setters */
    void setSize(dimensions size);
    void setSize(double width, double height);
    void setWidth(double width);
    void setHeight(double height);
    void changeSize(double deltaWidth, double deltaHeight);
    void changeWidth(double delta);
    void changeHeight(double delta);
    void setRadius(double radius);

    /* Check if a car and a Rock objects are overlapping */
    bool isOverlapping(const Rect &r) const;

    /* Draw */
    void draw() const override;


};

#endif //RUNNER_CAR_H
