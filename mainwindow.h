#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QLCDNumber>
#include <QTimeEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>

// Core Logic Headers
#include "userprofile.h"
#include "medicalhistoryentry.h"
#include "hydrationtracker.h"
#include "MedicineEntry.h"
#include "labreportentry.h"
#include "healthai.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation
    void onNavButtonClicked();

    // Logic Slots (Existing functionality)
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
    void onAskAI();
    void onClearAI();
    void onGenerateHospitalSummaryClicked();

private:
    // UI Setup methods
    void setupModernUI();
    void createSidebar();
    QWidget* createProfileTab();
    QWidget* createHydrationTab();
    QWidget* createHistoryTab();
    QWidget* createLabTab();
    QWidget* createMedicineTab();
    QWidget* createAITab();
    QWidget* createSummaryTab();

    // Modern UI Elements
    QStackedWidget *mainStack;
    QWidget *sidebar;

    // Profile Inputs
    QLineEdit *editName, *editAge, *editEmergency, *editAllergies, *editChronic;
    QComboBox *comboGender;

    // Hydration UI
    QProgressBar *hydrationBar;
    QLCDNumber *consumedDisplay;
    QSpinBox *targetSpin;

    // History UI
    QTableWidget *tableHistory;
    QLineEdit *editHistDate, *editHistCond;
    QTextEdit *textHistNotes;

    // Medicine UI
    QTableWidget *tableMed;
    QLineEdit *editMedName, *editMedDosage;
    QComboBox *comboMedFreq;
    QTimeEdit *timeMed;
    QTextEdit *textMedNotes;

    // Lab UI
    QTableWidget *tableLabs;
    QLineEdit *editLabName, *editLabPlace, *editLabDate, *editLabFile;
    QTextEdit *textLabNotes;

    // AI UI
    QTextEdit *aiResponse, *userQuery;

    // Logic Data
    UserProfile m_userProfile;
    HydrationTracker m_hydration;
    HealthAI ai;
    std::vector<LabReportEntry> m_labReports;
    std::vector<MedicalHistoryEntry> m_medicalHistory;
    std::vector<MedicineEntry> m_medicineList;

    // Persistence
    void loadAllData();
    void saveHistoryToFile();
    void saveMedicineList();
    void saveLabReportsToFile();
    void refreshHistoryTable();
    void refreshLabReportsTable();
    void refreshMedicineTable();
};

#endif
