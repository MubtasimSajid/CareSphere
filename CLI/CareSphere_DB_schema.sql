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
DROP TABLE IF EXISTS DoctorAppointments;
DROP TABLE IF EXISTS PatientNotes;
DROP TABLE IF EXISTS DoctorNotes;
DROP TABLE IF EXISTS DoctorTransactions;
DROP TABLE IF EXISTS DoctorReviews;
DROP TABLE IF EXISTS HospitalReviews;
DROP TABLE IF EXISTS DoctorHospitalAffiliations;

CREATE TABLE Departments (
    department_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    description TEXT
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
    description TEXT,
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
    dob DATE NOT NULL,
    role ENUM('patient', 'doctor', 'hospital') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Hospitals (
    hospital_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    hospital_name VARCHAR(100) NOT NULL,
    address TEXT NOT NULL,
    city VARCHAR(50) NOT NULL,
    state VARCHAR(50) NOT NULL,
    country VARCHAR(50) NOT NULL,
    pincode VARCHAR(10) NOT NULL,
    has_ambulance BOOLEAN DEFAULT FALSE,
    income DECIMAL(10,2) DEFAULT 0.00,
    expenditure DECIMAL(10,2) DEFAULT 0.00,
    average_rating DECIMAL(3,2) DEFAULT 0.00,
    total_ratings INT DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

CREATE TABLE Patients (
    patient_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    sex ENUM('male', 'female', 'transgender') NOT NULL,
    blood_group ENUM('A+', 'A-', 'B+', 'B-', 'AB+', 'AB-', 'O+', 'O-') NOT NULL,
    weight DECIMAL(5,2),
    height DECIMAL(5,2),
    blood_pressure VARCHAR(10),
    blood_sugar DECIMAL(5,2),
    bills_remaining DECIMAL(10,2) DEFAULT 0.00,
    bills_paid DECIMAL(10,2) DEFAULT 0.00,
    has_allergies BOOLEAN DEFAULT FALSE,
    has_chronic_conditions BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

CREATE TABLE PatientAllergies (
    allergy_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
    allergy_name VARCHAR(100) NOT NULL,
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)
);

CREATE TABLE PatientChronicConditions (
    condition_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_id INT NOT NULL,
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

CREATE TABLE Doctors (
    doctor_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    department_id INT NOT NULL,
    specialization VARCHAR(100) NOT NULL,
    appointment_fees DECIMAL(10,2) NOT NULL,
    license_number VARCHAR(50) NOT NULL,
    income DECIMAL(10,2) DEFAULT 0.00,
    average_rating DECIMAL(3,2) DEFAULT 0.00,
    total_ratings INT DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    FOREIGN KEY (department_id) REFERENCES Departments(department_id)
);

CREATE TABLE DoctorDegrees (
    degree_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT NOT NULL,
    degree_name VARCHAR(100) NOT NULL,
    institute VARCHAR(100) NOT NULL,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id)
);

CREATE TABLE DoctorSurgeryFees (
    fee_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT NOT NULL,
    surgery_id INT NOT NULL,
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

CREATE TABLE HospitalDoctors (
    hospital_doctor_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT NOT NULL,
    doctor_id INT NOT NULL,
    department_id INT NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id),
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id),
    FOREIGN KEY (department_id) REFERENCES Departments(department_id)
);

CREATE TABLE HospitalDepartments (
    hospital_dept_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT NOT NULL,
    department_id INT NOT NULL,
    department_name VARCHAR(100) NOT NULL,
    department_description TEXT,
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
    hospital_id INT NOT NULL,
    vehicle_number VARCHAR(20) NOT NULL,
    phone_number VARCHAR(15) NOT NULL,
    fee_per_km DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id)
);

CREATE TABLE Medicines (
    medicine_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT NOT NULL,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id)
);

CREATE TABLE Tests (
    test_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT NOT NULL,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id)
);

CREATE TABLE DoctorAppointments 
(
    appointment_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT NOT NULL,
    doctor_id INT NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id),
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id)
);

CREATE TABLE PatientNotes 
(
    note_id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT NOT NULL,
    note TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id) ON DELETE CASCADE
);

CREATE TABLE DoctorNotes 
(
    note_id INT AUTO_INCREMENT PRIMARY KEY,
    doctor_id INT NOT NULL,
    note TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id) ON DELETE CASCADE
);

CREATE TABLE DoctorTransactions (
    transaction_id INT AUTO_INCREMENT PRIMARY KEY,
    doctor_id INT NOT NULL,
    amount DECIMAL(10,2) NOT NULL,
    type ENUM('income', 'withdrawal') NOT NULL,
    description TEXT,
    transaction_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id) ON DELETE CASCADE
);

CREATE TABLE DoctorReviews 
(
    review_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT NOT NULL,
    patient_id INT NOT NULL,
    rating DECIMAL(3,2) NOT NULL,
    review_text TEXT,
    review_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id),
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)
);

CREATE TABLE HospitalReviews 
(
    review_id INT PRIMARY KEY AUTO_INCREMENT,
    hospital_id INT NOT NULL,
    patient_id INT NOT NULL,
    rating DECIMAL(3,2) NOT NULL,
    review_text TEXT,
    review_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id),
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)
);

CREATE TABLE DoctorHospitalAffiliations 
(
    affiliation_id INT PRIMARY KEY AUTO_INCREMENT,
    doctor_id INT NOT NULL,
    hospital_id INT NOT NULL,
    FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id),
    FOREIGN KEY (hospital_id) REFERENCES Hospitals(hospital_id),
    UNIQUE KEY unique_doctor_hospital (doctor_id, hospital_id)
);

-- Add rating columns to Doctors table
ALTER TABLE Doctors
ADD COLUMN average_rating DECIMAL(3,2) DEFAULT NULL,
ADD COLUMN total_ratings INT DEFAULT 0;

-- Add rating columns to Hospitals table
ALTER TABLE Hospitals
ADD COLUMN average_rating DECIMAL(3,2) DEFAULT NULL,
ADD COLUMN total_ratings INT DEFAULT 0;

-- Insert sample doctors
INSERT INTO Users (name, email, password, phone, dob, role) VALUES
('John Smith', 'john.smith@example.com', 'hashed_password1', '12345678901', '1980-01-01', 'doctor'),
('Sarah Johnson', 'sarah.johnson@example.com', 'hashed_password2', '12345678902', '1982-03-15', 'doctor'),
('Michael Brown', 'michael.brown@example.com', 'hashed_password3', '12345678903', '1978-07-22', 'doctor'),
('Emily Davis', 'emily.davis@example.com', 'hashed_password4', '12345678904', '1985-11-30', 'doctor'),
('Robert Wilson', 'robert.wilson@example.com', 'hashed_password5', '12345678905', '1975-04-18', 'doctor'),
('Lisa Anderson', 'lisa.anderson@example.com', 'hashed_password6', '12345678906', '1983-09-25', 'doctor'),
('David Taylor', 'david.taylor@example.com', 'hashed_password7', '12345678907', '1979-12-08', 'doctor'),
('Jennifer White', 'jennifer.white@example.com', 'hashed_password8', '12345678908', '1981-06-14', 'doctor'),
('Thomas Lee', 'thomas.lee@example.com', 'hashed_password9', '12345678909', '1977-02-28', 'doctor');

INSERT INTO Doctors (user_id, department_id, specialization, appointment_fees, license_number) VALUES
(1, 1, 'General Medicine', 500.00, 'MD12345'),
(2, 2, 'General Surgery', 800.00, 'MD23456'),
(3, 3, 'Obstetrics', 600.00, 'MD34567'),
(4, 4, 'Pediatrics', 400.00, 'MD45678'),
(5, 5, 'Cardiology', 1000.00, 'MD56789'),
(6, 6, 'Orthopedics', 900.00, 'MD67890'),
(7, 7, 'Neurology', 700.00, 'MD78901'),
(8, 8, 'Psychiatry', 500.00, 'MD89012'),
(9, 9, 'Oncology', 1200.00, 'MD90123');

-- Insert sample hospitals
INSERT INTO Users (name, email, password, phone, dob, role) VALUES
('City General Hospital', 'city.general@example.com', 'hashed_password10', '12345678910', '1990-01-01', 'hospital'),
('Medical Center', 'medical.center@example.com', 'hashed_password11', '12345678911', '1992-03-15', 'hospital'),
('Community Hospital', 'community.hospital@example.com', 'hashed_password', '12345678912', '1995-07-22', 'hospital');

INSERT INTO Hospitals (user_id, hospital_name, address, city, state, country, pincode) VALUES
(10, 'City General Hospital', '123 Main St', 'New York', 'NY', 'USA', '10001'),
(11, 'Medical Center', '456 Health Ave', 'Los Angeles', 'CA', 'USA', '90001'),
(12, 'Community Hospital', '789 Care Blvd', 'Chicago', 'IL', 'USA', '60601');

-- Insert sample doctor-hospital affiliations
INSERT INTO DoctorHospitalAffiliations (doctor_id, hospital_id) VALUES
(1, 1),  -- Dr. John Smith at City General Hospital
(1, 2),  -- Dr. John Smith also at Medical Center
(2, 1),  -- Dr. Sarah Johnson at City General Hospital
(3, 2),  -- Dr. Michael Brown at Medical Center
(4, 3),  -- Dr. Emily Davis at Community Hospital
(5, 1),  -- Dr. Robert Wilson at City General Hospital
(6, 2),  -- Dr. Lisa Anderson at Medical Center
(7, 3),  -- Dr. David Taylor at Community Hospital
(8, 1),  -- Dr. Jennifer White at City General Hospital
(9, 2);  -- Dr. Thomas Lee at Medical Center








