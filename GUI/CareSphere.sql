DROP DATABASE IF EXISTS CareSphereDB;
CREATE DATABASE CareSphereDB;
USE CareSphereDB;
CREATE TABLE users (
    id VARCHAR(100) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100)  NOT NULL UNIQUE,
    phoneNo VARCHAR(20)   NOT NULL UNIQUE,
    bloodGroup VARCHAR(10) NOT NULL,
    gender VARCHAR(10) DEFAULT 'Male',
    DOB VARCHAR(100) DEFAULT ''
);

CREATE TABLE prescriptions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(100) NOT NULL,
    doctor_name VARCHAR(100) NOT NULL,
    MEDICINES TEXT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);


CREATE TABLE NOTES(
	user_id VARCHAR(100) NOT NULL,
    notes VARCHAR(500)  NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);


CREATE TABLE reminders (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(100) NOT NULL,
    title VARCHAR(255) NOT NULL,
    reminder_date VARCHAR(100) NOT NULL,
    reminder_time VARCHAR(100) NULL, -- Optional time
    note TEXT NULL, -- Optional note
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE appointments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(100) NOT NULL,
    doctor_name VARCHAR(255) NOT NULL,
    location VARCHAR(255) NOT NULL,
    appointment_date VARCHAR(255) NOT NULL,
    appointment_time VARCHAR(255) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);




