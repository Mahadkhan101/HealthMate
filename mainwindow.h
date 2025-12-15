#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "userprofile.h"
#include "medicalhistoryentry.h"
#include "hydrationtracker.h"
#include "MedicineEntry.h"
#include "labreportentry.h"
#include "healthai.h"

#include <vector>
#include <QMainWindow>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onSaveProfileClicked();
    void onAddHistoryClicked();
    void onRemoveHistoryClicked();
    void onAdd250();
    void onAdd500();
    void onResetHydration();
    void onTargetChanged(int value);
    void updateHydrationUI();
    void onButtonMedAddClicked();
    void onButtonMedRemoveClicked();
    void onAddLabReportClicked();
    void onRemoveLabReportClicked();
    void onAttachFileClicked();
    void refreshLabReportsTable();
    void onAskAI();
    void onClearAI();
    void onGenerateHospitalSummaryClicked();


private:
    Ui::MainWindow *ui;
    UserProfile m_userProfile;
    HydrationTracker m_hydration;
    HealthAI ai;

    vector<LabReportEntry> m_labReports;
    vector <MedicalHistoryEntry> m_medicalHistory;// Stores all medical entries
    vector<MedicineEntry> m_medicineList;  // Stores all medicine entries

    void loadProfileToForm();
    void setupHistoryTable();
    void refreshHistoryTable();

    void loadHistoryFromFile();
    void saveHistoryToFile();
    void saveMedicineList();
    void loadMedicineList();

    void setupLabReportsTable();      // initialize headers, sizing
    void saveLabReportsToFile();      // persist to "labreports.txt"
    void loadLabReportsFromFile();    // load from file

};
#endif // MAINWINDOW_H
