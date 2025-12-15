#include "userprofile.h"
#include <fstream>
using namespace std;
UserProfile::UserProfile() : m_age(0){}
UserProfile::UserProfile(const string &name,
                         int age,
                         const string &gender,
                         const string &emergencyContact,
                         const string &allergies,
                         const string &chronicConditions)

    : m_name(name),
    m_age(age),
    m_gender(gender),
    m_emergencyContact(emergencyContact),
    m_allergies(allergies),
    m_chronicConditions(chronicConditions)
{}
//getters
string UserProfile::getName() const {return m_name;}
int UserProfile::getAge()const {return m_age;}
string UserProfile::getGender()const{return m_gender;}
string UserProfile::getEmergencyContact()const{return m_emergencyContact;}
string UserProfile::getAllergies()const{return m_allergies;}
string UserProfile::getChronicConditions()const{return m_chronicConditions;}

//setters
void UserProfile::setName(const string&name){m_name=name;}
void UserProfile::setAge(int age){m_age=age;}
void UserProfile::setGender(const string &gender){m_gender=gender;}
void UserProfile::setEmergencyContact(const string &contact){m_emergencyContact=contact;}
void UserProfile::setAllergies(const string &allergies){m_allergies=allergies;}
void UserProfile::setChronicConditions(const string &conditions){m_chronicConditions=conditions;}

//save to file
bool UserProfile::saveToFile(const string &filePath)const{
    ofstream out(filePath.c_str());
    if(!out)
        return false;

    out<<m_name<<"\n";
    out<<m_age<<"\n";
    out<<m_gender<<"\n";
    out<<m_emergencyContact<<"\n";
    out<<m_allergies<<"\n";
    out<<m_chronicConditions<<"\n";

    return true;
}
bool UserProfile::loadFromFile(const string &filePath){
    ifstream in(filePath.c_str());
    if(!in)return false;

    string name,gender,emergency,allergies,chronic;
    int age;

    if(!std::getline(in,name))return false;
    in>>age;
    in.ignore();
    if(!std::getline(in,gender))return false;
    if(!std::getline(in,emergency))return false;
    if(!std::getline(in,allergies))return false;
    if(!std::getline(in,chronic))return false;

    m_name=name;
    m_age=age;
    m_gender=gender;
    m_emergencyContact=emergency;
    m_allergies=allergies;
    m_chronicConditions=chronic;

    return true;

}
