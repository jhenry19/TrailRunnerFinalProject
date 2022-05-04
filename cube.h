#ifndef INC_3D_GRAPHICS_CUBE_H
#define INC_3D_GRAPHICS_CUBE_H

#include <vector>

struct point {
    double x;
    double y;
    double z;
};

class Cube {
private:
    point center;
    double edge_length; // Had to change to a double for scaling to work
    std::vector<point> corners;
    int growth_step = 0;

    // Private helper methods
    void reset_corners();
    void draw_point(const point &p) const;

public:

    // Constructors
    Cube();
    Cube(point center, unsigned int edge_length);

    // Draw the cube
    void draw() const;

    // Rotate the cube
    void rotate(double theta_x, double theta_y, double theta_z);

    // Move the cube
    void move(double delta_x, double delta_y, double delta_z);

    // Grow or shrink the cube. If grow is true it will grow by 10%, else it will shrink
    void scale(bool grow);
};


#endif //INC_3D_GRAPHICS_CUBE_H
