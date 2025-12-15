#ifndef MEDICALHISTORYENTRY_H
#define MEDICALHISTORYENTRY_H
#include <string>
using namespace std;
class MedicalHistoryEntry
{
private:
    string m_date,m_conditionName,m_notes;
public:
      // Default constructor
    MedicalHistoryEntry();
       // Parameterized constructor
    MedicalHistoryEntry(const string &date,const string &conditionName,const string &notes);
      //Getters
    string getDate()const;
    string getConditionName()const;
    string getNotes()const;
      //setters
    void setDate(const string &date);
    void setConditionName(const string &conditionName);
    void setNotes(const string &notes);
};

#endif // MEDICALHISTORYENTRY_H
