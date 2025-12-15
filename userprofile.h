#ifndef USERPROFILE_H
#define USERPROFILE_H

#include<string>
using namespace std;

class UserProfile
{
private:
    string m_name,m_gender,m_emergencyContact,m_allergies,m_chronicConditions;
    int m_age;
public:
    // Default constructor
    UserProfile();
    // Parameterized constructor
    UserProfile(const string &name,int age, const string&gender,
                const string&emergencyContact,const string&allergies,const string&chronicConditions);

    //Getters
    string getName()const;
    int getAge()const;
    string getGender()const;
    string getEmergencyContact()const;
    string getAllergies()const;
    string getChronicConditions()const;

    //setters
    void setName(const string&name);
    void setAge(int age);
    void setGender(const string &gender);
    void setEmergencyContact(const string &contact);
    void setAllergies(const string &allergies);
    void setChronicConditions(const string &conditions);
    // Input Output:
    bool saveToFile(const string &filePath)const;
    bool loadFromFile(const string &filePath);
};

#endif // USERPROFILE_H
