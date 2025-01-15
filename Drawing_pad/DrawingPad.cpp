#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <math.h>

int xmax = 1920, ymax = 1080;
bool drawGridFlag = false;
bool printpoint = false;
struct point {
    double x;
    double y;
};


std::vector<point> bz1;
std::vector<point> bz2;
std::vector<std::vector<point>> allCurves;

bool curveStarted = false;
bool curveCompleted = false;

// Factorial calculation
int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Binomial coefficient calculation
int binCof(int n, int i) {
    return factorial(n) / (factorial(i) * factorial(n - i));
}

// Curve point calculation
void bezierCurve(const std::vector<point>& cp) {
    bz2.clear();
    int n = cp.size();
    for (double u = 0; u <= 1; u += 0.0001) {
        point r = { 0.0, 0.0 };
        for (int i = 0; i < n; i++) {
            int bic = binCof(n - 1, i);
            double B = bic * pow(1 - u, n - 1 - i) * pow(u, i);
            r.x += B * cp[i].x;
            r.y += B * cp[i].y;
        }
        bz2.push_back(r);
    }
}

void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}

// Draw the curve
void drawBezierCurve(const std::vector<point>& points) {
    if (points.size() < 2) {
        return;
    }
    bezierCurve(points);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < bz2.size(); ++i) {
        glVertex2i(bz2[i].x, bz2[i].y);
    }
    glEnd();
}

// Draw control points
void drawControlPoints(const std::vector<point>& points, int curveIndex) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < points.size(); ++i) {
        glVertex2i(points[i].x, points[i].y);
    }
    glEnd();
    if (printpoint) {
        glColor3f(0.0, 0.0, 0.0);

        for (int i = 0; i < points.size(); ++i) {
            char buffer[50];

            sprintf_s(buffer, "C%d (%.1f, %.1f)", curveIndex, points[i].x, points[i].y);

            renderBitmapString(points[i].x + 10, points[i].y + 10, GLUT_BITMAP_HELVETICA_12, buffer);

            std::cout << "Control Point " << i << " of Curve " << curveIndex
                << ": (" << points[i].x << ", " << points[i].y << ")" << std::endl;
        }
    }

}
void drawGrid() {
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_LINES);
    for (int i = 0; i < xmax; i += 50) {
        glVertex2d(i, 0);
        glVertex2d(i, ymax);
    }
    for (int i = 0; i < ymax; i += 50) {
        glVertex2d(0, i);
        glVertex2d(xmax, i);
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (drawGridFlag) {
        drawGrid();
    }
    for (int i = 0; i < allCurves.size(); ++i) {
        drawBezierCurve(allCurves[i]);
        if (printpoint) {
            drawControlPoints(allCurves[i], i + 1);
        }

    }
    if (curveStarted) {
        drawBezierCurve(bz1);
        if (printpoint) {
            drawControlPoints(bz1, 0);
        }

    }
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(10.0f, ymax - 30.0f, GLUT_BITMAP_HELVETICA_18, "Dhuruv Kumar");
    renderBitmapString(10.0f, ymax - 50.0f, GLUT_BITMAP_HELVETICA_18, "SAP ID: 500107769");
    renderBitmapString(1600.0f, ymax - 30.0f, GLUT_BITMAP_HELVETICA_18, "Left click: Add control point");
    renderBitmapString(1600.0f, ymax - 50.0f, GLUT_BITMAP_HELVETICA_18, "A: Clear all curves");
    renderBitmapString(1600.0f, ymax - 70.0f, GLUT_BITMAP_HELVETICA_18, "Backspace: Remove last control point");
    renderBitmapString(1600.0f, ymax - 90.0f, GLUT_BITMAP_HELVETICA_18, "Space: Complete current curve");
    renderBitmapString(1600.0f, ymax - 110.0f, GLUT_BITMAP_HELVETICA_18, "Enter: Remove last curve");
    renderBitmapString(1600.0f, ymax - 130.0f, GLUT_BITMAP_HELVETICA_18, "G: Show grid");
    renderBitmapString(1600.0f, ymax - 150.0f, GLUT_BITMAP_HELVETICA_18, "P: Show coordinates");
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 8) {
        if (bz1.size() > 2) {
            bz1.erase(bz1.end() - 2);
        }
        else if (bz1.size() == 2) {
            bz1.pop_back();
        }
        else {
            bz1.clear();
            curveStarted = false;
        }
    }
    else if (key == ' ') {
        if (curveStarted) {
            allCurves.push_back(bz1);
            bz1.clear();
            bz2.clear();
            curveStarted = false;
        }
    }
    else if (key == 13) {
        if (!allCurves.empty()) {
            allCurves.pop_back();
        }
    }
    else if (key == 'a') {
        bz1.clear();
        bz2.clear();
        allCurves.clear();
        curveStarted = false;
        curveCompleted = false;
    }
    else if (key == 'g' || key == 'G') {
        drawGridFlag = !drawGridFlag;
    }
    else if (key == 'p' || key == 'P') {
        printpoint = !printpoint;
    }
    glutPostRedisplay();
}

// Mouse function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        point p = { static_cast<double>(x), static_cast<double>(ymax - y) };
        if (!curveStarted) {
            bz1.push_back(p);
            curveStarted = true;
        }
        else if (bz1.size() == 1) {
            bz1.push_back(p);
        }
        else {
            bz1.insert(bz1.end() - 1, p);
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(xmax, ymax);
    glutCreateWindow("Bezier Curve Drawing");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glLoadIdentity();
    gluOrtho2D(0, xmax, 0, ymax);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
