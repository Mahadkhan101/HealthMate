#ifndef HYDRATIONTRACKER_H
#define HYDRATIONTRACKER_H
#include <string>
using namespace std;
class HydrationTracker
{
private:
    int m_target,m_consumed;

public:
    HydrationTracker();
    HydrationTracker(int target);

    int getTarget()const;
    int getConsumed()const;
    int getPercentage()const;

    void setTarget(int target);
    void setConsumed(int consumed);

    void addWater(int amount);
    void reset();


    bool saveToFile(const std::string &filePath) const;
    bool loadFromFile(const std::string &filePath);
};

#endif // HYDRATIONTRACKER_H
