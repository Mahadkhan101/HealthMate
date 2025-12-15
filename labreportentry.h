#ifndef LABREPORTENTRY_H
#define LABREPORTENTRY_H
#include <string>
using namespace std;
class LabReportEntry
{
private:
    string m_reportName;
    string m_labName;
    string m_date;       // store as "YYYY-MM-DD" string for simplicity
    string m_notes;
    string m_filePath;   // path to attached file (PDF, JPG, etc.)

public:
    LabReportEntry();
    LabReportEntry(const string &reportName,
                   const string &labName,
                   const string &date,
                   const string &notes,
                   const string &filePath);

    // Getters
    string getReportName() const;
    string getLabName() const;
    string getDate() const;
    string getNotes() const;
    string getFilePath() const;

    // Setters
    void setReportName(const string &reportName);
    void setLabName(const string &labName);
    void setDate(const string &date);
    void setNotes(const string &notes);
    void setFilePath(const string &filePath);

};

#endif // LABREPORTENTRY_H
