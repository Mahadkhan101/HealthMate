#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QPdfWriter>
#include <QPainter>
#include <QDate>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::onSaveProfileClicked);

    connect(ui->pushButtonAddHistory,&QPushButton::clicked,
            this,&MainWindow::onAddHistoryClicked);

    connect(ui->pushButtonRemoveHistory,&QPushButton::clicked,
            this,&MainWindow::onRemoveHistoryClicked);

    connect(ui->add250Button, &QPushButton::clicked,
            this, &MainWindow::onAdd250);

    connect(ui->add500Button, &QPushButton::clicked,
            this, &MainWindow::onAdd500);

    connect(ui->resetHydrationButton, &QPushButton::clicked,
            this, &MainWindow::onResetHydration);

    connect(ui->targetSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onTargetChanged);

    connect(ui->buttonMedAdd, &QPushButton::clicked,
            this, &MainWindow::onButtonMedAddClicked);

    connect(ui->buttonMedRemove, &QPushButton::clicked,
            this, &MainWindow::onButtonMedRemoveClicked);

    connect(ui->btnAddLabReport,    &QPushButton::clicked,
            this, &MainWindow::onAddLabReportClicked);

    connect(ui->btnRemoveLabReport, &QPushButton::clicked,
            this, &MainWindow::onRemoveLabReportClicked);

    connect(ui->btnAttachFile,      &QPushButton::clicked,
            this, &MainWindow::onAttachFileClicked);

    connect(ui->btnAskAI,       &QPushButton::clicked,
            this, &MainWindow::onAskAI);
    connect(ui->btnClearAI,      &QPushButton::clicked,
            this, &MainWindow::onClearAI);
    connect(ui->btnGenerateHospitalPDF, &QPushButton::clicked,
            this, &MainWindow::onGenerateHospitalSummaryClicked);

    setupHistoryTable();
    if(m_userProfile.loadFromFile("profile.txt"))
    {loadProfileToForm();}

    loadHistoryFromFile();
    refreshHistoryTable();
    if (m_hydration.loadFromFile("hydration.txt"))
    {
        ui->targetSpinBox->setValue(m_hydration.getTarget());
        updateHydrationUI();
    }
    ui->tableWidgetLabReports->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetMedicine->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    loadMedicineList();
    setupLabReportsTable();
    loadLabReportsFromFile();
    refreshLabReportsTable();

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onSaveProfileClicked()
{

    QString name      = ui->lineEditName->text();
    QString ageText   = ui->lineEditAge->text();
    QString gender    = ui->comboBoxGender->currentText();
    QString emergency = ui->lineEditEmergency->text();
    QString allergies = ui->lineEditAllergies->text();
    QString chronic   = ui->lineEditChronic->text();

    int age = ageText.toInt();

    m_userProfile.setName(name.toStdString());
    m_userProfile.setAge(age);
    m_userProfile.setGender(gender.toStdString());
    m_userProfile.setEmergencyContact(emergency.toStdString());
    m_userProfile.setAllergies(allergies.toStdString());
    m_userProfile.setChronicConditions(chronic.toStdString());


     if (m_userProfile.saveToFile("profile.txt")) {
         QMessageBox::information(this, "Profile Saved", "Profile saved successfully.");
    } else {
         QMessageBox::warning(this, "Error", "Could not save profile.");
     }
}
void MainWindow::loadProfileToForm(){
    ui->lineEditName->setText(QString::fromStdString(m_userProfile.getName()));
    ui->lineEditAge->setText(QString::number(m_userProfile.getAge()));
    ui->comboBoxGender->setCurrentText(QString::fromStdString(m_userProfile.getGender()));
    ui->lineEditEmergency->setText(QString::fromStdString(m_userProfile.getEmergencyContact()));
    ui->lineEditAllergies->setText(QString::fromStdString(m_userProfile.getAllergies()));
    ui->lineEditChronic->setText(QString::fromStdString(m_userProfile.getChronicConditions()));
}
void MainWindow::setupHistoryTable(){
    ui->tableWidgetHistory->setColumnCount(3);
    QStringList headers;
    headers<<"Date"<<"Condition"<<"Notes";
    ui->tableWidgetHistory->setHorizontalHeaderLabels(headers);

    ui->tableWidgetHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetHistory->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidgetHistory->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
}
void MainWindow::onAddHistoryClicked()
{

    QString date      = ui->lineEditHistoryDate->text();
    QString condition = ui->lineEditHistoryCondition->text();
    QString notes     = ui->textEditHistoryNotes->toPlainText();


    if (date.isEmpty() || condition.isEmpty()) {
        QMessageBox::warning(this, "Missing Data",
                             "Please enter at least Date and Condition.");
        return;
    }


    MedicalHistoryEntry entry(
        date.toStdString(),
        condition.toStdString(),
        notes.toStdString()
        );

    m_medicalHistory.push_back(entry);

    int row = ui->tableWidgetHistory->rowCount();
    ui->tableWidgetHistory->insertRow(row);

    ui->tableWidgetHistory->setItem(row,0,
                                    new QTableWidgetItem(date));
    ui->tableWidgetHistory->setItem(row, 1,
                                    new QTableWidgetItem(condition));
    ui->tableWidgetHistory->setItem(row, 2,
                                    new QTableWidgetItem(notes));

    ui->lineEditHistoryDate->clear();
    ui->lineEditHistoryCondition->clear();
    ui->textEditHistoryNotes->clear();

    saveHistoryToFile();

}
void MainWindow::saveHistoryToFile()
{
    ofstream out("history.txt");
    if (!out) {return;
    }

    for (const MedicalHistoryEntry &entry : m_medicalHistory) {
        out << entry.getDate() << '\n';
        out << entry.getConditionName() << '\n';
        out << entry.getNotes() << '\n';
    }
}
void MainWindow::refreshHistoryTable()
{
    ui->tableWidgetHistory->setRowCount(0);

    for (const MedicalHistoryEntry &entry : m_medicalHistory) {
        int row = ui->tableWidgetHistory->rowCount();
        ui->tableWidgetHistory->insertRow(row);

        ui->tableWidgetHistory->setItem(row, 0,
                                        new QTableWidgetItem(QString::fromStdString(entry.getDate())));
        ui->tableWidgetHistory->setItem(row, 1,
                                        new QTableWidgetItem(QString::fromStdString(entry.getConditionName())));
        ui->tableWidgetHistory->setItem(row, 2,
                                        new QTableWidgetItem(QString::fromStdString(entry.getNotes())));
    }
}
void MainWindow::onRemoveHistoryClicked()
{
    int row = ui->tableWidgetHistory->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No selection",
                             "Please select a row to remove.");
        return;
    }

    // Remove from vector (same index) static cast=ai used
    //also e.g if the user selected row = 2, you erase the 3rd element of vector array :
    if (row >= 0 && row < static_cast<int>(m_medicalHistory.size())) {
        m_medicalHistory.erase(m_medicalHistory.begin() + row);
    }


    ui->tableWidgetHistory->removeRow(row);

    saveHistoryToFile();
}

void MainWindow::loadHistoryFromFile()
{
    m_medicalHistory.clear();

    ifstream in("history.txt");
    if (!in) {
        return;
    }

    string date;
    string condition;
    string notes;

    while (true) {
        if (!getline(in, date)) break;
        if (!getline(in, condition)) break;
        if (!getline(in, notes)) break;

        MedicalHistoryEntry entry(date, condition, notes);
        m_medicalHistory.push_back(entry);
    }
}
void MainWindow::onAdd250()
{
    m_hydration.addWater(250);
    m_hydration.saveToFile("hydration.txt");
    updateHydrationUI();
}
void MainWindow::onAdd500()
{
    m_hydration.addWater(500);
    m_hydration.saveToFile("hydration.txt");
    updateHydrationUI();
}
void MainWindow::onResetHydration()
{
    m_hydration.reset();
    m_hydration.saveToFile("hydration.txt");
    updateHydrationUI();
}
void MainWindow::updateHydrationUI()
{
    int consumed = m_hydration.getConsumed();
    int percent  = m_hydration.getPercentage();

    ui->hydrationProgressBar->setValue(percent);

    if (ui->consumedLCD)
        ui->consumedLCD->display(consumed);
}
void MainWindow::onTargetChanged(int value)
{
    m_hydration.setTarget(value);
    m_hydration.saveToFile("hydration.txt");
    updateHydrationUI();
}
void MainWindow::onButtonMedAddClicked()
{
    QString name = ui->lineEditMedName->text();
    QString dosage = ui->lineEditMedDosage->text();
    QString frequency = ui->comboBoxFrequency->currentText();
    QTime time = ui->timeEditMedTime->time();
    QString notes = ui->textEditMedNotes->toPlainText();

    // Validation
    if (name.isEmpty() || dosage.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter medicine name and dosage.");
        return;
    }


    MedicineEntry med(name, dosage, frequency, time, notes);

    m_medicineList.push_back(med);

    int row = ui->tableWidgetMedicine->rowCount();
    ui->tableWidgetMedicine->insertRow(row);
    ui->tableWidgetMedicine->setItem(row, 0,
                                     new QTableWidgetItem(med.getName()));
    ui->tableWidgetMedicine->setItem(row, 1,
                                     new QTableWidgetItem(med.getDosage()));
    ui->tableWidgetMedicine->setItem(row, 2,
                                     new QTableWidgetItem(med.getFrequency()));
    ui->tableWidgetMedicine->setItem(row, 3,
                                     new QTableWidgetItem(med.getTime().toString("hh:mm")));
    ui->tableWidgetMedicine->setItem(row, 4,
                                     new QTableWidgetItem(med.getNotes()));


    ui->lineEditMedName->clear();
    ui->lineEditMedDosage->clear();
    ui->comboBoxFrequency->setCurrentIndex(0);
    ui->timeEditMedTime->setTime(QTime::currentTime());
    ui->textEditMedNotes->clear();

     saveMedicineList();
}
void MainWindow::onButtonMedRemoveClicked()
{
    int row = ui->tableWidgetMedicine->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a medicine to remove.");
        return;
    }

    // Remove from vector
    if (row < static_cast<int>(m_medicineList.size())) {
        m_medicineList.erase(m_medicineList.begin() + row);
    }


    ui->tableWidgetMedicine->removeRow(row);
       saveMedicineList();

}
void MainWindow::saveMedicineList()
{
    ofstream out("medicines.txt");
    if (!out.is_open()) {
        QMessageBox::warning(this, "Save Error", "Cannot open file for writing.");
        return;
    }

    for (const auto &med : m_medicineList) {
        out << med.getName().toStdString() << "\n";
        out << med.getDosage().toStdString() << "\n";
        out << med.getFrequency().toStdString() << "\n";
        out << med.getTime().toString("hh:mm").toStdString() << "\n";
        out << med.getNotes().toStdString() << "\n";
    }

    out.close();
}
void MainWindow::loadMedicineList()
{
    m_medicineList.clear();
    ui->tableWidgetMedicine->setRowCount(0);

   ifstream in("medicines.txt");
    if (!in.is_open()) {
        return;
    }

    string name, dosage, frequency, timeStr, notes;

    while (true) {
        if (!getline(in, name)) break;
        if (!getline(in, dosage)) break;
        if (!getline(in, frequency)) break;
        if (!getline(in, timeStr)) break;
        if (!getline(in, notes)) break;

        QString qName = QString::fromStdString(name);
        QString qDosage = QString::fromStdString(dosage);
        QString qFrequency = QString::fromStdString(frequency);
        QTime qTime = QTime::fromString(QString::fromStdString(timeStr), "hh:mm"); //ai used
        QString qNotes = QString::fromStdString(notes);

        // Create MedicineEntry
        MedicineEntry med(qName, qDosage, qFrequency, qTime, qNotes);
        m_medicineList.push_back(med);

        // Add to table
        int row = ui->tableWidgetMedicine->rowCount();
        ui->tableWidgetMedicine->insertRow(row);
        ui->tableWidgetMedicine->setItem(row, 0, new QTableWidgetItem(qName));
        ui->tableWidgetMedicine->setItem(row, 1, new QTableWidgetItem(qDosage));
        ui->tableWidgetMedicine->setItem(row, 2, new QTableWidgetItem(qFrequency));
        ui->tableWidgetMedicine->setItem(row, 3, new QTableWidgetItem(qTime.toString("hh:mm")));
        ui->tableWidgetMedicine->setItem(row, 4, new QTableWidgetItem(qNotes));
    }

}
void MainWindow::setupLabReportsTable()
{
    ui->tableWidgetLabReports->setColumnCount(5);
    QStringList headers;
    headers << "Report Name" << "Lab Name" << "Date" << "Notes" << "Attached File";
    ui->tableWidgetLabReports->setHorizontalHeaderLabels(headers);

    ui->tableWidgetLabReports->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetLabReports->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetLabReports->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidgetLabReports->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::refreshLabReportsTable()
{
    ui->tableWidgetLabReports->setRowCount(0);

    for (const LabReportEntry &report : m_labReports) {
        int row = ui->tableWidgetLabReports->rowCount();
        ui->tableWidgetLabReports->insertRow(row);

        ui->tableWidgetLabReports->setItem(row, 0,
                                           new QTableWidgetItem(QString::fromStdString(report.getReportName())));
        ui->tableWidgetLabReports->setItem(row, 1,
                                           new QTableWidgetItem(QString::fromStdString(report.getLabName())));
        ui->tableWidgetLabReports->setItem(row, 2,
                                           new QTableWidgetItem(QString::fromStdString(report.getDate())));
        ui->tableWidgetLabReports->setItem(row, 3,
                                           new QTableWidgetItem(QString::fromStdString(report.getNotes())));
        ui->tableWidgetLabReports->setItem(row, 4,
                                           new QTableWidgetItem(QString::fromStdString(report.getFilePath())));
    }
}

void MainWindow::onAddLabReportClicked()
{
    QString reportName = ui->lineEditLabReportName->text();
    QString labName    = ui->lineEditLabName->text();
    QString date       = ui->lineEditLabDate->text();
    QString notes      = ui->textEditLabNotes->toPlainText();
    QString filePath   = ui->lineEditLabFile->text();

    if(reportName.isEmpty() || labName.isEmpty() || date.isEmpty()) {
        QMessageBox::warning(this, "Missing Data", "please enter Report Name, Lab Name, and Date.");
        return;
    }

    LabReportEntry entry(reportName.toStdString(),
                         labName.toStdString(),
                         date.toStdString(),
                         notes.toStdString(),
                         filePath.toStdString());

    m_labReports.push_back(entry);
    refreshLabReportsTable();
    saveLabReportsToFile();

    ui->lineEditLabReportName->clear();
    ui->lineEditLabName->clear();
    ui->lineEditLabDate->clear();
    ui->textEditLabNotes->clear();
    ui->lineEditLabFile->clear();
}

void MainWindow::onRemoveLabReportClicked()
{
    int row = ui->tableWidgetLabReports->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "No selection", "Please select a row to remove.");
        return;
    }

    if(row >= 0 && row < static_cast<int>(m_labReports.size()))
        m_labReports.erase(m_labReports.begin() + row);

    refreshLabReportsTable();
    saveLabReportsToFile();
}

void MainWindow::onAttachFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Lab Report File");
    if(!fileName.isEmpty()) {
        ui->lineEditLabFile->setText(fileName);
    }
}
void MainWindow::saveLabReportsToFile()
{
    ofstream out("labreports.txt");
    if(!out) return;

    for(const LabReportEntry &r : m_labReports) {
        out << r.getReportName() << "\n";
        out << r.getLabName() << "\n";
        out << r.getDate() << "\n";
        out << r.getNotes() << "\n";
        out << r.getFilePath() << "\n";
    }
}

void MainWindow::loadLabReportsFromFile()
{
    m_labReports.clear();
    ifstream in("labreports.txt");
    if(!in) return;

    string reportName, labName, date, notes, filePath;
    while(getline(in, reportName)) {
        if(!getline(in, labName)) break;
        if(!getline(in, date)) break;
        if(!getline(in, notes)) break;
        if(!getline(in, filePath)) break;

        LabReportEntry entry(reportName, labName, date, notes, filePath);
        m_labReports.push_back(entry);
    }
}
void MainWindow::onAskAI()
{
    QString question = ui->textEditUserQuestion->toPlainText().trimmed();
    if (question.isEmpty()) {
         QMessageBox::warning(this, "Input Error", "please enter a question.");
        return;
    }
    QString response = ai.answerQuestion(question);
    ui->textEditAiResponse->setPlainText(response);
}

void MainWindow::onClearAI()
{
    ui->textEditUserQuestion->clear();
    ui->textEditAiResponse->clear();
}
void MainWindow::onGenerateHospitalSummaryClicked()
{
    QString file1 = QFileDialog::getSaveFileName(
        this, "Save Patient Summary", "Patient_Summary.pdf", "PDF Files (*.pdf)"
        );
    if (file1.isEmpty()) return;

    QPdfWriter pdf(file1);
    pdf.setPageSize(QPageSize::A4);
    pdf.setResolution(96);

    QPainter painter(&pdf);


    const int margin = 40;
    const int pageHeight = pdf.height() - margin;
    int y = 60;
    const int lineHeight = 30;


    auto writeLine = [&](const QString &text, int x = 40, bool isTitle = false) {
        // Check if we need a new page
        if (y + lineHeight > pageHeight) {
            pdf.newPage();
            y = 60; // Reset to top margin
        }


        if(isTitle) painter.setFont(QFont("Times", 14, QFont::Bold));
        else painter.setFont(QFont("Times", 11));

        painter.drawText(QRect(x, y, pdf.width() - 80, lineHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         text);
        y += lineHeight;
    };

    painter.setFont(QFont("Times", 14, QFont::Bold));
    painter.drawText(QRect(0, y, pdf.width(), 40), Qt::AlignCenter, "Patient Medical Summary");
    y += 60;

    writeLine("PATIENT PROFILE", 40, true); // Bold header
    writeLine("Name: " + QString::fromStdString(m_userProfile.getName()), 60);
    writeLine("Age: " + QString::number(m_userProfile.getAge()), 60);
    writeLine("Gender: " + QString::fromStdString(m_userProfile.getGender()), 60);
    writeLine("Emergency Contact: " + QString::fromStdString(m_userProfile.getEmergencyContact()), 60);
    y += lineHeight;


    writeLine("MEDICAL HISTORY", 40, true);
    for (const auto &h : m_medicalHistory) {
        writeLine(QString::fromStdString(h.getDate()) + " - " +
                      QString::fromStdString(h.getConditionName()), 60);
    }
    y += lineHeight;

    // ===== Medicines =====
    writeLine("CURRENT MEDICINES", 40, true);
    for (const auto &m : m_medicineList) {
        writeLine(m.getName() + " | " + m.getDosage() + " | " + m.getFrequency(), 60);
    }
    y += lineHeight;

    painter.end();
    QMessageBox::information(this, "Done", "Patient summary PDF created.");
}
