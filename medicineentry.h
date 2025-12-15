#ifndef MEDICINEENTRY_H
#define MEDICINEENTRY_H
#include <QString>
#include <QTime>


class MedicineEntry
{
private:
    QString name;
    QString dosage;
    QString frequency;
    QTime time;
    QString notes;

public:
    MedicineEntry();
    // Constructor
    MedicineEntry(const QString &n, const QString &d, const QString &f,
                  const QTime &t, const QString &no);

    // Getters
    QString getName() const;
    QString getDosage() const;
    QString getFrequency() const;
    QTime getTime() const;
    QString getNotes() const;

    // Setters
    void setName(const QString &n);
    void setDosage(const QString &d);
    void setFrequency(const QString &f);
    void setTime(const QTime &t);
    void setNotes(const QString &no);

};

#endif // MEDICINEENTRY_H

