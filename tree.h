#include "graphics.h"
#include "shape.h"

class Tree : public Shape {
private:
    dimensions size;
    double TRUNK_LEAVES_SPLIT = .3; // Determines what percent of the tree's height is leave and the rest is trunks
    double TRUNK_WIDTH = .2;
public:
    /* Constructors */
    Tree();
    explicit Tree(dimensions size);
    explicit Tree(color fill);
    explicit Tree(point2D center);
    Tree(color fill, point2D center);
    Tree(double red, double green, double blue, double alpha);
    Tree(double x, double y);
    Tree(double red, double green, double blue, double alpha, double x, double y);
    Tree(color fill, double x, double y);
    Tree(double red, double green, double blue, double alpha, point2D center);
    Tree(color fill, dimensions size);
    Tree(point2D center, dimensions size);
    Tree(color fill, point2D center, dimensions size);
    Tree(double red, double green, double blue, double alpha, dimensions size);
    Tree(double x, double y, dimensions size);
    Tree(double red, double green, double blue, double alpha, double x, double y, dimensions size);
    Tree(color fill, double x, double y, dimensions size);
    Tree(double red, double green, double blue, double alpha, point2D center, dimensions size);

    /* Destructor */
    virtual ~Tree() = default;

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

    /* Check if two Tree objects are overlapping */
    bool isOverlapping(const Tree &r) const;

    /* Draw */
    void draw() const override;

};