#ifndef RUNNER_CAR_H
#define RUNNER_CAR_H

#include "shape.h"
#include "rect.h"


class Car : public Shape {
private:
    dimensions size;
    double tireRadius = 10;
public:
    /* Constructors */
    Car();
    explicit Car(dimensions size);
    explicit Car(color fill);
    explicit Car(point2D center);
    Car(color fill, point2D center);
    Car(double red, double green, double blue, double alpha);
    Car(double x, double y);
    Car(double red, double green, double blue, double alpha, double x, double y);
    Car(color fill, double x, double y);
    Car(double red, double green, double blue, double alpha, point2D center);
    Car(color fill, dimensions size);
    Car(point2D center, dimensions size);
    Car(color fill, point2D center, dimensions size);
    Car(double red, double green, double blue, double alpha, dimensions size);
    Car(double x, double y, dimensions size);
    Car(double red, double green, double blue, double alpha, double x, double y, dimensions size);
    Car(color fill, double x, double y, dimensions size);
    Car(double red, double green, double blue, double alpha, point2D center, dimensions size);

    /* Destructor */
    virtual ~Car() = default;

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
    const void setRadius(double radius);

    /* Check if a car and a Car objects are overlapping */
    bool isOverlapping(const Rect &r) const;

    /* Draw */
    void draw() const override;


};

#endif //RUNNER_CAR_H
