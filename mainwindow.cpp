#include "mainwindow.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QDate>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupModernUI();
    loadAllData();
}

MainWindow::~MainWindow() {}

void MainWindow::setupModernUI() {
    // High-Tech Global Stylesheet
    this->setStyleSheet(R"(
        QMainWindow { background-color: #0F172A; }
        QWidget#Sidebar {
            background-color: #1E293B;
            border-right: 1px solid #334155;
        }
        QLabel#HeaderLabel {
            color: #38BDF8; font-size: 18px; font-weight: bold; padding: 10px;
        }
        QPushButton#NavBtn {
            background: transparent; color: #94A3B8;
            border: none; padding: 15px; text-align: left;
            font-size: 13px; font-weight: bold; border-left: 3px solid transparent;
        }
        QPushButton#NavBtn:hover { background-color: #334155; color: #38BDF8; }
        QPushButton#NavBtn:checked {
            background-color: #0EA5E9; color: white; border-left: 3px solid #38BDF8;
        }
        QTableWidget {
            background-color: #1E293B; color: white; gridline-color: #334155;
            border: 1px solid #334155; border-radius: 8px; font-size: 10pt;
        }
        QHeaderView::section { background-color: #0F172A; color: #38BDF8; padding: 8px; border: none; font-weight: bold; }
        QLineEdit, QTextEdit, QSpinBox, QTimeEdit, QComboBox {
            background-color: #334155; border: 1px solid #475569;
            border-radius: 6px; color: white; padding: 8px; selection-background-color: #0EA5E9;
        }
        QPushButton#ActionBtn {
            background-color: #10B981; color: white; border-radius: 6px;
            padding: 10px; font-weight: bold; font-size: 11pt;
        }
        QPushButton#ActionBtn:hover { background-color: #059669; }
        QPushButton#SecondaryBtn {
            background-color: #EF4444; color: white; border-radius: 6px; padding: 10px; font-weight: bold;
        }
        QProgressBar {
            border: 1px solid #334155; border-radius: 12px; background: #1E293B;
            text-align: center; color: white; font-weight: bold; height: 25px;
        }
        QProgressBar::chunk { background-color: #0EA5E9; border-radius: 12px; }
        QLabel { color: #E2E8F0; font-weight: 500; }
    )");

    QWidget *central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // --- Sidebar Setup ---
    sidebar = new QWidget();
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(220);
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);

    QLabel *logo = new QLabel("HEALTHMATE AI");
    logo->setObjectName("HeaderLabel");
    logo->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(logo);
    sideLayout->addSpacing(20);

    QStringList navItems = {"Profile", "Hydration", "Medical History", "Lab Reports", "Medicines", "AI Assistant", "Summary"};
    for(int i=0; i<navItems.size(); ++i) {
        QPushButton *btn = new QPushButton("  " + navItems[i]);
        btn->setObjectName("NavBtn");
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        if(i==0) btn->setChecked(true);
        btn->setProperty("index", i);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onNavButtonClicked);
        sideLayout->addWidget(btn);
    }
    sideLayout->addStretch();

    // --- Stacked Content Area ---
    mainStack = new QStackedWidget();
    mainStack->addWidget(createProfileTab());
    mainStack->addWidget(createHydrationTab());
    mainStack->addWidget(createHistoryTab());
    mainStack->addWidget(createLabTab());
    mainStack->addWidget(createMedicineTab());
    mainStack->addWidget(createAITab());
    mainStack->addWidget(createSummaryTab());

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(mainStack);
    setCentralWidget(central);
    setWindowTitle("HealthMate AI Pro - v2.0");
    resize(1100, 750);
}

// --- TAB BUILDERS ---

QWidget* MainWindow::createProfileTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(50, 50, 50, 50);

    QLabel *t = new QLabel("Personal Bio-Data");
    t->setStyleSheet("font-size: 22px; color: #38BDF8; margin-bottom: 20px;");
    lay->addWidget(t);

    editName = new QLineEdit(); editName->setPlaceholderText("Full Name");
    editAge = new QLineEdit(); editAge->setPlaceholderText("Age");
    comboGender = new QComboBox(); comboGender->addItems({"MALE", "FEMALE", "OTHER"});
    editEmergency = new QLineEdit(); editEmergency->setPlaceholderText("Emergency Phone Number");
    editAllergies = new QLineEdit(); editAllergies->setPlaceholderText("Allergies (e.g. Peanuts, Penicillin)");
    editChronic = new QLineEdit(); editChronic->setPlaceholderText("Chronic Conditions (e.g. Diabetes)");

    lay->addWidget(new QLabel("Full Name")); lay->addWidget(editName);
    lay->addWidget(new QLabel("Age")); lay->addWidget(editAge);
    lay->addWidget(new QLabel("Gender Identity")); lay->addWidget(comboGender);
    lay->addWidget(new QLabel("Allergies")); lay->addWidget(editAllergies);
    lay->addWidget(new QLabel("Chronic Conditions")); lay->addWidget(editChronic);
    lay->addWidget(new QLabel("Emergency Contact")); lay->addWidget(editEmergency);

    QPushButton *saveBtn = new QPushButton("SAVE PROFILE TO SECURE STORAGE");
    saveBtn->setObjectName("ActionBtn");
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveProfileClicked);
    lay->addWidget(saveBtn);
    lay->addStretch();
    return w;
}

QWidget* MainWindow::createHydrationTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(50, 50, 50, 50);

    consumedDisplay = new QLCDNumber();
    consumedDisplay->setSegmentStyle(QLCDNumber::Flat);
    consumedDisplay->setMinimumHeight(150);
    consumedDisplay->setStyleSheet("background: #1E293B; border: 2px solid #0EA5E9; color: #38BDF8;");

    hydrationBar = new QProgressBar();
    targetSpin = new QSpinBox();
    targetSpin->setRange(500, 10000);
    targetSpin->setSingleStep(250);
    targetSpin->setSuffix(" ml");

    lay->addWidget(new QLabel("TOTAL CONSUMED (ML)"));
    lay->addWidget(consumedDisplay);
    lay->addWidget(new QLabel("PROGRESS TOWARDS GOAL"));
    lay->addWidget(hydrationBar);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *b1 = new QPushButton("ADD 250ML"); b1->setObjectName("ActionBtn");
    QPushButton *b2 = new QPushButton("ADD 500ML"); b2->setObjectName("ActionBtn");
    connect(b1, &QPushButton::clicked, this, &MainWindow::onAdd250);
    connect(b2, &QPushButton::clicked, this, &MainWindow::onAdd500);
    btns->addWidget(b1); btns->addWidget(b2);

    lay->addLayout(btns);
    lay->addWidget(new QLabel("ADJUST DAILY TARGET"));
    lay->addWidget(targetSpin);

    QPushButton *res = new QPushButton("RESET DAILY TRACKER");
    res->setObjectName("SecondaryBtn");
    connect(res, &QPushButton::clicked, this, &MainWindow::onResetHydration);
    lay->addWidget(res);
    lay->addStretch();

    connect(targetSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onTargetChanged);
    return w;
}

QWidget* MainWindow::createHistoryTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    tableHistory = new QTableWidget(0, 3);
    tableHistory->setHorizontalHeaderLabels({"Date", "Condition", "Clinical Notes"});
    tableHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    editHistDate = new QLineEdit(); editHistDate->setPlaceholderText("YYYY-MM-DD");
    editHistCond = new QLineEdit(); editHistCond->setPlaceholderText("Condition Name");
    textHistNotes = new QTextEdit(); textHistNotes->setPlaceholderText("Detailed symptoms or doctor's advice...");
    textHistNotes->setMaximumHeight(100);

    lay->addWidget(new QLabel("MEDICAL RECORDS LOG"));
    lay->addWidget(tableHistory);
    lay->addWidget(new QLabel("ADD NEW ENTRY"));
    lay->addWidget(editHistDate);
    lay->addWidget(editHistCond);
    lay->addWidget(textHistNotes);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *addBtn = new QPushButton("ADD RECORD"); addBtn->setObjectName("ActionBtn");
    QPushButton *remBtn = new QPushButton("DELETE SELECTED"); remBtn->setObjectName("SecondaryBtn");
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddHistoryClicked);
    connect(remBtn, &QPushButton::clicked, this, &MainWindow::onRemoveHistoryClicked);
    btns->addWidget(addBtn); btns->addWidget(remBtn);
    lay->addLayout(btns);
    return w;
}

QWidget* MainWindow::createLabTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    tableLabs = new QTableWidget(0, 5);
    tableLabs->setHorizontalHeaderLabels({"Report", "Lab", "Date", "Notes", "File Path"});
    tableLabs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    editLabName = new QLineEdit(); editLabName->setPlaceholderText("Report Title");
    editLabPlace = new QLineEdit(); editLabPlace->setPlaceholderText("Lab Facility Name");
    editLabDate = new QLineEdit(); editLabDate->setPlaceholderText("Date");
    editLabFile = new QLineEdit(); editLabFile->setReadOnly(true); editLabFile->setPlaceholderText("No file attached");
    textLabNotes = new QTextEdit(); textLabNotes->setPlaceholderText("Observation notes...");
    textLabNotes->setMaximumHeight(80);

    lay->addWidget(tableLabs);
    lay->addWidget(editLabName);
    lay->addWidget(editLabPlace);
    lay->addWidget(editLabDate);
    lay->addWidget(textLabNotes);

    QHBoxLayout *fileLay = new QHBoxLayout();
    fileLay->addWidget(editLabFile);
    QPushButton *attBtn = new QPushButton("ATTACH FILE");
    connect(attBtn, &QPushButton::clicked, this, &MainWindow::onAttachFileClicked);
    fileLay->addWidget(attBtn);
    lay->addLayout(fileLay);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *addBtn = new QPushButton("SAVE LAB REPORT"); addBtn->setObjectName("ActionBtn");
    QPushButton *remBtn = new QPushButton("REMOVE RECORD"); remBtn->setObjectName("SecondaryBtn");
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddLabReportClicked);
    connect(remBtn, &QPushButton::clicked, this, &MainWindow::onRemoveLabReportClicked);
    btns->addWidget(addBtn); btns->addWidget(remBtn);
    lay->addLayout(btns);
    return w;
}

QWidget* MainWindow::createMedicineTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    tableMed = new QTableWidget(0, 5);
    tableMed->setHorizontalHeaderLabels({"Name", "Dosage", "Frequency", "Time", "Notes"});
    tableMed->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    editMedName = new QLineEdit(); editMedName->setPlaceholderText("Medicine Name");
    editMedDosage = new QLineEdit(); editMedDosage->setPlaceholderText("e.g. 500mg");
    comboMedFreq = new QComboBox(); comboMedFreq->addItems({"Once a day", "Twice a day", "Thrice a day", "Every 6 hours", "Every 8 hours", "As Needed"});
    timeMed = new QTimeEdit(QTime::currentTime());
    textMedNotes = new QTextEdit(); textMedNotes->setMaximumHeight(80);

    lay->addWidget(tableMed);
    lay->addWidget(new QLabel("NEW PRESCRIPTION"));
    lay->addWidget(editMedName);
    lay->addWidget(editMedDosage);
    lay->addWidget(comboMedFreq);
    lay->addWidget(timeMed);
    lay->addWidget(textMedNotes);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *addBtn = new QPushButton("ADD MEDICINE"); addBtn->setObjectName("ActionBtn");
    QPushButton *remBtn = new QPushButton("REMOVE MEDICINE"); remBtn->setObjectName("SecondaryBtn");
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onButtonMedAddClicked);
    connect(remBtn, &QPushButton::clicked, this, &MainWindow::onButtonMedRemoveClicked);
    btns->addWidget(addBtn); btns->addWidget(remBtn);
    lay->addLayout(btns);
    return w;
}

QWidget* MainWindow::createAITab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(40, 40, 40, 40);

    aiResponse = new QTextEdit(); aiResponse->setReadOnly(true);
    aiResponse->setPlaceholderText("HealthMate AI Response will appear here...");
    aiResponse->setStyleSheet("background: #0F172A; border: 1px solid #38BDF8; font-size: 11pt; color: #38BDF8;");

    userQuery = new QTextEdit(); userQuery->setMaximumHeight(100);
    userQuery->setPlaceholderText("Describe your symptoms or ask about medications...");

    lay->addWidget(new QLabel("AI CLINICAL ASSISTANT"));
    lay->addWidget(aiResponse);
    lay->addWidget(userQuery);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *askBtn = new QPushButton("GENERATE AI ANALYSIS"); askBtn->setObjectName("ActionBtn");
    QPushButton *clrBtn = new QPushButton("CLEAR CHAT"); clrBtn->setObjectName("SecondaryBtn");
    connect(askBtn, &QPushButton::clicked, this, &MainWindow::onAskAI);
    connect(clrBtn, &QPushButton::clicked, this, &MainWindow::onClearAI);
    btns->addWidget(askBtn); btns->addWidget(clrBtn);
    lay->addLayout(btns);
    return w;
}

QWidget* MainWindow::createSummaryTab() {
    QWidget *w = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setAlignment(Qt::AlignCenter);

    QLabel *icon = new QLabel("📄");
    icon->setStyleSheet("font-size: 80px;");
    icon->setAlignment(Qt::AlignCenter);

    QLabel *info = new QLabel("Hospital Summary Generator\n\nClick below to compile all your data into a professional PDF summary.");
    info->setAlignment(Qt::AlignCenter);
    info->setStyleSheet("font-size: 14pt; color: #94A3B8;");

    QPushButton *genBtn = new QPushButton("GENERATE PDF FOR DOCTOR");
    genBtn->setObjectName("ActionBtn");
    genBtn->setFixedWidth(300);
    connect(genBtn, &QPushButton::clicked, this, &MainWindow::onGenerateHospitalSummaryClicked);

    lay->addStretch();
    lay->addWidget(icon);
    lay->addWidget(info);
    lay->addSpacing(30);
    lay->addWidget(genBtn);
    lay->addStretch();
    return w;
}

// --- NAVIGATION & SLOTS ---

void MainWindow::onNavButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int index = btn->property("index").toInt();
    mainStack->setCurrentIndex(index);
}

void MainWindow::onSaveProfileClicked() {
    m_userProfile.setName(editName->text().toStdString());
    m_userProfile.setAge(editAge->text().toInt());
    m_userProfile.setGender(comboGender->currentText().toStdString());
    m_userProfile.setEmergencyContact(editEmergency->text().toStdString());
    m_userProfile.setAllergies(editAllergies->text().toStdString());
    m_userProfile.setChronicConditions(editChronic->text().toStdString());

    if (m_userProfile.saveToFile("profile.txt"))
        QMessageBox::information(this, "Success", "Profile data synchronized.");
}

void MainWindow::onAddHistoryClicked() {
    QString date = editHistDate->text();
    QString cond = editHistCond->text();
    QString notes = textHistNotes->toPlainText();

    if(date.isEmpty() || cond.isEmpty()) return;

    m_medicalHistory.push_back(MedicalHistoryEntry(date.toStdString(), cond.toStdString(), notes.toStdString()));
    refreshHistoryTable();
    saveHistoryToFile();
    editHistDate->clear(); editHistCond->clear(); textHistNotes->clear();
}

void MainWindow::onRemoveHistoryClicked() {
    int row = tableHistory->currentRow();
    if (row >= 0 && row < (int)m_medicalHistory.size()) {
        m_medicalHistory.erase(m_medicalHistory.begin() + row);
        refreshHistoryTable();
        saveHistoryToFile();
    }
}

void MainWindow::onAdd250() { m_hydration.addWater(250); updateHydrationUI(); }
void MainWindow::onAdd500() { m_hydration.addWater(500); updateHydrationUI(); }
void MainWindow::onResetHydration() { m_hydration.reset(); updateHydrationUI(); }
void MainWindow::onTargetChanged(int v) { m_hydration.setTarget(v); updateHydrationUI(); }

void MainWindow::updateHydrationUI() {
    consumedDisplay->display(m_hydration.getConsumed());
    hydrationBar->setValue(m_hydration.getPercentage());
    m_hydration.saveToFile("hydration.txt");
}

void MainWindow::onButtonMedAddClicked() {
    MedicineEntry med(editMedName->text(), editMedDosage->text(), comboMedFreq->currentText(), timeMed->time(), textMedNotes->toPlainText());
    if(med.getName().isEmpty()) return;
    m_medicineList.push_back(med);
    refreshMedicineTable();
    saveMedicineList();
    editMedName->clear(); editMedDosage->clear();
}

void MainWindow::onButtonMedRemoveClicked() {
    int row = tableMed->currentRow();
    if(row >= 0 && row < (int)m_medicineList.size()){
        m_medicineList.erase(m_medicineList.begin() + row);
        refreshMedicineTable();
        saveMedicineList();
    }
}

void MainWindow::onAddLabReportClicked() {
    LabReportEntry entry(editLabName->text().toStdString(), editLabPlace->text().toStdString(),
                         editLabDate->text().toStdString(), textLabNotes->toPlainText().toStdString(),
                         editLabFile->text().toStdString());
    if(entry.getReportName().empty()) return;
    m_labReports.push_back(entry);
    refreshLabReportsTable();
    saveLabReportsToFile();
    editLabName->clear(); editLabPlace->clear();
}

void MainWindow::onRemoveLabReportClicked() {
    int row = tableLabs->currentRow();
    if(row >= 0 && row < (int)m_labReports.size()){
        m_labReports.erase(m_labReports.begin() + row);
        refreshLabReportsTable();
        saveLabReportsToFile();
    }
}

void MainWindow::onAttachFileClicked() {
    QString fn = QFileDialog::getOpenFileName(this, "Select Report");
    if(!fn.isEmpty()) editLabFile->setText(fn);
}

void MainWindow::onAskAI() {
    QString q = userQuery->toPlainText();
    if(!q.isEmpty()) aiResponse->setPlainText(ai.answerQuestion(q));
}

void MainWindow::onClearAI() { userQuery->clear(); aiResponse->clear(); }

// --- PERSISTENCE ---

void MainWindow::refreshHistoryTable() {
    tableHistory->setRowCount(0);
    for(const auto &e : m_medicalHistory) {
        int r = tableHistory->rowCount(); tableHistory->insertRow(r);
        tableHistory->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(e.getDate())));
        tableHistory->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(e.getConditionName())));
        tableHistory->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(e.getNotes())));
    }
}

void MainWindow::refreshLabReportsTable() {
    tableLabs->setRowCount(0);
    for(const auto &r : m_labReports) {
        int row = tableLabs->rowCount(); tableLabs->insertRow(row);
        tableLabs->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(r.getReportName())));
        tableLabs->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(r.getLabName())));
        tableLabs->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(r.getDate())));
        tableLabs->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(r.getNotes())));
        tableLabs->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(r.getFilePath())));
    }
}

void MainWindow::refreshMedicineTable() {
    tableMed->setRowCount(0);
    for(const auto &m : m_medicineList) {
        int r = tableMed->rowCount(); tableMed->insertRow(r);
        tableMed->setItem(r, 0, new QTableWidgetItem(m.getName()));
        tableMed->setItem(r, 1, new QTableWidgetItem(m.getDosage()));
        tableMed->setItem(r, 2, new QTableWidgetItem(m.getFrequency()));
        tableMed->setItem(r, 3, new QTableWidgetItem(m.getTime().toString()));
        tableMed->setItem(r, 4, new QTableWidgetItem(m.getNotes()));
    }
}

void MainWindow::saveHistoryToFile() {
    std::ofstream out("history.txt");
    for (const auto &e : m_medicalHistory) out << e.getDate() << "\n" << e.getConditionName() << "\n" << e.getNotes() << "\n";
}

void MainWindow::saveMedicineList() {
    std::ofstream out("medicines.txt");
    for (const auto &m : m_medicineList) out << m.getName().toStdString() << "\n" << m.getDosage().toStdString() << "\n" << m.getFrequency().toStdString() << "\n" << m.getTime().toString().toStdString() << "\n" << m.getNotes().toStdString() << "\n";
}

void MainWindow::saveLabReportsToFile() {
    std::ofstream out("labreports.txt");
    for(const auto &r : m_labReports) out << r.getReportName() << "\n" << r.getLabName() << "\n" << r.getDate() << "\n" << r.getNotes() << "\n" << r.getFilePath() << "\n";
}

void MainWindow::loadAllData() {
    if(m_userProfile.loadFromFile("profile.txt")) {
        editName->setText(QString::fromStdString(m_userProfile.getName()));
        editAge->setText(QString::number(m_userProfile.getAge()));
        comboGender->setCurrentText(QString::fromStdString(m_userProfile.getGender()));
        editEmergency->setText(QString::fromStdString(m_userProfile.getEmergencyContact()));
        editAllergies->setText(QString::fromStdString(m_userProfile.getAllergies()));
        editChronic->setText(QString::fromStdString(m_userProfile.getChronicConditions()));
    }
    if (m_hydration.loadFromFile("hydration.txt")) {
        targetSpin->setValue(m_hydration.getTarget());
        updateHydrationUI();
    }

    // Load History
    std::ifstream inH("history.txt");
    std::string d, c, n;
    while(std::getline(inH, d) && std::getline(inH, c) && std::getline(inH, n))
        m_medicalHistory.push_back(MedicalHistoryEntry(d, c, n));
    refreshHistoryTable();

    // Load Labs
    std::ifstream inL("labreports.txt");
    std::string rn, ln, ld, lno, fp;
    while(std::getline(inL, rn) && std::getline(inL, ln) && std::getline(inL, ld) && std::getline(inL, lno) && std::getline(inL, fp))
        m_labReports.push_back(LabReportEntry(rn, ln, ld, lno, fp));
    refreshLabReportsTable();

    // Load Meds
    std::ifstream inM("medicines.txt");
    std::string mn, md, mf, mt, mno;
    while(std::getline(inM, mn) && std::getline(inM, md) && std::getline(inM, mf) && std::getline(inM, mt) && std::getline(inM, mno))
        m_medicineList.push_back(MedicineEntry(QString::fromStdString(mn), QString::fromStdString(md), QString::fromStdString(mf), QTime::fromString(QString::fromStdString(mt)), QString::fromStdString(mno)));
    refreshMedicineTable();
}

void MainWindow::onGenerateHospitalSummaryClicked() {
    QString file = QFileDialog::getSaveFileName(this, "Save Summary", "Patient_Summary.pdf", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter pdf(file);
    pdf.setPageSize(QPageSize::A4);
    QPainter painter(&pdf);
    int y = 100;

    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(2000, y, "MEDICAL SUMMARY REPORT"); y += 500;

    painter.setFont(QFont("Arial", 12));
    painter.drawText(500, y, "Patient: " + QString::fromStdString(m_userProfile.getName())); y += 200;
    painter.drawText(500, y, "Age: " + QString::number(m_userProfile.getAge())); y += 200;
    painter.drawText(500, y, "Chronic Conditions: " + QString::fromStdString(m_userProfile.getChronicConditions())); y += 400;

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(500, y, "Active Medications:"); y += 250;
    painter.setFont(QFont("Arial", 11));
    for(const auto &m : m_medicineList) {
        painter.drawText(700, y, m.getName() + " - " + m.getDosage() + " (" + m.getFrequency() + ")");
        y += 200;
    }

    painter.end();
    QMessageBox::information(this, "PDF Export", "Summary has been generated successfully.");
}
