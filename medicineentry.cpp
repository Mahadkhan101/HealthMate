#include "medicineentry.h"

MedicineEntry::MedicineEntry() {}
// Constructor
MedicineEntry::MedicineEntry(const QString &n, const QString &d, const QString &f,
                             const QTime &t, const QString &no)
    : name(n), dosage(d), frequency(f), time(t), notes(no)
{}

// Getters
QString MedicineEntry::getName() const { return name; }
QString MedicineEntry::getDosage() const { return dosage; }
QString MedicineEntry::getFrequency() const { return frequency; }
QTime MedicineEntry::getTime() const { return time; }
QString MedicineEntry::getNotes() const { return notes; }

// Setters
void MedicineEntry::setName(const QString &n) { name = n; }
void MedicineEntry::setDosage(const QString &d) { dosage = d; }
void MedicineEntry::setFrequency(const QString &f) { frequency = f; }
void MedicineEntry::setTime(const QTime &t) { time = t; }
void MedicineEntry::setNotes(const QString &no) { notes = no; }
