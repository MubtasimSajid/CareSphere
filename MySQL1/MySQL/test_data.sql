-- Insert test users
INSERT INTO Users (name, email, phone, password, role) VALUES
('John Doe', 'john@example.com', '1234567890', 'password123', 'Patient'),
('Jane Smith', 'jane@example.com', '0987654321', 'password123', 'Patient'),
('Dr. Sarah Wilson', 'sarah@example.com', '5555555555', 'password123', 'Doctor'),
('Dr. Michael Brown', 'michael@example.com', '6666666666', 'password123', 'Doctor'),
('City Hospital', 'city@hospital.com', '7777777777', 'password123', 'Hospital'),
('General Hospital', 'general@hospital.com', '8888888888', 'password123', 'Hospital');

-- Insert hospitals
INSERT INTO Hospitals (user_id, hospital_name, location) VALUES
(5, 'City Hospital', '123 Main St'),
(6, 'General Hospital', '456 Oak Ave');

-- Insert patients
INSERT INTO Patients (user_id, date_of_birth, blood_group, medical_history) VALUES
(1, '1990-01-01', 'A+', 'No major issues'),
(2, '1985-05-15', 'B-', 'Allergic to penicillin');

-- Insert doctors
INSERT INTO Doctors (user_id, specialization, hospital_id) VALUES
(3, 'Cardiology', 1),
(4, 'Pediatrics', 2);

-- Insert appointments
INSERT INTO Appointments (patient_id, doctor_id, hospital_id, department, appointment_date, status) VALUES
(1, 3, 1, 'Cardiology', '2024-03-20 10:00:00', 'Scheduled'),
(1, 4, 2, 'Pediatrics', '2024-03-21 14:30:00', 'Scheduled'),
(2, 3, 1, 'Cardiology', '2024-03-22 09:00:00', 'Scheduled');

-- Insert medical records
INSERT INTO MedicalRecords (patient_id, doctor_id, hospital_id, diagnosis, treatment, prescription) VALUES
(1, 3, 1, 'Regular checkup', 'No treatment needed', 'Continue healthy lifestyle'),
(2, 4, 2, 'Common cold', 'Rest and fluids', 'Paracetamol 500mg');

-- Insert bills
INSERT INTO Bills (patient_id, doctor_id, hospital_id, total_amount, status, issue_date, due_date) VALUES
(1, 3, 1, 150.00, 'Pending', '2024-03-19', '2024-04-19'),
(1, 4, 2, 200.00, 'Pending', '2024-03-19', '2024-04-19'),
(2, 3, 1, 175.00, 'Pending', '2024-03-19', '2024-04-19');

-- Insert notifications
INSERT INTO Notifications (user_id, message, is_read) VALUES
(1, 'Your appointment is scheduled for tomorrow', false),
(3, 'New appointment scheduled for tomorrow', false),
(2, 'Your bill is pending payment', false);





-- Insert Doctors into Users table 
INSERT INTO Users (name, email, phone, password, role) VALUES
('Dr. Sarah Wilson', 'sarah.wilson@hospital.com', '1234567890', 'password123', 'Doctor'),
('Dr. Michael Brown', 'michael.brown@hospital.com', '2345678901', 'password123', 'Doctor'),
('Dr. Emily Davis', 'emily.davis@hospital.com', '3456789012', 'password123', 'Doctor'),
('Dr. James Wilson', 'james.wilson@hospital.com', '4567890123', 'password123', 'Doctor'),
('Dr. Lisa Anderson', 'lisa.anderson@hospital.com', '5678901234', 'password123', 'Doctor'),
('Dr. Robert Taylor', 'robert.taylor@hospital.com', '6789012345', 'password123', 'Doctor'),
('Dr. Jennifer Lee', 'jennifer.lee@hospital.com', '7890123456', 'password123', 'Doctor'),
('Dr. David Miller', 'david.miller@hospital.com', '8901234567', 'password123', 'Doctor'),
('Dr. Maria Garcia', 'maria.garcia@hospital.com', '9012345678', 'password123', 'Doctor'),
('Dr. Thomas Johnson', 'thomas.johnson@hospital.com', '0123456789', 'password123', 'Doctor');

-- Insert Doctors into Doctors table
INSERT INTO Doctors (user_id, specialization) VALUES
((SELECT user_id FROM Users WHERE email = 'sarah.wilson@hospital.com'), 'Cardiology'),
((SELECT user_id FROM Users WHERE email = 'michael.brown@hospital.com'), 'Orthopedics'),
((SELECT user_id FROM Users WHERE email = 'emily.davis@hospital.com'), 'Pediatrics'),
((SELECT user_id FROM Users WHERE email = 'james.wilson@hospital.com'), 'Neurology'),
((SELECT user_id FROM Users WHERE email = 'lisa.anderson@hospital.com'), 'Oncology'),
((SELECT user_id FROM Users WHERE email = 'robert.taylor@hospital.com'), 'Surgery'),
((SELECT user_id FROM Users WHERE email = 'jennifer.lee@hospital.com'), 'Psychiatry'),
((SELECT user_id FROM Users WHERE email = 'david.miller@hospital.com'), 'Radiology'),
((SELECT user_id FROM Users WHERE email = 'maria.garcia@hospital.com'), 'Obstetrics and Gynecology'),
((SELECT user_id FROM Users WHERE email = 'thomas.johnson@hospital.com'), 'Medicine'); 