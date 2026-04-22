#include <iostream>
#include <raylib.h>
#include <cmath>

#define WIDTH  1600
#define HEIGHT 1000

#define GRAVITY 9.8
#define SIMULATION_SPEED 10
#define TRACE_SIZE 10000
#define FPS 120
#define ENERGY_LOSS 0.99f


class Pendulum
{  
private:
    Vector2 attachPoint;
    double angle;
    double angularVelocity;
    double lenght;
    double mass;

public:
    // Pendulum constructor
    Pendulum(Vector2 attachPoint, double angle, double lenght, double angularVelocity, double mass)
    {
        this->attachPoint = attachPoint;
        this->angle = angle;
        this->angularVelocity = angularVelocity;
        this->lenght = lenght;
        this->mass = mass;
    }

    double
    getAngle()
    { return angle; }

    double
    getMass()
    { return mass; }

    double
    getLenght()
    { return lenght; }

    double
    getAngularVelocity()
    { return angularVelocity; }

    void
    setAttachPoint(Vector2 attachPoint)
    {   
        this->attachPoint = attachPoint;
    }

    Vector2
    getMassPos()
    {
        float Bx = (float)(lenght * sin(angle));
        float By = (float)(lenght * cos(angle));
        return {attachPoint.x + Bx, attachPoint.y + By};
    }

    void
    draw()
    {
        // Draw attach point
        DrawCircle(attachPoint.x, attachPoint.y, 5, WHITE);
        // Draw string
        DrawLineEx(attachPoint, getMassPos(), 4, WHITE);
        // Draw mass
        DrawCircle(getMassPos().x, getMassPos().y, 10, BLUE);
    }

    // Update function for single pendulum simulation
    void
    update(double angularAcceleration, double dt)
    {
        angularVelocity += angularAcceleration * dt;
        angularVelocity *= pow(ENERGY_LOSS, dt);
        angle += angularVelocity * dt;
    }
};

class DoublePendulum 
{
private:    
    Pendulum *p1;
    Pendulum *p2;

public:
    DoublePendulum(Pendulum &p1, Pendulum &p2)
    {
        this->p1 = &p1;
        this->p2 = &p2;
    }

    Vector2
    getMassPos1()
    {
        return p1->getMassPos();
    }

    Vector2
    getMassPos2()
    {
        return p2->getMassPos();
    }
    
    void
    draw() {
        p2->draw();
        p1->draw();
    }

    void
    update(double dt)
    {
        // Angle
        double a1 = p1->getAngle();
        double a2 = p2->getAngle();

        // Lenght
        double l1 = p1->getLenght();
        double l2 = p2->getLenght();

        // Mass
        double m1 = p1->getMass();
        double m2 = p2->getMass();

        // Angular Velocity
        double w1 = p1->getAngularVelocity();
        double w2 = p2->getAngularVelocity();

        // Calculates angular acceleration for both pendulums
        double angularAcceleration1 = (-GRAVITY*(2*m1+m2)*sin(a1)-m2*GRAVITY*sin(a1-2*a2)-2*sin(a1-a2)*m2*(w2*w2*l2+w1*w1*l1*cos(a1-a2))) / (l1*(2*m1+m2-m2*cos(2*a1-2*a2)));
        double angularAcceleration2 = (2*sin(a1-a2)*(w1*w1*l1*(m1+m2)+GRAVITY*(m1+m2)*cos(a1)+w2*w2*l2*m2*cos(a1-a2))) / (l2*(2*m1+m2-m2*cos(2*a1-2*a2)));

        p1->update(angularAcceleration1, dt);

        // Updates p2's attach point to match p1's mass
        p2->setAttachPoint(p1->getMassPos());

        p2->update(angularAcceleration2, dt);
    }
};

class Trace
{
private:
    int size;
    Vector2 trace[TRACE_SIZE];
    Color color;

public:
    Trace(Color color)
    {
        size = 0;
        this->color = color;
        // Initializes unused traces as -1 for checking
        for(int i = 0; i < TRACE_SIZE; i++) trace[i] = {-1.0f, -1.0f};
    }

    void
    addTrace(Vector2 p)
    {
        // Circular list
        size = (size + 1) % TRACE_SIZE;
        trace[size] = p;
    }

    void
    draw()
    {
        for(int i = 0; i < TRACE_SIZE - 1; i++)
        {
            // Avoid connecting first and last trace
            if (i == size) continue; 

            // Avoids drawing unused traces
            if ((trace[i].x == -1 && trace[i].y == -1) || (trace[i+1].x == -1 && trace[i+1].y == -1)) continue;

            DrawLineEx(trace[i], trace[i+1], 2, color);
        }
    }
};


int
main()
{
    // First pendulum
    Vector2 attachPoint1 = {WIDTH / 2, HEIGHT - 600};
    double angle1 = 3.14159256;
    double angularVelocity1 = -0.5;
    double lenght1 = 300;
    double mass1 = 20;

    Pendulum p1(attachPoint1, angle1, lenght1, angularVelocity1, mass1);


    // Second pendulum
    Vector2 attachPoint2 = p1.getMassPos();
    double angle2 = 3.14159256;
    double angularVelocity2 = 1;
    double lenght2 = 300;
    double mass2 = 20;

    Pendulum p2(attachPoint2, angle2, lenght2, angularVelocity2, mass2);

    DoublePendulum pendulum(p1, p2);

    Trace trace(RED);

    InitWindow(WIDTH, HEIGHT, "Pendulum");

    SetTargetFPS(FPS);

    // Main loop
    while(!WindowShouldClose())
    {
        // Add new trace point
        trace.addTrace(pendulum.getMassPos2());
        // Update pendulum
        pendulum.update(GetFrameTime() * SIMULATION_SPEED);
        
        // Draw
        BeginDrawing();
        if (IsKeyPressed(KEY_S)) {
            // TakeScreenshot deteta automaticamente o formato pelo nome
            TakeScreenshot("scr.png");
            std::cout << "Frame capturado com sucesso!" << std::endl;
        }
        ClearBackground(BLACK);
        trace.draw();
        pendulum.draw();
        EndDrawing();
    }

    return 0;
}