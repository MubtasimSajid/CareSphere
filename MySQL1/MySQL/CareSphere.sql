DROP DATABASE IF EXISTS CareSphere;
CREATE DATABASE CareSphere;
USE CareSphere;

DROP TABLE IF EXISTS Notifications;
DROP TABLE IF EXISTS EmergencyRequests;
DROP TABLE IF EXISTS Payments;
DROP TABLE IF EXISTS Bills;
DROP TABLE IF EXISTS MedicalRecords;
DROP TABLE IF EXISTS Appointments;
DROP TABLE IF EXISTS Medications;
DROP TABLE IF EXISTS HospitalSurgeries;
DROP TABLE IF EXISTS Surgeries;
DROP TABLE IF EXISTS Tests;
DROP TABLE IF EXISTS HospitalDoctors;
DROP TABLE IF EXISTS HospitalDepartments;
DROP TABLE IF EXISTS HospitalAmbulances;
DROP TABLE IF EXISTS Doctors;
DROP TABLE IF EXISTS Patients;
DROP TABLE IF EXISTS Hospitals;
DROP TABLE IF EXISTS Departments;
DROP TABLE IF EXISTS Users;
DROP TABLE IF EXISTS PatientAllergies;
DROP TABLE IF EXISTS PatientChronicConditions;
DROP TABLE IF EXISTS DoctorDegrees;
DROP TABLE IF EXISTS DoctorSurgeryFees;
DROP TABLE IF EXISTS ScheduledSurgeries;
DROP TABLE IF EXISTS HospitalSurgeryFees;
DROP TABLE IF EXISTS Medicines;

CREATE TABLE Departments (
    department_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL
);

INSERT INTO Departments (name) VALUES
('Medicine'),
('Surgery'),
('Obstetrics and Gynecology'),
('Pediatrics'),
('Cardiology'),
('Orthopedics'),
('Neurology'),
('Psychiatry'),
('Oncology'),
('Radiology');



CREATE TABLE Surgeries (
    surgery_id INT PRIMARY KEY AUTO_INCREMENT,
    department_id INT,
    name VARCHAR(100) NOT NULL,
    FOREIGN KEY (department_id) REFERENCES Departments(department_id)
);

INSERT INTO Surgeries (department_id, name) VALUES
(1, 'Endoscopic Biopsy'),
(1, 'Lumbar Puncture'),
(2, 'Appendectomy'),
(2, 'Cholecystectomy'),
(3, 'Cesarean Section'),
(3, 'Hysterectomy'),
(4, 'Pediatric Hernia Repair'),
(4, 'Congenital Heart Defect Surgery'),
(5, 'Coronary Artery Bypass Grafting'),
(5, 'Angioplasty with Stent Placement'),
(6, 'Total Knee Replacement'),
(6, 'Spinal Fusion'),
(7, 'Craniotomy'),
(7, 'Laminectomy'),
(8, 'Deep Brain Stimulation'),
(8, 'Electroconvulsive Therapy'),
(9, 'Mastectomy'),
(9, 'Tumor Resection'),
(10, 'Radiofrequency Ablation'),
(10, 'Embolization Therapy');


CREATE TABLE Users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    phone VARCHAR(11) UNIQUE NOT NULL,
    date_of_birth DATE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    role ENUM('patient', 'doctor', 'hospital') NOT NULL
);

CREATE TABLE Hospitals (
    hospital_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT UNIQUE,
    hospital_name VARCHAR(100) NOT NULL,
    address TEXT NOT NULL,
    income DECIMAL(10,2) DEFAULT 0.00,
    expenditure DECIMAL(10,2) DEFAULT 0.00,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

CREATE TABLE Patients (
    patient_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT UNIQUE,
    sex ENUM('male', 'female', 'transgender') NOT NULL,
    blood_group ENUM('A+', 'A-', 'B+', 'B-', 'AB+', 'AB-', 'O+', 'O-') NOT NULL,
    weight DECIMAL(5,2),
    height DECIMAL(5,2),
    blood_pressure VARCHAR(20),
    blood_sugar DECIMAL(5,2),
    bills_remaining DECIMAL(10,2) DEFAULT 0.00,
    bills_paid DECIMAL(10,2) DEFAULT 0.00,
    has_allergies BOOLEAN DEFAULT FALSE,
    has_chronic_conditions BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

CREATE TABLE PatientAllergies (
    allergy_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT,
    allergy_name VARCHAR(100) NOT NULL,
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)
);

CREATE TABLE PatientChronicConditions (
    condition_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT,
    condition_name VARCHAR(100) NOT NULL,
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)
);

CREATE TABLE MedicalRecords (
    record_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT,
    doctor_id INT,
    record_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    current_height DECIMAL(5,2),
    current_weight DECIMAL(5,2),
    current_blood_pressure VARCHAR(20),
    current_blood_sugar DECIMAL(5,2),
    diagnosis TEXT,
    treatment TEXT,
    medicines TEXT,
    remarks TEXT,
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)
);

CREATE TABLE DoctorDegrees (
    degree_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT,
    degree_name VARCHAR(100) NOT NULL,
    institute VARCHAR(100) NOT NULL,
    FOREIGN KEY (doctor_id) REFERENCES Users(user_id)
);

CREATE TABLE Doctors (
    doctor_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT UNIQUE,
    department_id INT,
    specialization VARCHAR(100) NOT NULL,
    appointment_fees DECIMAL(10,2) NOT NULL,
    income DECIMAL(10,2) DEFAULT 0.00,
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    FOREIGN KEY (department_id) REFERENCES Departments(department_id)
);

CREATE TABLE DoctorSurgeryFees (
    fee_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT,
    surgery_id INT,
    fee DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id),
    FOREIGN KEY (surgery_id) REFERENCES Surgeries(surgery_id)
);

CREATE TABLE Appointments (
    appointment_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT,
    patient_id INT,
    appointment_date DATE,
    appointment_time TIME,
    status ENUM('scheduled', 'completed', 'cancelled') DEFAULT 'scheduled',
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id),
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id),
    CONSTRAINT unique_doctor_appointment UNIQUE (doctor_id, appointment_date, appointment_time)
);

CREATE TABLE ScheduledSurgeries (
    schedule_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT,
    patient_id INT,
    surgery_id INT,
    surgery_date DATE,
    surgery_time TIME,
    status ENUM('scheduled', 'completed', 'cancelled') DEFAULT 'scheduled',
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id),
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id),
    FOREIGN KEY (surgery_id) REFERENCES Surgeries(surgery_id),
    CONSTRAINT unique_doctor_surgery UNIQUE (doctor_id, surgery_date, surgery_time)
);


CREATE TABLE Bills (
    bill_id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT,
    hospital_id INT,
    amount DECIMAL(10,2) NOT NULL,
    description TEXT,
    issue_date DATE NOT NULL,
    due_date DATE NOT NULL,
    status ENUM('Pending', 'Paid', 'Overdue') DEFAULT 'Pending',
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id) ON DELETE CASCADE,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id) ON DELETE CASCADE
);

CREATE TABLE Payments (
    payment_id INT AUTO_INCREMENT PRIMARY KEY,
    bill_id INT,
    amount DECIMAL(10,2) NOT NULL,
    payment_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    payment_method ENUM('Cash', 'Credit Card', 'Debit Card', 'Online Banking') NOT NULL,
    FOREIGN KEY (bill_id) REFERENCES Bills(bill_id) ON DELETE CASCADE
);

CREATE TABLE Notifications (
    notification_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    message TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    is_read BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (user_id) REFERENCES Users(user_id) ON DELETE CASCADE
);

CREATE TABLE Reports (
    report_id INT AUTO_INCREMENT PRIMARY KEY,
    report_type ENUM('Billing', 'Patient Statistics', 'Hospital Performance') NOT NULL,
    generated_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    data TEXT NOT NULL
);

CREATE TABLE Medications (
    medication_id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT,
    doctor_id INT,
    hospital_id INT,
    medication_name VARCHAR(100) NOT NULL,
    dosage VARCHAR(50) NOT NULL,
    frequency VARCHAR(50) NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE,
    status ENUM('Active', 'Completed', 'Discontinued') DEFAULT 'Active',
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id) ON DELETE CASCADE,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id) ON DELETE CASCADE,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id) ON DELETE CASCADE
);

CREATE TABLE HospitalDepartments (
    hospital_dept_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT,
    department_id INT,
    appointment_fee DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id),
    FOREIGN KEY (department_id) REFERENCES Departments(department_id)
);

CREATE TABLE HospitalSurgeryFees (
    fee_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT,
    surgery_id INT,
    fee DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id),
    FOREIGN KEY (surgery_id) REFERENCES Surgeries(surgery_id)
);

CREATE TABLE HospitalAmbulances (
    ambulance_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT,
    vehicle_number VARCHAR(20) UNIQUE NOT NULL,
    phone_number VARCHAR(11) NOT NULL,
    fee_per_km DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id)
);

CREATE TABLE Medicines (
    medicine_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id)
);

CREATE TABLE Tests (
    test_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id)
);








