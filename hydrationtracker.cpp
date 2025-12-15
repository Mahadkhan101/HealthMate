#include "hydrationtracker.h"
#include <fstream>

HydrationTracker::HydrationTracker():m_target(2000),m_consumed(0){}
HydrationTracker::HydrationTracker(int target):m_target(target),m_consumed(0){}

int HydrationTracker::getTarget()const{return m_target;}
int HydrationTracker::getConsumed()const{return m_consumed;}

void HydrationTracker::setTarget(int target){m_target=target;}
void HydrationTracker::setConsumed(int consumed){m_consumed=consumed;}

void HydrationTracker::addWater(int amount) {
    m_consumed += amount;
    if (m_consumed > m_target)
        m_consumed = m_target;
}
void HydrationTracker::reset(){
    m_consumed=0;
}
int HydrationTracker::getPercentage()const{
    if (m_target==0)return 0;
    return ((m_consumed*100)/m_target);
}

bool HydrationTracker::saveToFile(const std::string &filePath) const
{
    std::ofstream out(filePath);
    if (!out) return false;

    out << m_target << "\n";
    out << m_consumed << "\n";

    return true;
}
bool HydrationTracker::loadFromFile(const std::string &filePath)
{
    std::ifstream in(filePath);
    if (!in) return false;

    in >> m_target;
    in >> m_consumed;

    return true;
}
