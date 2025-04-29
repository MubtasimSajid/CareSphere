# What is CareSphere?
CareSphere is a healthcare management system which aims to provide a platform for the different stakeholders of the sector (patients, doctors & hospitals/clinics alike) a carefully curated featureset to enable them to handle their operations more efficiently & communicate with others more effectively. CareSphere simplifies medicare management.
We have 2 different editions of the app - a GUI version, another CLI. Both of them offer a separate set of features & was designed for the purposes of a different demographic.

Below are explainers for both of them & the ways to set them up -

# CareSphere GUI
The GUI edition of CareSphere aims to be a one-stop personal hub for the patient to collect & organize all their medical necesseties including prescriptions, reminders, appointments & notes. It intuitively visualizes the healthcare-related materials in one place  & helps you elegantly order your medical priorities.

The GUI app was created using C++ blended with Qt. The files needed for this app to be run locally are stored in the GUI directory. For some reason, if the steps described below to test the app out yourself doesn't work, please try renaming the "GUI" folder to "CareSphere".
So, without further ado, let's begin with the prerequisites.

1. Qt - we used the version 6.x.
2. Qt Creator IDE - not absolutely necessary, but will make your life easier to run experiments with the codes or the UI/UX elements & to build it.
3. C++ compiler - we only ran it on Windows, so we used MinGW.
4. CMake
5. MySQL Workbench - again, not necessary, but helps with the database's setup

Now that you've installed all the prerequisite softwares, you must clone the project or download & extract the ZIP. After that, we'll build the project. If you have the Qt Creator IDE installed, it's automatically configured to build for Windows. For other OSes, further modifications may be necessary. Without using the Qt Creator IDE, you can open the CMakeLists.txt in the command-line, & build it manually using the following terminal commands:
```
mkdir build
cd build
cmake ..
cmake --build .
```

Since it also uses MySQL to maintain the database in the background, you'll need to copy the libmysql.dll file into the build directory. It's already done in the repository, but you may want to double-check in case it doesn't run properly. To properly set up & connect to the database for the codes to work, here's a step-by-step guide:

1. Open MySQL_utilities.h & move to the function MySQLinit()
2. Add the username & password used for your local MySQL server
3. Execute the SQL codes written inside CareSphere.sql file

To test all the tables are working, you may want to run the following SQL commands & check the states of the tables:
```sql
SELECT * FROM users;
SELECT * FROM prescriptions;
SELECT * FROM NOTES;
SELECT * FROM reminders;
SELECT * FROM appointments;
```
If they are working correctly, you're good to go. Otherwise, there may have been an error in executing the .sql file.

# CareSphere CLI
The CLI app is way more verbose in terms of the rich feature set it offers. It has different modes for different types of users: doctors, hospitals & patients. This is the app for hospitals/clinics to hold all their management-related information to properly deliver healthcare services. Doctors can log in here, link themselves to a medicare instituition & manage their availability, patient list & other professional details seamlessly. Similarly, a patient can book an appointment with a doctor, store their previous medical record & see their medical details at a glance. This is the one app for the different players in the health management sector to not only properly handle themselves, but also to easily collaborate with other parties.

The technologies used here are listed below -
1. C++ (using Visual Studio IDE & MinGW compiler on Windows)
2. MySQL Database
3. MySQL Connector for C++

To run this app locally, first clone the "CLI" folder (or download & extract the ZIP), & then open the folder in Visual Studio (it can, obviously, be run on other IDEs as well (may need more tinkering in some cases), but we'll stick to Visual Studio as that's where it was written). Configure the MySQL Connector from the project settings to match with your device's setup. The database connection details (such as host, username & password) may need to be updated inside the code. After all that's done, the project is ready to be built & run.
The project is structured into 3 sub-folders.
/src - main source files, houses all the .cpp codes
/include - the header files reside here

# License
Please note that CareSphere was initially made to be submitted as a project submission for IUT CSE-22's OOP-4302 lab course. It was made by [Mubtasim Sajid Ahmed](https://github.com/MubtasimSajid), [Mahmudul Hasan Sakib](https://github.com/beiged231), [Farhan Tahsin Khan](https://github.com/Farhan41229) & [AKM Ishmam Tahmid](https://github.com/ishmam259). Now we're releasing this fully open & free. Hope you have fun tinkering with it. If you develop further upon it & change the app substansially, you're allowed to place your own preferred license over it.
