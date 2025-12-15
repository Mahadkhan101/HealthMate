#include "labreportentry.h"

// Default constructor
LabReportEntry::LabReportEntry()
    : m_reportName(""),
    m_labName(""),
    m_date(""),
    m_notes(""),
    m_filePath("")
{}

// Parameterized constructor
LabReportEntry::LabReportEntry(const string &reportName,
                               const string &labName,
                               const string &date,
                               const string &notes,
                               const string &filePath)
    : m_reportName(reportName),
    m_labName(labName),
    m_date(date),
    m_notes(notes),
    m_filePath(filePath)
{}

// Getters
string LabReportEntry::getReportName() const { return m_reportName; }
string LabReportEntry::getLabName() const    { return m_labName; }
string LabReportEntry::getDate() const       { return m_date; }
string LabReportEntry::getNotes() const      { return m_notes; }
string LabReportEntry::getFilePath() const   { return m_filePath; }

// Setters
void LabReportEntry::setReportName(const string &reportName) { m_reportName = reportName; }
void LabReportEntry::setLabName(const string &labName)       { m_labName = labName; }
void LabReportEntry::setDate(const string &date)             { m_date = date; }
void LabReportEntry::setNotes(const string &notes)           { m_notes = notes; }
void LabReportEntry::setFilePath(const string &filePath)     { m_filePath = filePath; }
