#include "medicalhistoryentry.h"

 // Default constructor
MedicalHistoryEntry::MedicalHistoryEntry() {}
  // Parameterized constructor
MedicalHistoryEntry::MedicalHistoryEntry(const string &date,
                                         const string &conditionName,
                                         const string &notes)
    :m_date(date),
    m_conditionName(conditionName),
    m_notes(notes){}

//Getters
string MedicalHistoryEntry::getDate()const{return m_date;}
string MedicalHistoryEntry::getConditionName()const{return m_conditionName;}
string MedicalHistoryEntry::getNotes()const{return m_notes;}
//setters
void MedicalHistoryEntry::setDate(const string &date){m_date=date;}
void MedicalHistoryEntry::setConditionName(const string &conditionName){m_conditionName=conditionName;}
void MedicalHistoryEntry::setNotes(const string &notes){m_notes=notes;}

