﻿#include "Collision.h"
#include "Object.h"
#include <iomanip>
#include <memory>

int sumbounce = 0;
int sumexam = 0;



//#############
//Event
//#############
#ifdef EVENT_DRIVEN
void Event::handle() const //notice
{
    if ((ball->cnt() == countBall) && (object->cnt() == countObject))
        ball->bounce(*object); //从指针变成对象的引用，交给ball类运行时绑定
    std::clog << "::::::::::::::::::::::::::::::Event handle: " << *this << std::endl;
}

bool Event::valid() const
{
    return (ball->cnt() == countBall) && (object->cnt() == countObject);
}

std::ostream &operator<<(std::ostream &os, const Event &event)
{
    os << std::setprecision(3) << std::fixed;
    os << std::setw(8) << event.time
       << " | " << std::setw(4) << event.ball->num()
       << " | " << std::setw(3) << event.countBall
       << " | " << std::setw(10);
    switch (event.object->type())
    {
    case Object_type::BALL:
        os << "ball";
        break;
    case Object_type::WALL:
        os << "wall";
        break;
    }
    os << " | " << std::setw(3) << event.object->num()
       << " | " << std::setw(3) << event.countObject
       << std::defaultfloat;
    return os;
}

std::ostream &operator<<(std::ostream &os, std::priority_queue<Event, std::vector<Event>> eventqueue) //time consuming
{
    os << "---------------Events-------------------------" << std::endl;
    os << "   time  | Ball | cnt | Obj_type  | Obj | cnt"
       //"    1.000 |    1 |   3 | fixedball |   1 |   3"
       << std::endl;
    while (!eventqueue.empty())
    {
        os << eventqueue.top() << std::endl;
        eventqueue.pop();
    }
    return os;
}
#endif

//#############
//CollisionSystem
//#############

void CollisionSystem::run(float t)
{
    float targetTime = currentTime + t;
#ifdef TIME_DRIVEN
    while (currentTime < targetTime)
    {
        for (auto i = balls.cbegin(); i != balls.cend(); i++)
        {
            for (auto j = i + 1; j != balls.cend(); j++)
            {
                sumexam++;
                if ((**i).examine(**j))
                {
                    (**i).bounce(**j);
                    sumbounce++;
                }
            }
            for (auto j = walls.cbegin(); j != walls.cend();j++)
            {
                sumexam++;
                if((**i).examine(**j))
                {
                    std::cout << "！！！！！！！！！！！！！！！！！！！！exam passed" << std::endl;
                    (**i).bounce(**j);
                    sumbounce++;
                }
            }
        }
        move(DELTATIME);
    }
#endif

#ifdef EVENT_DRIVEN
    while (!(eventQueue.empty()) &&  (targetTime >= eventQueue.top().time)) //队列非空、事件有效且发生
    {//notice:处理事件以后记得刷新小球位置
        if (!eventQueue.top().valid())
        {
            eventQueue.pop();
            continue;
        }
        move(eventQueue.top().t() - currentTime); //跳转到事件发生时间
        Event temp = eventQueue.top();//提出放置，因为需要检测对应物体
        eventQueue.top().handle(); //处理事件，处理时小球自动递增计数器
        eventQueue.pop();
        std::cout << currentTime << std::endl;

        { //主小球检测
            for (auto const &i : balls)
                if ((t = (temp.ball->predict(*i)) >= 0))//predict内置重复检测
                    eventQueue.push(Event(temp.ball, i, t + currentTime));
            for (auto const &i : walls)
                if ((temp.object!=i)&&(t = (temp.ball->predict(*i)) >= 0))//智能指针重复检测
                    eventQueue.push(Event(temp.ball, i, t + currentTime));
        }

        if (temp.object->type() == Object_type::BALL) //副小球检测
        {
            std::shared_ptr<Ball> ball = std::dynamic_pointer_cast<Ball>(temp.object);//智能指针转换
            for (auto const &i : balls)
                if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));
            for (auto const &i : walls)
                if ((t = (ball->predict(*i)) > 0))
                    eventQueue.push(Event(ball, i, t + currentTime));
        }

        while ((!eventQueue.empty()) && (!temp.valid()))
            eventQueue.pop(); //清理后续无效事件，方便下一次操作

        std::clog << eventQueue;
    }
    //发生的事件全部处理完成
    move(targetTime - currentTime);
#endif
}

void CollisionSystem::reverse()
{
#ifdef EVENT_DRIVEN
    while (!eventQueue.empty())
        eventQueue.pop(); //清空
    #ifdef DEBUG
        OUTPUT << eventQueue;
    #endif
#endif

    for (auto &i : balls)
        i->rev();

#ifdef EVENT_DRIVEN
    this->init();
#endif
}




//#############
//private
//#############
void CollisionSystem::init()
{
    float t;
    for (auto i = balls.cbegin(); i != balls.cend(); i++)
    {
        for (auto j = i + 1; j != balls.cend(); j++)
            if ((**i).examine(**j))
            {
                std::cerr << "Error:system init crossed" << std::endl;
                exit(EXIT_FAILURE);
            }
    }
#ifdef EVENT_DRIVEN
    for (auto i = balls.cbegin(); i != balls.cend(); i++)
    {
        for (auto j = i + 1; j != balls.cend(); j++)
        {
            if ((t = ((**i).predict(**j)) >= 0))
                eventQueue.push(Event(*i, *j, t + currentTime));
        }
        for (auto const& j : walls)
            if ((t = ((**i).predict(*j)) >= 0))
                eventQueue.push(Event(*i, j, t + currentTime));
    }
#endif
}

void CollisionSystem::move(float t)
{
    for (auto &i : balls)
        i->move(t);
    currentTime += t;
}

//#############
//operator
//#############
std::istream &operator>>(std::istream &is, CollisionSystem &system)
{
    char identifier;
    int num;
    while (is >> identifier) //iden//如果读到文件结尾eof自动离开主循环
    {

        if (!(is >> num)) //num
        {
            std::cerr << "Error:system input" << std::endl;
#ifdef DEBUG
            std::cerr << is.eof() << is.bad() << is.fail() << is.good() << std::endl;
#endif
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i != num; i++)
        {
            switch (identifier)
            {
            case 'B':
            {
                system.balls.push_back(std::make_shared<Ball>(is));
                break;
            }
            case 'W':
            {
                system.walls.push_back(std::make_shared<Wall>(is));
                break;
            }
            }
        }
    }
    std::clog << "Success:systen read in" << std::endl;
    
    system.init();//notice
    return is;
}

std::ostream &operator<<(std::ostream &os, CollisionSystem &system)
{
    if (!system.balls.empty())
        os << system.balls;
    if (!system.walls.empty())
        os << system.walls;
#ifdef EVENT_DRIVEN
    if(!system.eventQueue.empty())
    os << system.eventQueue;
#endif
    return os;
}