﻿#ifndef COLLISION_H
#define COLLISION_H
#include "Object.h"
#include <vector>
#include <iostream>
#include <memory>
#include <queue>

class Object; //ac
class MovableObject;
class FixedBall; //ac
class Wall;
class Ball;

class Event;
class CollisionSystem;

class Event
{
    //io
    friend std::ostream operator<<(std::ostream &, const Event &);

public:
    //construct
    Event(Ball &b, Object &o, const float t) : ball(b), object(o), time(t), countBall(b.cnt()), countObject(o.cnt()) {}
    //copy move destruct
    //tochk
    Event(const Event &) = default;
    Event(Event &&) = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&) = default;
    ~Event() = default;

    //information
    float t() { return time; }

    //action
    void handle()
    {
        if ((ball.cnt() == countBall) && (object.cnt() == countObject))
            ball.bounce(object);
    }

    //operator
    bool operator<(const Evnet &event) { return time < event.time; }

private:
    Ball &ball;
    Object &object;
    float time;
    unsigned int countBall, countObject;
};

std::ostream operator<<(std::ostream &os, const Event &event) //todo
{
    os << std::endl;
    return os;
}

class CollisionSystem
{
    friend void draw(CollisionSystem &);
    friend std::istream operator>>(std::istream &, CollisionSystem &);

public:
    //construct and destruct
    CollisionSystem() = default;
    ~CollisionSystem() = default;

    //action
    void run(float);
    void reverse();

private:
    std::vector<Ball> balls;
    std::vector<FixedBall> fixedBalls;
    std::vector<Wall> walls;
    std::priority_queue<Event> events;
    float currentTime;
} std::istream operator>>(std::istream &, CollisionSystem &);

#endif