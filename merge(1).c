#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "custom_functions.h"

#define MAX_CREDENTIALS 100 // Maximum number of credentials

////////////////////////
// Structs//
////////////////////////

struct login{
    char patientID [10];
    char password [10];
    char Name[50];
};

struct date{
    int day;
    int month;
    int year;
};

struct time{
    int hours;
    int minutes;
};

struct ehr{
    char patientID[10];
    char patientName[50];
    struct date dob;
    char medicalHistory[500];
    char allergies[200];
    char medications[200];
    char pastProcedures[300];
    char diagnostics[300];
};

struct doctorSchedule{
    char doctorID[10];
    struct date apdate;
    struct time starttime;
    struct time endtime;
};

struct appointment{
    char appointmentID[20];
    char patientID[10];
    char patientName[50];
    char doctorID[10];
    char doctorName[50];
    char reason[50];
    char status[10];
    struct date apdate;
    struct time starttime;
    struct time endtime;
    char cancel[50];
	int cancel_ref;
};

struct treatments{
    char patientID[7];
    char patientName[50];
    struct date treatmentdate;
    char condition[100];
    char treatment[100];
    char outcome[100];
};

// Define a structure to hold username and password for any role
typedef struct {
    char username[50];
    char password[50];
    char role[20];
} Credentials;

// Define the structure for Appointment
typedef struct {
    char Date[11];
    char Time[6];
    char PatientName[50];
    char Doctor[50];
    char AppointmentType[50];
    char Status[20];
} Appointment;

struct doctor{
    char doctorID[10];
    char doctorName[50];
};

struct insurance_claim {
    char claimID[10];
    char patientID[10];
    char appointmentID[10];
    char billingID[10];
    char insuranceCompany[50];
    char insuranceNumber[20];
    char claimStatus[20];
    double claimAmount;
    struct date claimDate;
    struct date approvalDate;
    struct time claimTime;
    struct time approvalTime;
};

struct billing {
    char billingID[10];
    char patientID[10];
    char appointmentID[20];
    double total;
    double insuranceCoveredAmount;
    double patientResponsibleAmount;
    double insurancepaid;
    char paymentmethod[11];
    double amountgiven;
    double change;
    double tax;
    char insuranceProvider[50];
    char insuranceID[20];
    char claimID[20];
    char status[8]; //14
    struct date insurancebillingdate;   //17
    struct time insurancebillingtime;   //19
    struct date insurancepaymentdate;   //22
    struct time insurancepaymenttime;   //24
    struct date billingdate;            //27
    struct time billingtime;            //29
    struct date paymentdate;            //32
    struct time paymenttime;            //34
};

struct inv {
    char itemName [50];
    int itemQuantity;
};

//////////////////////////
///Doctor Functions
///Ahmed Jinan Ali
///TP075798
/////////////////////////

//Validation functions

// Function to check if the input time is valid
int isValidTime(struct time t) {
    // Basic validation example (adjust as per your requirements)
    if (t.hours < 0 || t.hours > 23)
        return 0;
    if (t.minutes < 0 || t.minutes > 59)
        return 0;
    return 1;
}

// Function to get the current date and time
void get_current_datetime(struct date* current_date, struct time* current_time) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    current_date->day = tm.tm_mday;
    current_date->month = tm.tm_mon + 1;
    current_date->year = tm.tm_year + 1900;

    current_time->hours = tm.tm_hour;
    current_time->minutes = tm.tm_min;
}

// Function to check if the input time is valid
int is_valid_time(struct time t) {
    return (t.hours >= 9 && t.hours < 21);
}

bool datetimeChecker(struct date proposed_date, struct time proposed_time, struct date target_date, struct time target_time){

    printf(" proposed_date: \t%d-%d-%d\n", proposed_date.year, proposed_date.month, proposed_date.day);
    printf(" proposed_time: \t%d:%d\n", proposed_time.hours, proposed_time.minutes);
    printf(" target_date: \t%d-%d-%d\n", target_date.year, target_date.month, target_date.day);
    printf(" target_time: \t%d:%d\n", target_time.hours, target_time.minutes);

    if (proposed_date.day == target_date.day && proposed_date.month == target_date.month && proposed_date.year == target_date.year){
        printf("Date Not Same\n");
        if (proposed_time.hours = target_time.hours && proposed_time.minutes == target_time.minutes){
            printf("Return True\n");
            return true;
        }
    }
    printf("Return False\n");
    return false;
};

int compareDates(struct date date1, struct date date2) {
    if (date1.year != date2.year)
        return date1.year - date2.year;
    if (date1.month != date2.month)
        return date1.month - date2.month;
    return date1.day - date2.day;
}

////////////////////////
// Initializations//
////////////////////////

int Initialise_EHRFile() {
    bool viewingEHRflag = false; //Flag to measure if file exists or not
    int EHRcounter = 0; //We will need this in later operations to define how long our arrays should be

    FILE* file_handler_EHRcheck;
    file_handler_EHRcheck = fopen("ehr.txt", "r"); // Opens file
    if (file_handler_EHRcheck == NULL) { //If the file comes back as null, means file does not exist
        viewingEHRflag = true; //File does not exist hence we will need to make a file
    } else {
        // Read the file to count the number of ehr elements
        struct ehr temp_EHR; //Makes temporary variable in of type "Struct ehr"
        while (fread(&temp_EHR, sizeof(struct ehr), 1, file_handler_EHRcheck) == 1) { //Reads the file in chunks of struct ehr
            EHRcounter++; // increments ehr counter by 1 for each unique appointment
        }
        fclose(file_handler_EHRcheck); //Closes file after reading all appointments
    }

    struct ehr* EHR_array = malloc(EHRcounter * sizeof(struct ehr)); //allocates memory for an array with "EHRcounter" Number of elemtns

    if (viewingEHRflag == true) { //If the file does not exist
        printf("Creating file\n");

        // Initialise default ehr
        struct ehr default_EHRs[] = {
            {"PA01", "Anwar Jabib", {20, 5, 1975}, "No significant medical history.", "None", "Ibuprofen", "Appendectomy in 1995", "Annual physical exam results normal."},
            {"PA02", "Martin Brundle", {4, 8, 1988}, "Asthma, controlled with medication.", "Peanuts", "Salbutamol", "None", "Asthma diagnosis confirmed by spirometry."},
            {"PA03", "Thomas Jefferson", {1, 12, 1992}, "Diabetic, type 2.", "Latex", "Metformin", "Gallbladder removal in 2010", "Type 2 diabetes diagnosed in 2015."},
            {"PA04", "James Maddison", {18, 4, 2001}, "History of migraines.", "Shellfish", "Sumatriptan", "Tonsillectomy in 2016", "MRI confirms no abnormalities."},
            {"PA05", "Aaron Burr", {30, 9, 1964}, "Survivor of myocardial infarction.", "Aspirin", "Lisinopril", "Coronary artery bypass graft in 2018", "Echocardiogram shows improved heart function."},
            {"PA06", "Brandon Williams", {14, 7, 1985}, "Hypertension, controlled with medication.", "None", "Amlodipine", "Appendectomy in 2003", "Blood pressure well-controlled on current regimen."},
            {"PA07", "Carlos Sainz", {28, 9, 1993}, "Seasonal allergies.", "Pollen", "Loratadine", "None", "Allergy tests confirm sensitivity to various pollens."},
        };

        EHRcounter = sizeof(default_EHRs) / sizeof(default_EHRs[0]);//Calculates number of appointments
        EHR_array = realloc(EHR_array, EHRcounter * sizeof(struct ehr)); //Reallocate memory for EHR_array to match number of default appointments array
        memcpy(EHR_array, default_EHRs, EHRcounter * sizeof(struct ehr)); //Copies all things from default appointment array to appointment array

        FILE* file_handler_EHRinsert;
        file_handler_EHRinsert = fopen("ehr.txt", "w"); //Opens file in write mode
        fwrite(EHR_array, sizeof(struct ehr), EHRcounter, file_handler_EHRinsert); //Writes appointments from ehr array to file
        fclose(file_handler_EHRinsert);//close file handler
        printf("EHR txt made!\n");
    }
};

int Initialise_AppointmentFile() {
    bool viewingappointmentflag = false; //Flag to measure if file exists or not
    int appointmentcounter = 0; //We will need this in later operations to define how long our arrays should be
    FILE* file_handler_appointmentcheck;
    file_handler_appointmentcheck = fopen("Appointment.txt", "r"); // Opens file
    if (file_handler_appointmentcheck == NULL) { //If the file comes back as null, means file does not exist
        viewingappointmentflag = true; //File does not exist hence we will need to make a file
    } else {
        // Read the file to count the number of appointments
        struct appointment temp_appointment; //Makes temporary variable in of type "Struct appointment"
        while (fread(&temp_appointment, sizeof(struct appointment), 1, file_handler_appointmentcheck) == 1) { //Reads the file in chunks of struct appointments
            appointmentcounter++; // increments appointment counter by 1 for each unique appointment
        }
        fclose(file_handler_appointmentcheck); //Closes file after reading all appointments
    }

    struct appointment* appointment_array = malloc(appointmentcounter * sizeof(struct appointment)); //allocates memory for an array with "appointmentcounter" Number of elemtns

    if (viewingappointmentflag == true) { //If the file does not exist
        printf("Creating file\n");

        // Initialise default appointments
        struct appointment default_appointments[] = {
            {"AP01","PA01","Anwar Jabib","DC01","Dr. Black","Cold","Finished",4,5,2024,13,00,14,00,"",-1},
            {"AP02","PA02","Martin Brundle","DC02","Dr. David","Broken Femur","Finished",4,5,2024,15,00,16,00,"",-1},
            {"AP03","PA03","Thomas Jefferson","DC04","Dr. Teo","Diahhrea","Cancelled",5,5,2024,13,00,14,00,"Leave",1},
            {"AP04","PA04","James Maddison","DC03","Dr. Clark","Swollen Finger","Finished",5,5,2024,15,00,16,00,"",-1},
            {"AP05","PA05","Aaron Burr","DC01","Dr. Black","Sprained Ankle","Finished",5,5,2024,17,00,18,00,"",-1},
            {"AP06","PA06","Brandon Williams","DC03","Dr. Clark","Cold","Cancelled",6,5,2024,9,00,10,00,"Doctor On Leave",1},
            {"AP07","PA07","Carlos Sainz","DC05","Dr. Who","Fever","Finished",6,5,2024,12,00,13,00,"",-1},
            {"AP08","PA01","Anwar Jabib","DC03","Dr. David","Sick","Finished",3,6,2024,14,00,15,00,"",-1},
            {"AP09","PA01","Anwar Jabib","DC03","Dr. Clark","Cough","Upcoming",10,6,2024,14,00,15,00,"",-1},
            {"AP10","PA02","Martin Brundle","DC02","Dr. David","Ankle Broken","Upcoming",10,6,2024,14,00,15,00,"",-1},
            {"AP11","PA03","Thomas Jefferson","DC01","Dr. Black","Fever","Upcoming",10,6,2024,14,00,15,00,"",-1},
            {"AP12","PA04","James Maddison","DC04","Dr. Teo","Ankle Broken","Upcoming",10,6,2024,14,00,15,00,"",-1},
            {"AP13","PA05","Thomas Jefferson","DC05","Dr. Who","Headaches","Upcoming",10,6,2024,14,00,15,00,"",-1},
        }; //Declares and initalize an array with these appointments

        appointmentcounter = sizeof(default_appointments) / sizeof(default_appointments[0]);//Calculates number of appointments
        appointment_array = realloc(appointment_array, appointmentcounter * sizeof(struct appointment)); //Reallocate memory for appointment_array to match number of default appointments array
        memcpy(appointment_array, default_appointments, appointmentcounter * sizeof(struct appointment)); //Copies all things from default appointment array to appointment array

        FILE* file_handler_appointmentinsert;
        file_handler_appointmentinsert = fopen("Appointment.txt", "w"); //Opens file in write mode
        fwrite(appointment_array, sizeof(struct appointment), appointmentcounter, file_handler_appointmentinsert); //Writes appointments from appointment array to file
        fclose(file_handler_appointmentinsert);//close file handler
        printf("Appointment txt made!\n");
    }

    free(appointment_array); //Frees memory that was allocated for "appointment array"
    return appointmentcounter; //Returns the appointment counter since we will need it later for when we need to open the file in other functions - so we dont recalculate in each function
};


int Initialise_TreatmentsFile() {
    bool fileExistsFlag = false;
    int treatmentsCounter = 0;

    // Check if the file exists
    FILE* fileCheck = fopen("treatments.txt", "r");
    if (fileCheck == NULL) {
        fileExistsFlag = true;
    } else {
        struct treatments tempTreatment;
        while (fread(&tempTreatment, sizeof(struct treatments), 1, fileCheck) == 1) {
            treatmentsCounter++;
        }
        fclose(fileCheck);
    }

    // Allocate memory for treatments array
    struct treatments* treatmentsArray = malloc(treatmentsCounter * sizeof(struct treatments));

    if (fileExistsFlag) {
        printf("Creating file...\n");

        // Initialise default treatments
        struct treatments defaultTreatments[] = {
            {"PA01", "Anwar Jabib", {4, 5, 2024}, "Cold", "Prescribed rest and increased fluid intake", "Recovered"},
            {"PA02", "Martin Brundle", {4, 5, 2024}, "Broken Femur", "Surgery and physical therapy", "Recovering"},
            {"PA03", "Thomas Jefferson", {5, 5, 2024}, "Diarrhea", "Antidiarrheal medication", "Failed"},
            {"PA04", "James Maddison", {5, 5, 2024}, "Swollen Finger", "Cold compress and anti-inflammatory drugs", "Recovered"},
            {"PA05", "Aaron Burr", {5, 5, 2024}, "Sprained Ankle", "Compression wrap and elevation", "Recovered"},
            {"PA06", "Brandon Williams", {6, 5, 2024}, "Cold", "Over-the-counter cold remedies", "Failed"},
            {"PA07", "Carlos Sainz", {6, 5, 2024}, "Fever", "Antipyretics and rest", "Recovered"},
            {"PA01", "Anwar Jabib", {3, 6, 2024}, "Sick", "General check-up and symptomatic treatment", "Recovered"},
            {"PA01", "Anwar Jabib", {10, 6, 2024}, "Cough", "Cough suppressants and expectorants", "Failed"},
            {"PA02", "Martin Brundle", {10, 6, 2024}, "Ankle Broken", "Cast immobilization and pain relievers", "No Change"}
        };

        treatmentsCounter = sizeof(defaultTreatments) / sizeof(defaultTreatments[0]);
        treatmentsArray = realloc(treatmentsArray, treatmentsCounter * sizeof(struct treatments));
        memcpy(treatmentsArray, defaultTreatments, treatmentsCounter * sizeof(struct treatments));

        // Write the default treatments to the file
        FILE* fileWrite = fopen("treatments.txt", "w");
        fwrite(treatmentsArray, sizeof(struct treatments), treatmentsCounter, fileWrite);
        fclose(fileWrite);

        printf("treatments.txt file created with default records.\n");
    }

    // Free the allocated memory
    free(treatmentsArray);

    return treatmentsCounter;
}


int Initialise_DoctorScheduleFile() {
    bool fileExistsFlag = false;
    int schedulesCounter = 0;

    // Check if the file exists
    FILE* fileCheck = fopen("doctor_schedule.txt", "r");
    if (fileCheck == NULL) {
        fileExistsFlag = true;
    } else {
        struct doctorSchedule tempSchedule;
        while (fread(&tempSchedule, sizeof(struct doctorSchedule), 1, fileCheck) == 1) {
            schedulesCounter++;
        }
        fclose(fileCheck);
    }

    // Allocate memory for schedules array
    struct doctorSchedule* schedulesArray = malloc(schedulesCounter * sizeof(struct doctorSchedule));

    if (fileExistsFlag) {
        printf("Creating file...\n");

        // Initialise default doctor schedules
        struct doctorSchedule defaultSchedules[] = {
            {"DC01", {17, 6, 2024}, {9, 0}, {16, 0}},
            {"DC02", {17, 6, 2024}, {9, 0}, {16, 0}},
            {"DC03", {17, 6, 2024}, {9, 0}, {16, 0}},
            {"DC04", {17, 6, 2024}, {9, 0}, {16, 0}},
            {"DC05", {17, 6, 2024}, {9, 0}, {16, 0}},
            {"DC01", {18, 6, 2024}, {9, 0}, {16, 0}},
            {"DC02", {18, 6, 2024}, {9, 0}, {16, 0}},
            {"DC03", {18, 6, 2024}, {9, 0}, {16, 0}},
            {"DC04", {18, 6, 2024}, {9, 0}, {16, 0}},
            {"DC05", {18, 6, 2024}, {9, 0}, {16, 0}}
        };

        schedulesCounter = sizeof(defaultSchedules) / sizeof(defaultSchedules[0]);
        schedulesArray = realloc(schedulesArray, schedulesCounter * sizeof(struct doctorSchedule));
        memcpy(schedulesArray, defaultSchedules, schedulesCounter * sizeof(struct doctorSchedule));

        // Write the default doctor schedules to the file
        FILE* fileWrite = fopen("doctor_schedule.txt", "w");
        fwrite(schedulesArray, sizeof(struct doctorSchedule), schedulesCounter, fileWrite);
        fclose(fileWrite);

        printf("doctor_schedule.txt file created with default records.\n");
    }

    // Free the allocated memory
    free(schedulesArray);

    return schedulesCounter;
}

////////////////////////
// File Read FUNCTIONS//
////////////////////////

// Function to read EHR records from a file and dump them into an array
struct ehr* ReadEHRsFromFile(const char* filename, int* recordCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Count the number of records in the file
    int counter = 0;
    struct ehr temp;
    while (fread(&temp, sizeof(struct ehr), 1, file) == 1) {
        counter++;
    }

    // Go back to the beginning of the file to read the records
    rewind(file);

    // Allocate memory for the records
    struct ehr* records = malloc(counter * sizeof(struct ehr));
    if (records == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read the records from the file into the array
    int i;
    for (i = 0; i < counter && fread(&records[i], sizeof(struct ehr), 1, file) == 1; i++);

    // Close the file
    fclose(file);

    // Set the record count
    *recordCount = counter;

    return records;
}

////////////////////////////////
// Array View/Update FUNCTIONS//
////////////////////////////////

void viewByPatientIDEHR(struct ehr* EHR_array, int EHRcounter, const char* patientID) {
    for (int i = 0; i < EHRcounter; i++) {
        if (strcmp(EHR_array[i].patientID, patientID) == 0) {
            printf("Patient ID: %s\n", EHR_array[i].patientID);
            printf("Patient Name: %s\n", EHR_array[i].patientName);
            printf("Date of Birth: %02d/%02d/%04d\n", EHR_array[i].dob.day, EHR_array[i].dob.month, EHR_array[i].dob.year);
            printf("Medical History: %s\n", EHR_array[i].medicalHistory);
            printf("Allergies: %s\n", EHR_array[i].allergies);
            printf("Medications: %s\n", EHR_array[i].medications);
            printf("Past Procedures: %s\n", EHR_array[i].pastProcedures);
            printf("Diagnostics: %s\n", EHR_array[i].diagnostics);
            return; // Record found, exit the function
        }
    }
    printf("No record found for Patient ID: %s\n", patientID); // If no record is found
}

void viewAppointmentsByDoctorID(const char* doctorID, int statusFilter) {
    int appointmentCount = Initialise_AppointmentFile(); // Get the number of appointments
    struct appointment* appointmentArray = malloc(appointmentCount * sizeof(struct appointment));

    // Open the file and read the appointments into the array
    FILE* fileHandler = fopen("Appointment.txt", "r");
    if (fileHandler == NULL) {
        printf("Error opening the file.\n");
        free(appointmentArray);
        return;
    }

    fread(appointmentArray, sizeof(struct appointment), appointmentCount, fileHandler);
    fclose(fileHandler);

    // Iterate through the array and print appointments that match the doctorID
    system("cls");
    int numberAppointments = 0;
    for (int i = 0; i < appointmentCount; i++) {
        if ((strcmp(appointmentArray[i].doctorID, doctorID) == 0) && ((strcmp(appointmentArray[i].status, "Upcoming") == 0 ) || statusFilter)) {
            printf("------------------\n");
            printf("AppointmentID:  %s\n", appointmentArray[i].appointmentID);
            printf("PatientID:      %s\n", appointmentArray[i].patientID);
            printf("Patient Name:   %s\n", appointmentArray[i].patientName);
            printf("DoctorID:       %s\n", appointmentArray[i].doctorID);
            printf("Doctor Name:    %s\n", appointmentArray[i].doctorName);
            printf("Reason:         %s\n", appointmentArray[i].reason);
            printf("Status:         %s\n", appointmentArray[i].status);
            printf("Date:           %02d/%02d/%04d\n", appointmentArray[i].apdate.day, appointmentArray[i].apdate.month, appointmentArray[i].apdate.year);
            printf("Start Time:     %02d:%02d\n", appointmentArray[i].starttime.hours, appointmentArray[i].starttime.minutes);
            printf("End Time:       %02d:%02d\n", appointmentArray[i].endtime.hours, appointmentArray[i].endtime.minutes);
            printf("--------------------");
            numberAppointments++;
            // Add more details as needed...
        }
    }
    free(appointmentArray);
    if (numberAppointments == 0){
        if(statusFilter == 1){
            printf("No appointments found");
        }
        else{
            printf("No upcoming appointments yet");
        }
    }
};

void viewTreatmentsByCondition(const char* condition) {
    int treatmentCount = Initialise_TreatmentsFile(); // Get the number of treatments
    struct treatments* treatmentArray = malloc(treatmentCount * sizeof(struct treatments));

    // Open the file and read the treatments into the array
    FILE* fileHandler = fopen("treatments.txt", "r");
    if (fileHandler == NULL) {
        printf("Error opening the file.\n");
        free(treatmentArray);
        return;
    }

    fread(treatmentArray, sizeof(struct treatments), treatmentCount, fileHandler);
    fclose(fileHandler);

    // Iterate through the array and print treatments that match the condition
    system("cls");
    int numberTreatments = 0;
    for (int i = 0; i < treatmentCount; i++) {
        if ((strcmp(treatmentArray[i].condition, condition) == 0) || 0){
            printf("------------------\n");
            printf("PatientID:      %s\n", treatmentArray[i].patientID);
            printf("Patient Name:   %s\n", treatmentArray[i].patientName);
            printf("Condition:      %s\n", treatmentArray[i].condition);
            printf("Treatment:      %s\n", treatmentArray[i].treatment);
            printf("Outcome:        %s\n", treatmentArray[i].outcome);
            printf("Date:           %02d/%02d/%04d\n", treatmentArray[i].treatmentdate.day, treatmentArray[i].treatmentdate.month, treatmentArray[i].treatmentdate.year);
            printf("------------------\n");
            numberTreatments++;
            // Add more details as needed...
        }
    }
    free(treatmentArray);
    if (numberTreatments == 0){
        printf("No treatments for %s has been recorded", condition);
    }
}

void viewTreatmentsByPatientID(const char* patientID) {
    int treatmentCount = Initialise_TreatmentsFile(); // Get the number of treatments
    struct treatments* treatmentArray = malloc(treatmentCount * sizeof(struct treatments));

    // Open the file and read the treatments into the array
    FILE* fileHandler = fopen("treatments.txt", "r");
    if (fileHandler == NULL) {
        printf("Error opening the file.\n");
        free(treatmentArray);
        return;
    }

    fread(treatmentArray, sizeof(struct treatments), treatmentCount, fileHandler);
    fclose(fileHandler);

    // Iterate through the array and print treatments that match the condition
    system("cls");
    int numberTreatments = 0;
    for (int i = 0; i < treatmentCount; i++) {
        if ((strcmp(treatmentArray[i].patientID, patientID) == 0) || 0){
            printf("------------------\n");
            printf("PatientID:      %s\n", treatmentArray[i].patientID);
            printf("Patient Name:   %s\n", treatmentArray[i].patientName);
            printf("Condition:      %s\n", treatmentArray[i].condition);
            printf("Treatment:      %s\n", treatmentArray[i].treatment);
            printf("Outcome:        %s\n", treatmentArray[i].outcome);
            printf("Date:           %02d/%02d/%04d\n", treatmentArray[i].treatmentdate.day, treatmentArray[i].treatmentdate.month, treatmentArray[i].treatmentdate.year);
            printf("------------------\n");
            numberTreatments++;
            // Add more details as needed...
        }
    }
    free(treatmentArray);
    if (numberTreatments == 0){
        printf("No treatments for %s has been recorded", patientID);
    }
}

void editDoctorSchedule(struct doctorSchedule* scheduleArray, int scheduleCount, const char* doctorID){
    struct date apdate;
    struct time newStartTime;
    struct time newEndTime;

    printf("--------------------------------\n");
    printf("Enter date you want to edit [DD/MM/YYYY]: ");
    scanf("%d/%d/%d", &apdate.day, &apdate.month, &apdate.year);

    int numberSchedules = 0;
    for (int i = 0; i < scheduleCount; i++) {
        if ((strcmp(scheduleArray[i].doctorID, doctorID) == 0) && (compareDates(scheduleArray[i].apdate, apdate) == 0)){
            printf("--------------------------------\n");
            printf("Enter new start time for %02d/%02d/%04d: \n", scheduleArray[i].apdate.day, scheduleArray[i].apdate.month, scheduleArray[i].apdate.year);

            while (scanf("%d:%d", &newStartTime.hours, &newStartTime.minutes) != 2 || !isValidTime(newStartTime)) {
                printf("Invalid time format or values. Please enter again [HH:MM]: ");
                while (getchar() != '\n'); // Clear input buffer
            }

            printf("Enter new end time [HH:MM]: \n");
            while (scanf("%d:%d", &newEndTime.hours, &newEndTime.minutes) != 2 || !isValidTime(newEndTime)) {
                printf("Invalid time format or values. Please enter again [HH:MM]: ");
                while (getchar() != '\n'); // Clear input buffer
            }

            scheduleArray[i].starttime = newStartTime;
            scheduleArray[i].endtime = newEndTime;

            numberSchedules++;
        }
    }
    if (numberSchedules == 0) {
        printf("No schedules found for that date \n");
        scanf("%d/%d/%d", &apdate.day, &apdate.month, &apdate.year);
        int prompt = 0;
        printf("\nEnter any number to return to menu: ");
        scanf("%d", &prompt);
        return;
    }

    FILE *writting_doctorschedule_fh = fopen("doctor_schedule.txt", "w");
    if (writting_doctorschedule_fh == NULL) {
        perror("Failed to open doctor_schedule.txt for writing");
    }

    fwrite(scheduleArray, sizeof(struct appointment), scheduleCount, writting_doctorschedule_fh);
    fclose(writting_doctorschedule_fh);

}

void viewDoctorScheduleByDoctorID(const char* doctorID) {
    int scheduleCount = Initialise_DoctorScheduleFile();
    struct doctorSchedule* scheduleArray = malloc(scheduleCount * sizeof(struct doctorSchedule));

    FILE* fileHandler = fopen("doctor_schedule.txt", "r");
    if (fileHandler == NULL) {
        printf("Error opening the file.\n");
        free(scheduleArray);
        return;
    }

    fread(scheduleArray, sizeof(struct doctorSchedule), scheduleCount, fileHandler);
    fclose(fileHandler);

    int numberSchedules = 0;
    struct date current_date;
    struct time current_time;
    get_current_datetime(&current_date, &current_time);

    system("cls");
    printf("Doctor Schedule for Doctor ID: %s\n", doctorID);

    printf("--------------------------------\n");
    for (int i = 0; i < scheduleCount; i++) {
        if ((strcmp(scheduleArray[i].doctorID, doctorID) == 0) && (compareDates(scheduleArray[i].apdate, current_date) >= 0)){
            printf("Date:        %02d/%02d/%04d\n", scheduleArray[i].apdate.day, scheduleArray[i].apdate.month, scheduleArray[i].apdate.year);
            printf("Start Time:       %02d:%02d\n", scheduleArray[i].starttime.hours, scheduleArray[i].starttime.minutes);
            printf("End Time:         %02d:%02d\n", scheduleArray[i].endtime.hours, scheduleArray[i].endtime.minutes);
            printf("--------------------------------\n");
            numberSchedules++;
        }
    }

    free(scheduleArray);
    if (numberSchedules > 0) {
        int promptSchedule = 0;
        printf("Enter 1 to edit a schedule, enter any other key to return: ");
        scanf("%d", &promptSchedule);
        if (promptSchedule) {
            editDoctorSchedule(scheduleArray, scheduleCount, doctorID);
        }
        else {
            return;
        }
    }
    if (numberSchedules == 0) {
        printf("No schedules found for Doctor ID: %s\n", doctorID);
    }
}

int doctorMainMenu(const char* doctorID) {
    int choice = 0;
    do {
        system("cls");
        printf("--------------------------------------\n");
        printf("|       Doctor Hub                   |\n");
        printf("--------------------------------------\n");
        printf("|       Electronic Health Records    |\n");
        printf("|       -----------------------------|\n");
        printf("|    1. Search EHR                   |\n");
        printf("|    2. Update EHR   disable         |\n");
        printf("|       -----------------------------|\n");
        printf("|       Appointments                 |\n");
        printf("|       -----------------------------|\n");
        printf("|    3. View All Appointments        |\n");
        printf("|    4. View Upcoming Appointments   |\n");
        printf("|    5. Manage Schedule              |\n");
        printf("|       -----------------------------|\n");
        printf("|       Reports & Analytics          |\n");
        printf("|       -----------------------------|\n");
        printf("|    6. Generate Report by PatientID |\n");
        printf("|    7. Generate Report by Condition |\n");
        printf("|       -----------------------------|\n");
        printf("|       -----------------------------|\n");
        printf("|    8. Logout to Main Menu          |\n");
        printf("|    9. Exit Program                 |\n");
        printf("--------------------------------------\n\n");
        printf("Enter the corresponding number for what you wish to do: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
            choice = 0; // Reset choice to a neutral value that does not match the exit condition
            continue; // Skip the rest of the loop and prompt again
        }
        int prompt = 0;
        switch (choice) {
        case 1:
            while(prompt == 0){
                system("cls");
                printf("Electronic Health Record Search\n");
                int EHRcount;
                struct ehr* EHRrecords = ReadEHRsFromFile("ehr.txt", &EHRcount);
                char patientID[10]; // Buffer to store patient ID
                printf("Enter patientID: ");
                scanf("%6s", patientID); // Read up to 6 characters for patient ID
                viewByPatientIDEHR(EHRrecords, EHRcount, patientID);
                printf("\nEnter 0 if you wish to search another ID, enter 1 to return: ");
                if (scanf("%d", &prompt) != 1) {
                    while (getchar() != '\n'); // Clear the input buffer
                    printf("Invalid input. Please enter 1 or 0.\n\n\n");
                    choice = 0; // Reset choice to a neutral value that does not match the exit condition
                    continue; // Skip the rest of the loop and prompt again
                }
            }
            printf("ok");
            break;
        case 2:
            continue;
        case 3:
            prompt = 0;
            viewAppointmentsByDoctorID(doctorID, 1);
            printf("\nEnter any number to return to menu: ");
            scanf("%d", &prompt);
            break;
        case 4:
            prompt = 0;
            viewAppointmentsByDoctorID(doctorID, 0);
            printf("\nEnter any number to return to menu: ");
            scanf("%d", &prompt);
            break;
        // ... handle other cases ...
        case 5:
            prompt = 0;
            viewDoctorScheduleByDoctorID(doctorID);
            printf("\nEnter any number to return to menu: ");
            scanf("%d", &prompt);
            break;
        case 6:
            prompt = 0;
            while(prompt == 0){
                char patientID[10]; // Buffer to store patient ID
                printf("Enter patient you want to search: ");
                scanf("%6s", patientID);
                viewTreatmentsByPatientID(patientID);
                printf("\nEnter 0 if you wish to search another patientID, enter 1 to return: ");
                if (scanf("%d", &prompt) != 1) {
                    while (getchar() != '\n'); // Clear the input buffer
                    printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
                    choice = 0; // Reset choice to a neutral value that does not match the exit condition
                    continue; // Skip the rest of the loop and prompt again
                }
            }
            printf("ok");
            break;

        case 7:
            prompt = 0;
            while(prompt == 0){
                char condition[100]; // Buffer to store patient ID
                printf("Enter condition you want to search: ");
                scanf("%99s", condition);
                viewTreatmentsByCondition(condition);
                printf("\nEnter 0 if you wish to search another Condition, enter 1 to return: ");
                if (scanf("%d", &prompt) != 1) {
                    while (getchar() != '\n'); // Clear the input buffer
                    printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
                    choice = 0; // Reset choice to a neutral value that does not match the exit condition
                    continue; // Skip the rest of the loop and prompt again
                }
            }
            printf("ok");
            break;
        case 8:
            printf("Loging out...\n");
            return 0;
        case 9:
            printf("Exiting...\n");
            exit(0);
            break; // This break exits the switch
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 9);
    return 0; // Return a value to indicate success
}

//////////////////////////
///Patient Functions
///Alfie
///TP075798
/////////////////////////

int Initialise_InsuranceClaimFile() {
    bool claimFlag = false;
    int claimCounter = 0;
    FILE *file_handler_checking;

    file_handler_checking = fopen("InsuranceClaims.txt", "r");
    if (file_handler_checking == NULL) {
        claimFlag = true;
    } else {
        // Read the file to count the number of claims
        struct insurance_claim temp_claim;
        while (fread(&temp_claim, sizeof(struct insurance_claim), 1, file_handler_checking) == 1) {
            claimCounter++;
        }
        fclose(file_handler_checking);
    }

    struct insurance_claim *claim_array = malloc(claimCounter * sizeof(struct insurance_claim));

    if (claimFlag == true) {
        printf("Creating file\n");

        // Initialize default insurance claims
        struct insurance_claim default_claims[] = {
            {"CLM01", "PA01", "AP01", "B01", "XYZ Insurance", "INS123456", "Rejected", 1000.00, {13, 5, 2024}, {0, 0, 0}, {13, 30}, {0, 0}},
            {"CLM02", "PA02", "AP02", "B02","ABC Insurance", "INS987654", "Pending", 1500.00, {14, 5, 2024}, {15, 5, 2024}, {14, 45}, {10, 0}},
            {"CLM03", "PA03", "AP03", "B03","DEF Insurance", "INS246810", "Rejected", 500.00, {15, 5, 2024}, {0, 0, 0}, {10, 0}, {0, 0}},
            {"CLM04", "PA04", "AP04", "B04","XYZ Insurance", "INS987654", "Approved", 2000.00, {16, 5, 2024}, {17, 5, 2024}, {11, 0}, {12, 0}},
            {"CLM05", "PA05", "AP05", "B05","DEF Insurance", "DEF Insurance", "Pending", 750.00, {17, 5, 2024}, {0, 0, 0}, {9, 0}, {0, 0}},
            {"CLM06", "PA01", "AP08", "B06","ABC Insurance", "INS987654", "Approved", 2100.00, {3,6,2024}, {9,6,2024}, {15,30}, {20,00}},
        };

        claimCounter = sizeof(default_claims) / sizeof(default_claims[0]);
        claim_array = realloc(claim_array, claimCounter * sizeof(struct insurance_claim));
        memcpy(claim_array, default_claims, claimCounter * sizeof(struct insurance_claim));

        FILE *file_handler_claims;
        file_handler_claims = fopen("InsuranceClaims.txt", "w");
        fwrite(claim_array, sizeof(struct insurance_claim), claimCounter, file_handler_claims);
        fclose(file_handler_claims);
        printf("InsuranceClaims.txt made!\n");
    }

    free(claim_array);
    return claimCounter;
};

int Initialise_BillingFile() {
    bool billingFlag = false;
    int billingcounter = 0;
    FILE *file_handler_checking;

    file_handler_checking = fopen("Billing.txt", "r");
    if (file_handler_checking == NULL) {
        billingFlag = true;
    } else {
        // Read the file to count the number of payments
        struct billing temp_billing;
        while (fread(&temp_billing, sizeof(struct billing), 1, file_handler_checking) == 1) {
            billingcounter++;
        }
        fclose(file_handler_checking);
    }

    struct billing *billing_array = malloc(billingcounter * sizeof(struct billing));

    if (billingFlag == true) {
        printf("Creating file\n");

        // Initialize default billing records
        struct billing default_billings[] = {
            {"B01","PA01","AP01", 100.00, 0, 0, 0, "Cash", 100.00, 0, 0.5, "", "", "", "Paid", {13,5,2024}, {13,30}, {16,5,2024}, {20,41}, {13,5,2024}, {13,30}, {16,5,2024}, {20,41}},
            {"B02","PA02","AP02", 1050.00, 0, 0, 0, "Cash", 2000.00, 950.00, 0.5, "", "", "", "Paid", {13,5,2024}, {13,35}, {15,5,2024}, {15,55}, {13,5,2024}, {13,35}, {15,5,2024}, {15,55}},
            {"B03","PA04","AP04", 500.00, 250.00, 250.00, 250.00, "Credit Card", 500.00, 0, 0.0, "XYZ Insurance", "INS123456", "CLM789012", "Pending", {14,5,2024}, {14,0}, {15,5,2024}, {9,30}, {14,5,2024}, {14,0}, {15,5,2024}, {9,30}},
            {"B04","PA05","AP05", 750.00, 500.00, 250.00, 500.00, "Cash", 1000.00, 250.00, 0.5, "ABC Insurance", "INS987654", "CLM345678", "Paid", {16,5,2024}, {12,45}, {17,5,2024}, {11,0}, {16,5,2024}, {12,45}, {17,5,2024}, {11,0}},
            {"B05","PA07","AP07", 1200.00, 800.00, 400.00, 800.00, "Check", 1200.00, 0, 0.0, "DEF Insurance", "INS246810", "CLM123456", "Pending", {15,5,2024}, {10,30}, {16,5,2024}, {15,45}, {15,5,2024}, {10,30}, {16,5,2024}, {15,45}},
            {"B06","PA01","AP08", 210.00, 0, 0, 0, "PayPal", 2100.00, 0, 0.0, "ABC Insurance", "INS987654", "CLM123676", "Unpaid", {3,6,2024}, {14,30}, {8,6,2024}, {20,41}, {3,6,2024}, {15,30}, {9,6,2024}, {20,00}},

        };

        billingcounter = sizeof(default_billings) / sizeof(default_billings[0]);
        billing_array = realloc(billing_array, billingcounter * sizeof(struct billing));
        memcpy(billing_array, default_billings, billingcounter * sizeof(struct billing));

        FILE *file_handler_payments;
        file_handler_payments = fopen("Billing.txt", "w");
        fwrite(billing_array, sizeof(struct billing), billingcounter, file_handler_payments);
        fclose(file_handler_payments);
        printf("Billing txt made!\n");
    }

    free(billing_array);
    return billingcounter;
};


int initialise_DoctorFile() {
    bool doctorsFlag = false;
    int doctorCounter = 0;
    FILE* file_handler_doctors;

    file_handler_doctors = fopen("Doctors.txt", "r");
    if (file_handler_doctors == NULL) {
        printf("File does not exist\n");
        doctorsFlag = true;
    } else {
        // Read the file to count the number of doctors
        struct doctor temp_doctor;
        while (fread(&temp_doctor, sizeof(struct doctor), 1, file_handler_doctors) == 1) {
            doctorCounter++;
        }
        fclose(file_handler_doctors);
    }

    struct doctor* doctors_array = malloc(doctorCounter * sizeof(struct doctor));

    if (doctorsFlag == true) {
        printf("Creating file\n");

        // Initialize default doctor records
        struct doctor default_doctors[] = {
            {"DC01", "Dr. Black"},
            {"DC02", "Dr. David"},
            {"DC03", "Dr. Clark"},
            {"DC04", "Dr. Teo"},
            {"DC05", "Dr. Who"}
        };

        doctorCounter = sizeof(default_doctors) / sizeof(default_doctors[0]);
        doctors_array = realloc(doctors_array, doctorCounter * sizeof(struct doctor));
        memcpy(doctors_array, default_doctors, doctorCounter * sizeof(struct doctor));

        FILE* fh_DoctorAdd;
        fh_DoctorAdd = fopen("Doctors.txt", "w");
        fwrite(doctors_array, sizeof(struct doctor), doctorCounter, fh_DoctorAdd);
        fclose(fh_DoctorAdd);
        printf("Doctor file Made!\n");
    }

    free(doctors_array);
    return doctorCounter;
};



void AppointmentHistory(appointmentcounter, patientID){


    FILE* fh_AppointmentHistory;
    fh_AppointmentHistory = fopen("Appointment.txt", "r");
    struct appointment *AppointmentHistory_array = malloc(appointmentcounter*sizeof(*AppointmentHistory_array));
    fread(&*AppointmentHistory_array, sizeof(struct appointment), appointmentcounter, fh_AppointmentHistory);
    fclose(fh_AppointmentHistory);

    bool AppointmentFlag = false;
    printf("Appointment counter %d", appointmentcounter);
    printf("PatientID %s\n\n", patientID);

    printf("Patient ID array %s\n\n", AppointmentHistory_array[7].patientID);
    int AllAppointments = 0;

    for (int i = 0; i < appointmentcounter; i++) {
        if (strcmp(AppointmentHistory_array[i].patientID, patientID) == 0) {
            printf("\n\nHere is the entire appointment History for %s\n\n", patientID);
            AppointmentFlag = true;
            AllAppointments ++;
            printAppointment(AppointmentHistory_array[i]);

        }
    }
    if (AppointmentFlag){
        printf("You have %d total appointment(s).\n\n", AllAppointments);
    }

    if (!AppointmentFlag) {
        printf("No record of appointments for %s found!\n", patientID);
    }

    free(AppointmentHistory_array);
    getch();
    return 0;



};


void UpcomingAppointments(appointmentcounter, patientID){


    FILE* fh_UpcomingAppointment;
    fh_UpcomingAppointment = fopen("Appointment.txt", "r");
    struct appointment *UpcomingAppointment_array = malloc(appointmentcounter*sizeof(*UpcomingAppointment_array));
    fread(&*UpcomingAppointment_array, sizeof(struct appointment), appointmentcounter, fh_UpcomingAppointment);
    fclose(fh_UpcomingAppointment);


    bool AppointmentFlag = false;
    printf("Appointment counter %d", appointmentcounter);
    printf("PatientID %s\n\n", patientID);

    int patientAppointments = 0;

    for (int i = 0; i < appointmentcounter; i++) {
        if (strcmp(UpcomingAppointment_array[i].patientID, patientID) == 0) {
            if (strcmp(UpcomingAppointment_array[i].status, "Upcoming") == 0) {
                printf("\n\nHere are the Upcoming appointments for %s\n\n", patientID);
                AppointmentFlag = true;
                patientAppointments ++;
                printAppointment(UpcomingAppointment_array[i]);
            }
        }
    }
    if (AppointmentFlag){
        printf("You have %d scheduled appointment(s).\n\n", patientAppointments);
    }

    if (!AppointmentFlag) {
        printf("No upcoming appointments for %s found!\n", patientID);
    }

    free(UpcomingAppointment_array);
    getch();
    return 0;

};


void viewAppointment(appointmentcounter, patientID){
    system("cls");
    int AppointmentChoice;
    printf("Appointment Counter: %d", appointmentcounter);



    while(1){
        system("cls");
        printf("-----------------------------------\n");
        printf("|   What would you like to view?   |\n");
        printf("------------------------------------\n");
        printf("|1. View Entire Appointment History|\n");
        printf("|2. View Upcoming Appointments     |\n");
        printf("|3. Return Home                    |\n");
        printf("------------------------------------\n");
        printf("\nEnter the corresponding number for what you wish to do: ");

        if (scanf("%d", &AppointmentChoice) != 1) {
                while (getchar() != '\n');
                printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
                getch();
                continue;
            }

        switch (AppointmentChoice) {
        case 1: AppointmentHistory(appointmentcounter, patientID);
            break;
        case 2: UpcomingAppointments(appointmentcounter, patientID);
            break;
        case 3: return 0;
            break;
        }
    }
};

void printAppointment(struct appointment a){
    printf("\n\n");
    printf("------------------------------------\n");
    printf("Appointment ID \t\t%s\n", a.appointmentID);
    printf("patient ID \t\t%s\n", a.patientID);
    printf("Patient Name \t\t%s\n", a.patientName);
    printf("Doctor ID \t\t%s\n", a.doctorID);
    printf("Doctor Name \t\t%s\n", a.doctorName);
    printf("patient ID \t\t%s\n", a.patientID);
    printf("Reason \t\t\t%s\n", a.reason);
    printf("Status \t\t\t%s\n", a.status);
    printf("Appointment Date: \t%d-%d-%d\n", a.apdate.year, a.apdate.month, a.apdate.day);
    printf("Appointment Time: \t%d:%d\n", a.starttime.hours, a.starttime.minutes);
    printf("Appointment End Time: \t%d:%d\n", a.endtime.hours, a.endtime.minutes);
    printf("------------------------------------\n");
};




// Function to check if a year is a leap year
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to check if the input date is valid
int is_valid_date(struct date d) {
    // Check if month is valid
    if (d.month < 1 || d.month > 12) {
        return 0;
    }

    // Check if day is valid for the given month
    if (d.day < 1) {
        return 0;
    }

    int days_in_month;
    switch (d.month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            days_in_month = 31;
            break;
        case 4: case 6: case 9: case 11:
            days_in_month = 30;
            break;
        case 2:
            if (is_leap_year(d.year)) {
                days_in_month = 29;
            } else {
                days_in_month = 28;
            }
            break;
        default:
            return 0;
    }

    return d.day <= days_in_month;
}



int makeAppointment(int appointmentcounter, int DoctorCounter, char patientID[10], char Name[50]) {

    //Opens Txt
    FILE *fh_doctors = fopen("Doctors.txt", "r");
    if (fh_doctors == NULL) {
        perror("Failed to open Doctors.txt");
        return -1;
    }

    struct doctor *doctors_array = malloc(DoctorCounter * sizeof(*doctors_array));
    if (doctors_array == NULL) {
        perror("Failed to allocate memory for doctors_array");
        fclose(fh_doctors);
        return -1;
    }

    fread(doctors_array, sizeof(struct doctor), DoctorCounter, fh_doctors);
    fclose(fh_doctors);



    //Initialises varibles
    char reason[50];
    struct date appointmentdate;
    struct time makingStartTime;
    int start_hour;
    int start_minute;
    bool InputValidity = false;

    while (!InputValidity) {
        printf("\nPatient ID: %s\n", patientID);
        printf("Patient Name: %s\n", Name);

        printf("Please input the reason for visit: ");
        getchar();  // to clear the newline left by previous scanf
        fgets(reason, sizeof(reason), stdin);
        reason[strcspn(reason, "\n")] = 0;  // Remove trailing newline

        printf("Enter a date for your appointment. [DD.MM.YYYY]: ");
        scanf("%d.%d.%d", &appointmentdate.day, &appointmentdate.month, &appointmentdate.year);

        printf("Enter the hour at which you would like this to occur. (24 Hour Format:) ");
        scanf("%d", &start_hour);
        makingStartTime.hours = start_hour;

        if (!is_valid_date(appointmentdate)) {
            printf("Invalid date input.\n");
            continue;
        }

        if (!is_valid_time(makingStartTime)) {
            printf("Invalid time input.\n");
            continue;
        }

        struct date current_date;
        struct time current_time;
        get_current_datetime(&current_date, &current_time);

        //Checks that the date enetered occurs after current time and date
        if (appointmentdate.year > current_date.year ||
            (appointmentdate.year == current_date.year && appointmentdate.month > current_date.month) ||
            (appointmentdate.year == current_date.year && appointmentdate.month == current_date.month && appointmentdate.day > current_date.day) ||
            (appointmentdate.year == current_date.year && appointmentdate.month == current_date.month && appointmentdate.day == current_date.day &&
             makingStartTime.hours > current_time.hours) ||
            (appointmentdate.year == current_date.year && appointmentdate.month == current_date.month && appointmentdate.day == current_date.day &&
             makingStartTime.hours == current_time.hours && makingStartTime.minutes > current_time.minutes)) {
            InputValidity = true;
        } else {
            printf("Error! Entered Date Occurs Before The Current Date and Time! Please Try Again!\n");
        }
    }

    FILE *fh_searchingappointments = fopen("Appointment.txt", "r");
    if (fh_searchingappointments == NULL) {
        perror("Failed to open Appointment.txt");
        free(doctors_array);
        return -1;
    }

    struct appointment *openappointments_array = malloc(appointmentcounter * sizeof(*openappointments_array));
    if (openappointments_array == NULL) {
        perror("Failed to allocate memory for openappointments_array");
        free(doctors_array);
        fclose(fh_searchingappointments);
        return -1;
    }

    fread(openappointments_array, sizeof(struct appointment), appointmentcounter, fh_searchingappointments);
    fclose(fh_searchingappointments);

    char appointmentdoctorName[50] = "";
    char appointmentdoctorID[10] = "";

    for (int i = 0; i < DoctorCounter; i++) {
        struct doctor target_doctor = doctors_array[i];
        bool breakflag = false;

        for (int x = 0; x < appointmentcounter; x++) {
            if (strcmp(openappointments_array[x].doctorID, target_doctor.doctorID) == 0 && openappointments_array[x].cancel_ref == -1){
                bool flag = datetimeChecker(appointmentdate, makingStartTime, openappointments_array[x].apdate, openappointments_array[x].starttime);
                if (flag) {
                    breakflag = true;
                    break;
                }
            }
        }

        if (!breakflag) {
            strcpy(appointmentdoctorID, target_doctor.doctorID);
            strcpy(appointmentdoctorName, target_doctor.doctorName);
            break;
        }
    }
    //How Appointment Maker Checks if doctor is free
    // Iterate Doctors: For each Doctor:
	// Iterate all appointments
	// Is target appointment assigned to current doctor
	// ^ If yes - is appointment time same as input time
	// ^ If yes - doctor not available BREAK
	// IF iterate through all appointments No Break -> Doctor is Available

    if (strcmp(appointmentdoctorName, "") == 0 && strcmp(appointmentdoctorID, "") == 0) {
        printf("No Doctors Available at this time! Please select a different date/time \n\n");
        getch();
        free(doctors_array);
        free(openappointments_array);
        return -1;
    }

    int maxAppointmentID = 0;
    for (int i = 0; i < appointmentcounter; i++) {
        const char *idStr = openappointments_array[i].appointmentID;
        int id = atoi(idStr + 2);
        if (id > maxAppointmentID) {
            maxAppointmentID = id;
        }
    }

    char newAppointmentID[10];
    sprintf(newAppointmentID, "AP%02d", maxAppointmentID + 1);



    FILE *fh_makingNewAppointment = fopen("Appointment.txt", "r");
    if (fh_makingNewAppointment == NULL) {
        perror("Failed to open Appointment.txt for reading");
        free(doctors_array);
        free(openappointments_array);
        return -1;
    }

    struct appointment *MakeNewAppointments_array = malloc(appointmentcounter * sizeof(*MakeNewAppointments_array));
    if (MakeNewAppointments_array == NULL) {
        perror("Failed to allocate memory for MakeNewAppointments_array");
        free(doctors_array);
        free(openappointments_array);
        fclose(fh_makingNewAppointment);
        return -1;
    }

    fread(MakeNewAppointments_array, sizeof(struct appointment), appointmentcounter, fh_makingNewAppointment);
    fclose(fh_makingNewAppointment);

    appointmentcounter += 1;

    struct appointment *Updatedappointment_array = malloc(appointmentcounter * sizeof(struct appointment));
    if (Updatedappointment_array == NULL) {
        perror("Failed to allocate memory for Updatedappointment_array");
        free(doctors_array);
        free(openappointments_array);
        free(MakeNewAppointments_array);
        return -1;
    }

    for (int i = 0; i < appointmentcounter - 1; i++) {
        Updatedappointment_array[i] = MakeNewAppointments_array[i];
    }

    struct time EndTime;
    EndTime.hours = start_hour + 1;
    EndTime.minutes = 0;


    struct appointment newAppointment;
    strcpy(newAppointment.appointmentID, newAppointmentID);
    strcpy(newAppointment.patientID, patientID);
    strcpy(newAppointment.patientName, Name);
    strcpy(newAppointment.doctorID, appointmentdoctorID);
    strcpy(newAppointment.doctorName, appointmentdoctorName);
    strcpy(newAppointment.reason, reason);
    strcpy(newAppointment.status, "Upcoming");
    newAppointment.apdate = appointmentdate;
    newAppointment.starttime = makingStartTime;
    newAppointment.endtime= EndTime;
    strcpy(newAppointment.cancel, "N/A");
    newAppointment.cancel_ref = -1;
    Updatedappointment_array[appointmentcounter - 1] = newAppointment;

    printf("Success! Here are your Appointment Details!\n");
    printf("-------------------------------------------------\n");
    printf(" Appointment ID \t%s \n", newAppointment.appointmentID);
    printf(" Patient ID \t\t%s \n", newAppointment.patientID);
    printf(" Patient Name \t\t%s\n", newAppointment.patientName);
    printf(" Doctor ID \t\t%s\n", newAppointment.doctorID);
    printf(" Doctor Name \t\t%s\n", newAppointment.doctorName);
    printf(" Reason \t\t%s\n", newAppointment.reason);
    printf(" Status \t\t%s\n", newAppointment.status);
    printf(" Appointment Date: \t%d-%d-%d\n", newAppointment.apdate.year, newAppointment.apdate.month, newAppointment.apdate.day);
    printf(" Appointment Time: \t%d:%d\n", newAppointment.starttime.hours, newAppointment.starttime.minutes);
    printf("-------------------------------------------------\n\n\n");

    FILE *writting_appointment_fh = fopen("Appointment.txt", "w");
    if (writting_appointment_fh == NULL) {
        perror("Failed to open Appointment.txt for writing");
        free(doctors_array);
        free(openappointments_array);
        free(MakeNewAppointments_array);
        free(Updatedappointment_array);
        return -1;
    }

    fwrite(Updatedappointment_array, sizeof(struct appointment), appointmentcounter, writting_appointment_fh);
    fclose(writting_appointment_fh);

    free(doctors_array);
    free(openappointments_array);
    free(MakeNewAppointments_array);
    free(Updatedappointment_array);

    getch();

    return appointmentcounter;
}


void DisplayBillingInfo(struct billing bill){
    printf("----------------------------------------------\n");
    printf("Billing ID:\t\t\t%s\n", bill.billingID);
    printf("Patient ID:\t\t\t%s\n", bill.patientID);
    printf("Appointment ID:\t\t\t%s\n", bill.appointmentID);
    printf("----------------------------------------------\n");
    printf("Total:\t\t\t\t$%.2lf\n", bill.total);
    printf("Insurance Covered Amount:\t$%.2lf\n", bill.insuranceCoveredAmount);
    printf("Patient Responsible Amount:\t$%.2lf\n", bill.patientResponsibleAmount);
    printf("Insurance Paid:\t\t\t$%.2lf\n", bill.insurancepaid);
    printf("----------------------------------------------\n");
    printf("Payment Method:\t\t\t%s\n", bill.paymentmethod);
    printf("Amount Given:\t\t\t$%.2lf\n", bill.amountgiven);
    printf("Change:\t\t\t\t$%.2lf\n", bill.change);
    printf("Tax:\t\t\t\t%.2lf%%\n", bill.tax);
    printf("----------------------------------------------\n");
    printf("Insurance Provider:\t\t%s\n", bill.insuranceProvider);
    printf("Insurance ID:\t\t\t%s\n", bill.insuranceID);
    printf("Claim ID:\t\t\t%s\n", bill.claimID);
    printf("Status:\t\t\t\t%s\n", bill.status);
    printf("----------------------------------------------\n");
    printf("Insurance Billing Date:\t\t%d-%d-%d\n", bill.insurancebillingdate.year, bill.insurancebillingdate.month, bill.insurancebillingdate.day);
    printf("Insurance Billing Time:\t\t%d:%d\n", bill.insurancebillingtime.hours, bill.insurancebillingtime.minutes);
    printf("Insurance Payment Date:\t\t%d-%d-%d\n", bill.insurancepaymentdate.year, bill.insurancepaymentdate.month, bill.insurancepaymentdate.day);
    printf("Insurance Payment Time:\t\t%d:%d\n", bill.insurancepaymenttime.hours, bill.insurancepaymenttime.minutes);
    printf("----------------------------------------------\n");
    printf("Billing Date:\t\t\t%d-%d-%d\n", bill.billingdate.year, bill.billingdate.month, bill.billingdate.day);
    printf("Billing Time:\t\t\t%d:%d\n", bill.billingtime.hours, bill.billingtime.minutes);
    printf("Payment Date:\t\t\t%d-%d-%d\n", bill.paymentdate.year, bill.paymentdate.month, bill.paymentdate.day);
    printf("Payment Time:\t\t\t%d:%d\n", bill.paymenttime.hours, bill.paymenttime.minutes);
    printf("----------------------------------------------\n");

};

void searchpaymenthistory(int paymentcounter, char patientID[10]) {
    char appointmentID[10];
    int attempts = 0;
    bool found = false;

    FILE* fh_searchingpayment = fopen("Billing.txt", "r");
    if (fh_searchingpayment == NULL) {
        printf("Billing file not found!\n");
        return;
    }

    struct billing* openbilling_array = malloc(paymentcounter * sizeof(struct billing));
    if (openbilling_array == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fh_searchingpayment);
        return;
    }

    fread(openbilling_array, sizeof(struct billing), paymentcounter, fh_searchingpayment);
    fclose(fh_searchingpayment);

    do {
        printf("Please enter the Appointment ID you wish to search for: ");
        scanf("%s", appointmentID);

        for (int i = 0; i < paymentcounter; i++) {
            if (strcmp(openbilling_array[i].appointmentID, appointmentID) == 0) {
                if (strcmp(openbilling_array[i].patientID, patientID) == 0) {
                    printf("\n\nMatch found for Appointment ID %s\n", appointmentID);
                    DisplayBillingInfo(openbilling_array[i]);
                    found = true;
                    break;
                } else {
                    printf("Appointment ID %s does not belong to Patient ID %s\n", appointmentID, patientID);
                    break;
                }
            }
        }

        if (!found) {
            attempts++;
            printf("\nThat Appointment ID does not exist! You have %d attempt(s) left.\n", 3 - attempts);
        }

    } while (!found && attempts < 3);

    if (!found && attempts == 3) {
        printf("You have used up all your attempts!");
        return -1;

    }
    getch();
    free(openbilling_array);
};



void viewpaymenthistory(int paymentcounter, char patientID[10]) {
    int attempts = 0;
    bool found = false;

    FILE* fh_searchingpayment = fopen("Billing.txt", "r");
    if (fh_searchingpayment == NULL) {
        printf("Billing file not found!\n");
        return;
    }

    struct billing* openbilling_array = malloc(paymentcounter * sizeof(struct billing));
    if (openbilling_array == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fh_searchingpayment);
        return;
    }

    fread(openbilling_array, sizeof(struct billing), paymentcounter, fh_searchingpayment);
    fclose(fh_searchingpayment);
    int bills = 0;

    do {
        found = false;

        for (int i = 0; i < paymentcounter; i++) {
            if (strcmp(openbilling_array[i].patientID, patientID) == 0) {
                bills = bills +1;
                printf("\n\nMatch found for patient ID %s\n", patientID);
                printf("\n\nBill No. %d\n\n", bills);
                DisplayBillingInfo(openbilling_array[i]);
                found = true;
                break;
            }
        }

        if (!found) {
            attempts++;
            printf("\nThat Patient ID does not exist! You have %d attempt(s) left.\n", 3 - attempts);
        }

    } while (!found && attempts < 3);

    if (!found && attempts == 3) {
        printf("You have used up all your attempts!\nPlease try again after re-logging in.\n");
    }
    getch();
    free(openbilling_array);
};


void DisplayInsuranceInfo(struct insurance_claim claim){
    printf("----------------------------------------------\n");
    printf("Claim ID:\t\t\t%s\n", claim.claimID);
    printf("Patient ID:\t\t\t%s\n", claim.patientID);
    printf("Appointment ID:\t\t\t%s\n", claim.appointmentID);
    printf("Billing ID:\t\t\t%s\n", claim.billingID);
    printf("----------------------------------------------\n");
    printf("Insurance Company:\t\t%s\n", claim.insuranceCompany);
    printf("Insurance Number:\t\t%s\n", claim.insuranceNumber);
    printf("Claim Status:\t\t\t%s\n", claim.claimStatus);
    printf("----------------------------------------------\n");
    printf("Claim Amount:\t\t\t$%.2lf\n", claim.claimAmount);
    printf("----------------------------------------------\n");
    printf("Claim Date:\t\t\t%d-%d-%d\n", claim.claimDate.year, claim.claimDate.month, claim.claimDate.day);
    printf("Claim Time:\t\t\t%d:%d\n", claim.claimTime.hours, claim.claimTime.minutes);
    printf("Approval Date:\t\t\t%d-%d-%d\n", claim.approvalDate.year, claim.approvalDate.month, claim.approvalDate.day);
    printf("Approval Time:\t\t\t%d:%d\n", claim.approvalTime.hours, claim.approvalTime.minutes);
    printf("----------------------------------------------\n");

};

void ViewClaims(ClaimCounter, patientID){
    int attempts = 0;
    bool found = false;

    FILE* fh_SearchingClaims = fopen("InsuranceClaims.txt", "r");
    if (fh_SearchingClaims == NULL) {
        printf("Billing file not found!\n");
        return;
    }

    struct insurance_claim* openClaims_array = malloc(ClaimCounter * sizeof(struct insurance_claim));
    if (openClaims_array == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fh_SearchingClaims);
        return;
    }

    fread(openClaims_array, sizeof(struct insurance_claim), ClaimCounter, fh_SearchingClaims);
    fclose(fh_SearchingClaims);

    int claims = 0;

    do {
        found = false;

        for (int i = 0; i < ClaimCounter; i++) {
            if (strcmp(openClaims_array[i].patientID, patientID) == 0) {
                claims++;
                printf("\n\nMatch found for patient ID %s\n", patientID);
                printf("\n\nClaims No. %d\n\n", claims);
                DisplayInsuranceInfo(openClaims_array[i]);
                found = true;
            }
        }

        if (!found) {
            attempts++;
            printf("\nThat Patient ID does not exist! You have %d attempt(s) left.\n", 3 - attempts);
        }

    } while (!found && attempts < 3);

    if (!found && attempts == 3) {
        printf("You have used up all your attempts!\nPlease try again after re-logging in.\n");
    }
    getch();
    free(openClaims_array);
};

void SearchClaims(ClaimCounter, patientID){
    int attempts = 0;
    bool found = false;

    FILE* fh_SearchingClaims = fopen("InsuranceClaims.txt", "r");
    if (fh_SearchingClaims == NULL) {
        printf("Insurance claims file not found!\n");
        return;
    }

    struct insurance_claim* openClaims_array = malloc(ClaimCounter * sizeof(struct insurance_claim));
    if (openClaims_array == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fh_SearchingClaims);
        return;
    }

    fread(openClaims_array, sizeof(struct insurance_claim), ClaimCounter, fh_SearchingClaims);
    fclose(fh_SearchingClaims);

    char AppointmentID[10];

    do {
        found = false;
        printf("Enter the appointment ID to search for claims: ");
        scanf("%s", AppointmentID);
        for (int i = 0; AppointmentID[i]; i++) {
            AppointmentID[i] = toupper(AppointmentID[i]);
            }
        for (int i = 0; i < ClaimCounter; i++) {
            if (strcmp(openClaims_array[i].appointmentID, AppointmentID) == 0 &&
                strcmp(openClaims_array[i].patientID, patientID) == 0) {
                DisplayInsuranceInfo(openClaims_array[i]);
                found = true;
                break;
            }
        }

        if (!found) {
            attempts++;
            printf("\nNo matching claims found for appointment ID %s for patient ID %s! You have %d attempt(s) left.\n", AppointmentID, patientID, 3 - attempts);
        }

    } while (!found && attempts < 3);

    if (!found && attempts == 3) {
        printf("You have used up all your attempts!\nPlease try again after re-logging in.\n");
    }
    getch();
    free(openClaims_array);
};

void InsuranceClaimsPage(ClaimCounter, patientID){
    int InsuranceClaimsChoice;


    while(1){
        system("cls");
        printf("-----------------------------------\n");
        printf("|   What would you like to view?   |\n");
        printf("------------------------------------\n");
        printf("|1. View all Insurance Claims      |\n");
        printf("|2. Search Insurance Claims        |\n");
        printf("|3. Return Home                    |\n");
        printf("------------------------------------\n");
        printf("\nEnter the corresponding number for what you wish to do: ");

        if (scanf("%d", &InsuranceClaimsChoice) != 1) {
                while (getchar() != '\n');
                printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
                continue;
            }

        switch (InsuranceClaimsChoice) {
        case 1: ViewClaims(ClaimCounter, patientID);
            break;
        case 2: SearchClaims(ClaimCounter, patientID);
            break;
        case 3: return 0;
            break;
        }
    }
};


void BillingDetails(paymentcounter, InputPatientID) {
    char input[10];
    int option;
    bool repeat = true;

    while (repeat == true) {
        system("cls");
        printf("\n\n");
        printf("-----------------------------------\n");
        printf("|           Billing Page           |\n");
        printf("------------------------------------\n");
        printf("|        1. View Payment History   |\n");
        printf("|        2. Search Payment History |\n");
        printf("|        3. Exit                   |\n");
        printf("------------------------------------\n");
        printf("\nEnter the corresponding number for what you wish to do: ");
        scanf("%s", input);
        printf("\n\n");

        // Convert the input string to an integer
        option = atoi(input);
        // Check if the input is within the valid range
        if (option >= 1 && option <= 3) {
            if (option == 1) {
                viewpaymenthistory(paymentcounter, InputPatientID);
            } else if (option == 2) {
                searchpaymenthistory(paymentcounter, InputPatientID);
            } else if (option == 3) {
                repeat = false; // Exit the loop and function
            }
        } else {
            printf("Invalid Input! Please enter 1, 2, or 3.\n");
            getch();
        }
    }
};





int rescheduleAppointment(int appointmentcounter, int DoctorCounter, char patientID[10], char Name[50]) {
    printf("PatientID: %s\n", patientID);
    // Check if the appointment ID exists in the system
    char AppointmentID[20];
    printf("Enter the appointment ID you wish to reschedule: ");
    scanf("%s", AppointmentID);

    for (int i = 0; AppointmentID[i]; i++) {
        AppointmentID[i] = toupper(AppointmentID[i]);
    }

    FILE *fh_searchingappointments = fopen("Appointment.txt", "r");
    if (fh_searchingappointments == NULL) {
        perror("Failed to open Appointment.txt");
        return -1;
    }

    struct appointment *openappointments_array = malloc(appointmentcounter * sizeof(struct appointment));
    if (openappointments_array == NULL) {
        perror("Failed to allocate memory for openappointments_array");
        fclose(fh_searchingappointments);
        return -1;
    }

    fread(openappointments_array, sizeof(struct appointment), appointmentcounter, fh_searchingappointments);
    fclose(fh_searchingappointments);

    int rescheduledIndex = -1;
    for (int i = 0; i < appointmentcounter; i++) {
        if (strcmp(openappointments_array[i].appointmentID, AppointmentID) == 0) {
            rescheduledIndex = i;
            break;
        }
    }

    if (rescheduledIndex == -1) {
        printf("Appointment ID not found. Please try again.\n");
        free(openappointments_array);
        return -1;
    }

    // Open Doctors.txt
    FILE *fh_doctors = fopen("Doctors.txt", "r");
    if (fh_doctors == NULL) {
        perror("Failed to open Doctors.txt");
        free(openappointments_array);
        return -1;
    }

    struct doctor *doctors_array = malloc(DoctorCounter * sizeof(*doctors_array));
    if (doctors_array == NULL) {
        perror("Failed to allocate memory for doctors_array");
        fclose(fh_doctors);
        free(openappointments_array);
        return -1;
    }

    fread(doctors_array, sizeof(struct doctor), DoctorCounter, fh_doctors);
    fclose(fh_doctors);

    // Initialize variables
    char reason[50];
    struct date appointmentdate;
    struct time makingStartTime;
    int start_hour;
    int start_minute;
    bool InputValidity = false;

    while (!InputValidity) {
        printf("\nPatient ID: %s\n", patientID);
        printf("Patient Name: %s\n", Name);

        printf("Enter a new date for your appointment. [DD.MM.YYYY]: ");
        scanf("%d.%d.%d", &appointmentdate.day, &appointmentdate.month, &appointmentdate.year);

        printf("Enter the hour at which you would like this to occur. (24 Hour Format:) ");
        scanf("%d", &start_hour);
        makingStartTime.hours = start_hour;
        makingStartTime.minutes = 0;

        if (!is_valid_date(appointmentdate)) {
            printf("Invalid date input.\n");
            continue;
        }

        if (!is_valid_time(makingStartTime)) {
            printf("Invalid time input.\n");
            continue;
        }

        struct date current_date;
        struct time current_time;
        get_current_datetime(&current_date, &current_time);

        // Check that the date entered occurs after the current time and date
        if (appointmentdate.year > current_date.year ||
            (appointmentdate.year == current_date.year && appointmentdate.month > current_date.month) ||
            (appointmentdate.year == current_date.year && appointmentdate.month == current_date.month && appointmentdate.day > current_date.day) ||
            (appointmentdate.year == current_date.year && appointmentdate.month == current_date.month && appointmentdate.day == current_date.day &&
             makingStartTime.hours > current_time.hours) ||
            (appointmentdate.year == current_date.year && appointmentdate.month == current_date.month && appointmentdate.day == current_date.day &&
             makingStartTime.hours == current_time.hours && makingStartTime.minutes > current_time.minutes)) {
            InputValidity = true;
        } else {
            printf("Error! Entered Date Occurs Before The Current Date and Time! Please Try Again!\n");
        }
    }

    char appointmentdoctorName[50] = "";
    char appointmentdoctorID[10] = "";

    for (int i = 0; i < DoctorCounter; i++) {
        struct doctor target_doctor = doctors_array[i];
        bool breakflag = false;

        for (int x = 0; x < appointmentcounter; x++) {
            if (strcmp(openappointments_array[x].doctorID, target_doctor.doctorID) == 0 && openappointments_array[x].cancel_ref == -1) {
                bool flag = datetimeChecker(appointmentdate, makingStartTime, openappointments_array[x].apdate, openappointments_array[x].starttime);
                if (!flag) {
                    breakflag = true;
                    break;
                }
            }
        }

        if (breakflag) {
            strcpy(appointmentdoctorID, target_doctor.doctorID);
            strcpy(appointmentdoctorName, target_doctor.doctorName);
            break;
        }
    }

    if (strcmp(appointmentdoctorName, "") == 0 && strcmp(appointmentdoctorID, "") == 0) {
        printf("No Doctors Available at this time! Please select a different date/time \n\n");
        free(doctors_array);
        free(openappointments_array);
        return -1;
    }

    // Open Appointment.txt for reading
    FILE *fh_ReschedulingNewAppointment = fopen("Appointment.txt", "r");
    if (fh_ReschedulingNewAppointment == NULL) {
        perror("Failed to open Appointment.txt for reading");
        free(doctors_array);
        free(openappointments_array);
        return -1;
    }

    struct appointment *RescheduleAppointments_array = malloc(appointmentcounter * sizeof(struct appointment));
    if (RescheduleAppointments_array == NULL) {
        perror("Failed to allocate memory for RescheduleAppointments_array");
        free(doctors_array);
        free(openappointments_array);
        fclose(fh_ReschedulingNewAppointment);
        return -1;
    }

    fread(RescheduleAppointments_array, sizeof(struct appointment), appointmentcounter, fh_ReschedulingNewAppointment);
    fclose(fh_ReschedulingNewAppointment);

    // New Appointment ID making
    int maxAppointmentIDReschedule = 0;
    for (int i = 0; i < appointmentcounter; i++) {
        const char *idStr = openappointments_array[i].appointmentID;
        int id = atoi(idStr + 2);
        if (id > maxAppointmentIDReschedule) {
            maxAppointmentIDReschedule = id;
        }
    }

    char newAppointmentID[10];
    sprintf(newAppointmentID, "AP%02d", maxAppointmentIDReschedule + 1);

    // Update the appointment counter for the new appointment
    struct appointment *Updatedappointment_array = malloc((appointmentcounter + 1) * sizeof(struct appointment));
    if (Updatedappointment_array == NULL) {
        perror("Failed to allocate memory for Updatedappointment_array");
        free(doctors_array);
        free(RescheduleAppointments_array);
        return -1;
    }

    // Copy existing appointments to Updatedappointment_array
    for (int i = 0; i < appointmentcounter; i++) {
        Updatedappointment_array[i] = RescheduleAppointments_array[i];
    }

    // Update Old appointment status
    strcpy(Updatedappointment_array[rescheduledIndex].status, "Rescheduled");
    Updatedappointment_array[rescheduledIndex].cancel_ref = 1;

    struct time EndTime;
    EndTime.hours = start_hour + 1;
    EndTime.minutes = 0;

    struct appointment NewRescheduledAppointment;
    strcpy(NewRescheduledAppointment.appointmentID, newAppointmentID);
    strcpy(NewRescheduledAppointment.patientID, patientID);
    strcpy(NewRescheduledAppointment.patientName, Name);
    strcpy(NewRescheduledAppointment.doctorID, appointmentdoctorID);
    strcpy(NewRescheduledAppointment.doctorName, appointmentdoctorName);
    strcpy(NewRescheduledAppointment.reason, Updatedappointment_array[rescheduledIndex].reason);
    strcpy(NewRescheduledAppointment.status, "Upcoming");
    NewRescheduledAppointment.apdate = appointmentdate;
    NewRescheduledAppointment.starttime = makingStartTime;
    NewRescheduledAppointment.endtime = EndTime;
    strcpy(NewRescheduledAppointment.cancel, "");
    NewRescheduledAppointment.cancel_ref = -1;

    Updatedappointment_array[appointmentcounter] = NewRescheduledAppointment;

    // Display updated appointment details
    printf("\n\nSuccess! Here are your Appointment Details!\n");
    printf("-------------------------------------------------\n");
    printf(" Appointment ID \t%s \n", NewRescheduledAppointment.appointmentID);
    printf(" Patient ID \t\t%s \n", NewRescheduledAppointment.patientID);
    printf(" Patient Name \t\t%s\n", NewRescheduledAppointment.patientName);
    printf(" Doctor ID \t\t%s\n", NewRescheduledAppointment.doctorID);
    printf(" Doctor Name \t\t%s\n", NewRescheduledAppointment.doctorName);
    printf(" Reason \t\t%s\n", NewRescheduledAppointment.reason);
    printf(" Status \t\t%s\n", NewRescheduledAppointment.status);
    printf(" Appointment Date: \t%d-%d-%d\n", NewRescheduledAppointment.apdate.year, NewRescheduledAppointment.apdate.month, NewRescheduledAppointment.apdate.day);
    printf(" Appointment Time: \t%d:%d\n", NewRescheduledAppointment.starttime.hours, NewRescheduledAppointment.starttime.minutes);
    printf("-------------------------------------------------\n\n\n");

    // Open Appointment file for writing
    FILE *writting_appointment_fh = fopen("Appointment.txt", "w");
    if (writting_appointment_fh == NULL) {
        perror("Failed to open Appointment.txt for writing");
        free(doctors_array);
        free(RescheduleAppointments_array);
        free(Updatedappointment_array);
        return -1;
    }
    appointmentcounter = appointmentcounter+1;
    // Write updated array to file
    fwrite(Updatedappointment_array, sizeof(struct appointment), appointmentcounter, writting_appointment_fh);
    fclose(writting_appointment_fh);

    free(doctors_array);
    free(RescheduleAppointments_array);
    free(Updatedappointment_array);

    getch();

    return appointmentcounter;
};




int cancelAppointment(int appointmentcounter, char patientID[10]) {
    char AppointmentID[20];

    printf("Enter the appointment ID you wish to cancel: ");
    scanf("%s", AppointmentID);
    for (int i = 0; AppointmentID[i]; i++) {
        AppointmentID[i] = toupper(AppointmentID[i]);
    }

    FILE *fh_searchingappointments = fopen("Appointment.txt", "r");
    if (fh_searchingappointments == NULL) {
        perror("Failed to open Appointment.txt");
        return -1;
    }

    struct appointment *openappointments_array = malloc(appointmentcounter * sizeof(struct appointment));
    if (openappointments_array == NULL) {
        perror("Failed to allocate memory for openappointments_array");
        fclose(fh_searchingappointments);
        return -1;
    }

    fread(openappointments_array, sizeof(struct appointment), appointmentcounter, fh_searchingappointments);
    fclose(fh_searchingappointments);

    int cancelIndex = -1;
    for (int i = 0; i < appointmentcounter; i++) {
        if (strcmp(openappointments_array[i].appointmentID, AppointmentID) == 0 && strcmp(openappointments_array[i].patientID, patientID) == 0) {
            cancelIndex = i;
            break;
        }
    }

    if (cancelIndex == -1) {
        printf("Appointment ID not found or does not belong to the given patient. Please try again.\n");
        free(openappointments_array);
        return -1;
    }

    // Update the cancel_ref to 1
    openappointments_array[cancelIndex].cancel_ref = 1;
    strcpy(openappointments_array[cancelIndex].status, "Cancelled");
    strcpy(openappointments_array[cancelIndex].cancel, "Cancelled by Patient");

    // Write the updated array back to the file
    FILE *fh_writingappointments = fopen("Appointment.txt", "w");
    if (fh_writingappointments == NULL) {
        perror("Failed to open Appointment.txt for writing");
        free(openappointments_array);
        return -1;
    }

    fwrite(openappointments_array, sizeof(struct appointment), appointmentcounter, fh_writingappointments);
    fclose(fh_writingappointments);

    free(openappointments_array);

    printf("Appointment successfully cancelled.\n");

    getch();  // Pause the program
    return 0;
};


void printEHR(struct ehr record) {
    printf("\n\n");
    printf("           Patient Details\n");
    printf("------------------------------------\n");
    printf("Patient ID: \t\t%s\n", record.patientID);
    printf("Patient Name: \t\t%s\n", record.patientName);
    printf("Date of Birth: \t\t%d-%d-%d\n", record.dob.year, record.dob.month, record.dob.day);
    printf("------------------------------------\n\n");
    printf("          Medical Details\n");
    printf("------------------------------------\n\n");
    printf("Medical History: \t%s\n", record.medicalHistory);
    printf("Prescriptions: \t\t%s\n", record.medications);
    printf("Allergies: \t\t%s\n", record.allergies);
    printf("------------------------------------\n\n");
    getch();
};

void printAppointmentEHR(struct appointment record){
    printf("\n");
    printf("         Appointment History\n");
    printf("------------------------------------\n");
    printf("Appointment ID \t\t%s\n", record.appointmentID);
    printf("Doctor Name \t\t%s\n", record.doctorName);
    printf("Reason \t\t\t%s\n", record.reason);
    printf("Appointment Date: \t%d-%d-%d\n", record.apdate.year, record.apdate.month, record.apdate.day);
    printf("------------------------------------\n");
};

void DisplayBillingInfoEHR(struct billing record) {
    printf("\n");
    printf("         Billing History\n");
    printf("------------------------------------\n");
    printf("Appointment ID \t\t%s\n", record.appointmentID);
    printf("Billing ID \t\t%s\n", record.billingID);
    printf("Payment Amount \t\t%.2lf\n", record.total);
    printf("Billing Date: \t\t%d-%d-%d\n", record.billingdate.year, record.billingdate.month, record.billingdate.day);
    printf("Status:\t\t\t%s\n", record.status);
    printf("------------------------------------\n");
};

void printPaymentEHR(int paymentcounter, char* patientID) {
    bool EHRBillsfound = false;

    FILE* fh_searchingpayment = fopen("Billing.txt", "r");
    if (fh_searchingpayment == NULL) {
        printf("Billing file not found!\n");
        return;
    }

    struct billing* openbillingEHR_array = malloc(paymentcounter * sizeof(struct billing));
    if (openbillingEHR_array == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fh_searchingpayment);
        return;
    }

    fread(openbillingEHR_array, sizeof(struct billing), paymentcounter, fh_searchingpayment);
    fclose(fh_searchingpayment);
    int bills = 0;

    for (int i = 0; i < paymentcounter; i++) {
        if (strcmp(openbillingEHR_array[i].patientID, patientID) == 0) {
            bills++;
            printf("\n\nBill No. %d\n\n", bills);
            DisplayBillingInfoEHR(openbillingEHR_array[i]);
            EHRBillsfound = true;
        }
    }

    if (!EHRBillsfound) {
        printf("No billing records found for %s!\n", patientID);
    }

    free(openbillingEHR_array);
};



void AppointmentEHR(appointmentcounter, patientID){
    FILE* fh_EHRAppointment;
    fh_EHRAppointment = fopen("Appointment.txt", "r");
    struct appointment *AppointmentEHR_array = malloc(appointmentcounter*sizeof(*AppointmentEHR_array));
    fread(&*AppointmentEHR_array, sizeof(struct appointment), appointmentcounter, fh_EHRAppointment);
    fclose(fh_EHRAppointment);

    bool AppointmentFlag = false;

    int AllAppointments = 0;

    for (int i = 0; i < appointmentcounter; i++) {
        if (strcmp(AppointmentEHR_array[i].patientID, patientID) == 0) {
            if (strcmp(AppointmentEHR_array[i].status, "Finished") == 0) {
                    AppointmentFlag = true;
                    AllAppointments ++;
                    printAppointmentEHR(AppointmentEHR_array[i]);

        }
        }
    }
    if (AppointmentFlag){
        printf("You have %d total of finished appointment(s).\n\n", AllAppointments);
    }

    if (!AppointmentFlag) {
        printf("No record of appointments for %s found!\n", patientID);
    }

    free(AppointmentEHR_array);
    getch();
    return 0;

};

void viewEHR(patientID, EHRcounter, appointmentcounter, paymentcounter) {
    FILE* fh_EHR = fopen("EHR.txt", "r");
    if (fh_EHR == NULL) {
        printf("Failed to open EHR file\n");
        return;
    }

    struct ehr* EHR_array = malloc(EHRcounter * sizeof(struct ehr));
    if (EHR_array == NULL) {
        printf("Memory allocation failed\n");
        fclose(fh_EHR);
        return;
    }

    fread(EHR_array, sizeof(struct ehr), EHRcounter, fh_EHR);
    fclose(fh_EHR);

    bool EHRFlag = false;

    for (int i = 0; i < EHRcounter; i++) {
        if (strcmp(EHR_array[i].patientID, patientID) == 0) {
            printf("\n\nHere is the Electronic Health Record pertaining to %s\n", patientID);
            EHRFlag = true;
            printf("i = %d", i);
            printEHR(EHR_array[i]);
            AppointmentEHR(appointmentcounter, patientID);
            printPaymentEHR(paymentcounter, patientID);

        }
    }

    if (!EHRFlag) {
        printf("No record of appointments for %s found!\n", patientID);
    }

    free(EHR_array);
    getch();
    return 0;
};


void exiting(){
    printf("Exiting\n");
    getch();
    PatientMain();
};


int mainmenu(patientID, users, Name) {
    int choice;
    int paymentcounter = Initialise_BillingFile();
    int appointmentcounter = Initialise_AppointmentFile();
    int DoctorCounter = initialise_DoctorFile();
    int ClaimCounter = Initialise_InsuranceClaimFile();
    int EHRcounter = Initialise_EHRFile();


    while (1) {
        system("cls");
        printf("--------------------------------\n");
        printf("|   Patient Management System  |\n");
        printf("--------------------------------\n");
        printf("|    1. View Appointments      |\n");
        printf("|    2. Make Appointments      |\n");
        printf("|    3. Reschedule Appointment |\n");
        printf("|    4. Cancel Appointment     |\n");
        printf("|    5. View EHR               |\n");
        printf("|    6. Billing Details        |\n");
        printf("|    7. Insurance Claims       |\n");
        printf("|    8. Exit & Logout          |\n");
        printf("--------------------------------\n\n");
        printf("Enter the corresponding number for what you wish to do: ");

        if (scanf("%d", &choice) != 1) {
            // If the input is not an integer, clear the input buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 1 and 8.\n\n\n");
            getch();
            continue;
        }


        switch (choice) {
        case 1: viewAppointment(appointmentcounter, patientID, users);
            break;
        case 2:
            appointmentcounter = makeAppointment(appointmentcounter, DoctorCounter, patientID, Name);
            break;
        case 3:
            appointmentcounter = rescheduleAppointment(appointmentcounter,DoctorCounter, patientID, Name);
            break;
        case 4:
            cancelAppointment(appointmentcounter, patientID);
            break;
        case 5:
            viewEHR(patientID, EHRcounter, appointmentcounter, paymentcounter);
            break;
        case 6:
            BillingDetails(paymentcounter, patientID);
            break;
        case 7:
            InsuranceClaimsPage(ClaimCounter, patientID);
            break;
        case 8:
            exiting();
            break;
        };
    };
};


int LoginPage(users){
    FILE* fh_LoggingIn;
    fh_LoggingIn = fopen("Login.txt", "r");
    struct login *openLogin_array = malloc(users*sizeof(*openLogin_array));
    fread(&*openLogin_array, sizeof(struct login), users, fh_LoggingIn);
    fclose(fh_LoggingIn);

    char InputPatientID[10];
    char InputPassword [10];
    char patientID [10];
    char Name[50];

    int attempts = 0;
    bool authenticated = false;

    while (attempts<3){
        printf("Please Input PatientID: ");
        scanf("%s", &InputPatientID);
        // Convert the input to uppercase
        for (int i = 0; InputPatientID[i]; i++) {
            InputPatientID[i] = toupper(InputPatientID[i]);
        }
        printf("Please Input Password: ");
        scanf("%s", &InputPassword);
        for (int i = 0; i < users; i++) {
            if (strcmp(openLogin_array[i].patientID, InputPatientID) == 0 && strcmp(openLogin_array[i].password, InputPassword)==0){
                 strcpy(patientID, InputPatientID);
                 strcpy(Name, openLogin_array[i].Name);
                 authenticated = true;
                 mainmenu(patientID, users, Name);

                break;
            }

        }
        if (authenticated) {
            break;  // Exit the while loop
        } else {
            attempts++;
            printf("Invalid credentials. Attempt %d of 3.\n\n", attempts);
        }
    }

    if (!authenticated) {
        printf("You have exhausted your attempts!\nReturning Back to Main Menu.\n\n");
        getch();
        main();
    }



};

int RegisterPage(int users) {
    FILE* fh_LoggingIn;
    fh_LoggingIn = fopen("Login.txt", "r");
    if (fh_LoggingIn == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct login *openLogin_array = malloc(users * sizeof(struct login));
    if (openLogin_array == NULL) {
        perror("Memory allocation error");
        fclose(fh_LoggingIn);
        return 1;
    }

    fread(openLogin_array, sizeof(struct login), users, fh_LoggingIn);
    fclose(fh_LoggingIn);

    int maxID = 0;
    for (int i = 0; i < users; i++) {
        const char* idStr = openLogin_array[i].patientID;
        int id = atoi(idStr + 2); // Skip the first two characters ("PA", "pa", etc.)
        if (id > maxID) {
            maxID = id;
        }
    }

    char newPatientID[10];
    sprintf(newPatientID, "PA%02d", maxID + 1);  // Format the new patient ID

    // Create a new patient entry
    struct login newPatient;
    strcpy(newPatient.patientID, newPatientID);

    printf("\n\nPlease enter your Name: ");
    fgets(newPatient.Name, sizeof(newPatient.Name), stdin); // Read the entire line

    // Remove the newline character from the end of the name if it exists
    size_t len = strlen(newPatient.Name);
    if (len > 0 && newPatient.Name[len - 1] == '\n') {
        newPatient.Name[len - 1] = '\0';
    }

    printf("Please enter a password for your new account: ");
    scanf("%19s", newPatient.password);

    struct login* updatedDetails = malloc((users + 1) * sizeof(struct login));
    if (updatedDetails == NULL) {
        perror("Memory allocation error");
        free(openLogin_array);
        return 1;
    }

    // Copy existing user data to the new array
    for (int i = 0; i < users; i++) {
        updatedDetails[i] = openLogin_array[i];
    }

    // Add the new patient to the end of the array
    updatedDetails[users] = newPatient;

    users++;  // Increment the users count

    // Write updated data back to the file
    FILE* fh_RegisterLogin;
    fh_RegisterLogin = fopen("Login.txt", "w");
    if (fh_RegisterLogin == NULL) {
        perror("Error opening file for writing");
        free(openLogin_array);
        free(updatedDetails);
        return 1;
    }

    fwrite(updatedDetails, sizeof(struct login), users, fh_RegisterLogin);
    fclose(fh_RegisterLogin);

    // Print all patient details for confirmation
    printf("\n\nHello %s\n", updatedDetails[users - 1].Name);
    printf("Your Login Details Are:\n");
    printf("Patient ID: %s\nPassword: %s\n", updatedDetails[users - 1].patientID, updatedDetails[users - 1].password);

    getch();
    // Clean up: free allocated memory
    free(openLogin_array);
    free(updatedDetails);
    system("cls");

    return users;
};



int Initialise_Logins() {
    bool LoginFlag = false;
    int users = 0; // We will need this in later operations to define how long our arrays should be
    FILE* fh_Login;
    fh_Login = fopen("Login.txt", "r"); // Opens file

    if (fh_Login == NULL) { // If the file does not exist
        LoginFlag = true; // File does not exist, hence we will need to make a file
    } else {
        // Read the file to count the number of users
        struct login temp_login; // Temporary variable of type "struct login"
        while (fread(&temp_login, sizeof(struct login), 1, fh_Login) == 1) { // Reads the file in chunks of struct login
            users++; // Increments user count by 1 for each unique login
        }
        fclose(fh_Login); // Closes file after reading all logins
    }

    struct login* login_array = malloc(users * sizeof(struct login)); // Allocates memory for an array with "users" number of elements

    if (LoginFlag == true) { // If the file does not exist
        printf("Creating Login file\n");

        // Initialize default logins
        struct login default_logins[] = {
            {"PA01", "1234", "Anwar Jabib"},
            {"PA02", "8428", "Martin Brundle"},
            {"PA03", "1523", "Thomas Jefferson"},
            {"PA04", "0000", "James Maddison"},
            {"PA05", "4321", "Aaron Burr"},
            {"PA06", "9999", "Brandon Williams"},
            {"PA07", "3333", "Carlos Sainz"},
        };

        users = sizeof(default_logins) / sizeof(default_logins[0]); // Calculates number of logins
        login_array = realloc(login_array, users * sizeof(struct login)); // Reallocate memory for login_array to match number of default logins
        memcpy(login_array, default_logins, users * sizeof(struct login)); // Copies all elements from default logins array to login_array

        FILE* fh_LoginAdd;
        fh_LoginAdd = fopen("Login.txt", "w"); // Opens file in write mode
        fwrite(login_array, sizeof(struct login), users, fh_LoginAdd); // Writes logins from login_array to file
        fclose(fh_LoginAdd); // Closes file handler
        printf("Login.txt created!\n");
    } else { // If the file exists
        FILE* fh_FindingUsers;
        fh_FindingUsers = fopen("Login.txt", "r");
        fread(login_array, sizeof(struct login), users, fh_FindingUsers);
        fclose(fh_FindingUsers);
    }

    free(login_array); // Frees memory allocated for "login_array"
    return users; // Returns the user count since we will need it later for when we need to open the file in other functions
};


int PatientMain(){
    system("cls");
    int users = Initialise_Logins();



    bool repeat = true;
    char LoginOption[2];
    int option;
    while (repeat == true) {
        printf("------------------------\n");
        printf("|  Patient Login Page  |\n");
        printf("------------------------\n");
        printf("|     1. Login         |\n");
        printf("|     2. Register      |\n");
        printf("|     3. Exit          |\n");
        printf("------------------------\n");
        printf("\nEnter the corresponding number for what you wish to do: ");
        scanf("%s", &LoginOption);

        option = atoi(LoginOption);

        // Check if the input is within the valid range
        if (option >= 1 && option <= 2) {
            if (option == 1) {
                LoginPage(users);
            } else if (option == 2) {
                RegisterPage(users);
            } else if (option == 3) {
                printf("Return Home!"); // Put Badrs menu Here WAWAWWEAEAEAEAEAEAEAEAAEAEAEAEAEAEA - to get ur attention lol
            }
        } else {
            printf("Invalid Input! Please enter 1, 2 or 3.\n");
            getch();
            FirstMenu();
    }
    }
};
//////////////////////////
///Nurse Functions
///Hadi
///TP0
/////////////////////////
void write_inventory_to_file(const char *filename, struct inv *inventory, int numItems) //Function to write to file. Const ensures filename does not change
{
    FILE *fptr;
    fptr = fopen("inventory.txt", "w");
    if (fptr == NULL) //Check to see if the file exists
    {
        printf("Error opening file %s!\n", filename);
        return;
    }

    for (int i = 0; i < numItems; i++)
    {
        fprintf(fptr, "Item: %s, Quantity: %d\n", inventory[i].itemName, inventory[i].itemQuantity);
    }

    fclose(fptr);
    printf("Inventory has been intialized \n");
}

int read_inventory_from_file (const char *filename, struct inv **inventory) //Function to read from the file.
{
    FILE *fptr2;
    fptr2 = fopen("inventory.txt", "r");

    if (fptr2 == NULL)
    {
        printf("Error in reading inventory!");
        return -1;
    }

    int size = 10;
    int count = 0;
    *inventory = malloc(size * sizeof(struct inv)); //Dynamic memory allocation to read from the file, initially set to 10 items

    if (*inventory == NULL) //Error for if memory allocation fails
    {
        printf("Memory allocation error!\n");
        fclose(fptr2);
        return -1;
    }

    while (fscanf(fptr2, "Item: %49[^,], Quantity: %d\n", (*inventory)[count].itemName, &(*inventory)[count].itemQuantity) == 2) //Reads from the file. "Item: %49[^,], Quantity: %d\n" is the format, where the code will expect to find "Item" exactly as is is/ %49[^,] is used to read only 49 characters till a comma as the size of itemName is 50. (*inventory)[count].itemName, &(*inventory)[count].itemQuantity) will track the address of items read from the file and store them. ==2 ensures that 2 items were read, the item and quantity.
    {
        count++;
        if (count >= size)
        {
            size *= 2;
            struct inv *temp = realloc(*inventory, size * sizeof(struct inv)); //If the number of items goes above the memory limit , the dynamic memory will get doubled in size.
            if (temp == NULL) //Check if allocation was successful or not
            {
                printf("Memory reallocation error!\n");
                free(*inventory); //Frees allocated memory
                fclose(fptr2);
                return -1;
            }
            *inventory = temp; //If successful, the new amount is safely updated
        }
    }

    fclose(fptr2);
    return count; // Return the number of items read


}

int update_inventory_item(struct inv *inventory, int numItems, const char *itemName, int addAmount) //Function to update inventory
{
    for (int i = 0; i < numItems; i++)
    {
        if (strcmp(inventory[i].itemName, itemName) == 0)
        {
            inventory[i].itemQuantity += addAmount; // Add to existing quantity
            return 1; // Item found and updated
        }
    }
    return 0; // Item not found
}

void itemUpdate(struct inv *inventory, int numItems)// Function to allow user to update item name and quantity to add
 {
    char itemName[50];
    int addAmount;

    // Prompt for item name
    printf("Enter item name to update: ");
    scanf(" %[^\n]", itemName);

    // Prompt for amount to add
    printf("Enter amount to add to '%s': ", itemName);
    scanf("%d", &addAmount);

    // Updating the inventory with new values
    if (update_inventory_item(inventory, numItems, itemName, addAmount))
    {
        printf("Added %d to '%s'.\n", addAmount, itemName);
    }
    else
    {
        printf("Item '%s' not found in inventory.\n", itemName);
    }
}


int nurseMainMenu()
{
    int menuChoice;
    int menuChoice1;
    int menuChoice2;
    int menuChoice3;
    char doctorID[10];
    int prompt;
    int choice = 0;

    Initialise_DoctorScheduleFile();
    Initialise_TreatmentsFile();
    Initialise_AppointmentFile();


        printf("--------------------------------\n");
        printf("|       Staff Nurse System     |\n");
        printf("--------------------------------\n");
        printf("|    1. Doctor Scheduler       |\n");
        printf("|    2. Inventory Management   |\n");
        printf("|    3. Reports & Analytics    |\n");
        printf("|    4. Exit                   |\n");
        printf("--------------------------------\n\n");

        //Repeats until user selects correct option
        do
        {
            printf("Please select your choice (1 to 4): \n");
            scanf("%d", &menuChoice);

            if (menuChoice < 0 || menuChoice > 4)
            {
                printf("Invalid choice! Please try again.\n");
            }
        } while (menuChoice < 0 || menuChoice > 4);

        switch (menuChoice)
        {
            case 1:

                do
                {
                        printf("-------------------------------------\n");
                        printf("|       Doctor Scheduler            |\n");
                        printf("-------------------------------------\n");
                        printf("|    1. View All Appointments       |\n");
                        printf("|    2. View Upcoming Appointments  |\n");
                        printf("|    3. Manage Schedule             |\n");
                        printf("|    4. Exit                        |\n");
                        printf("-------------------------------------\n\n");

                        do
                        {
                        printf("Please select your choice (1 to 4): \n");
                        scanf("%d", &menuChoice1);

                            if (menuChoice1 < 0 || menuChoice1 > 4)
                            {
                                printf("Invalid choice! Please try again.\n");
                            }
                        } while (menuChoice1 < 0 || menuChoice1 > 4);



                        if (menuChoice1 == 1)
                            {
                                printf("Please enter a DoctorID (DC01 - DC05)\n");
                                scanf("%6s", doctorID);

                                prompt = 0;
                                viewAppointmentsByDoctorID(doctorID, 1);
                                printf("\nEnter any number to return to menu: ");
                                scanf("%d", &prompt);
                                break;
                            }
                        else if (menuChoice1 == 2)
                            {
                                printf("Please enter a DoctorID (DC01 - DC05)\n");
                                scanf("%6s", doctorID);

                                prompt = 0;
                                viewAppointmentsByDoctorID(doctorID, 0);
                                printf("\nEnter any number to return to menu: ");
                                scanf("%d", &prompt);
                                break;
                            }
                        else if (menuChoice1 == 3)
                            {
                                printf("Please enter a DoctorID (DC01 - DC05)\n");
                                scanf(" %6s", doctorID);

                                prompt = 0;
                                viewDoctorScheduleByDoctorID(doctorID);
                                printf("\nEnter any number to return to menu: ");
                                scanf("%d", &prompt);
                                break;
                            }
                        else if (menuChoice1 == 4)
                        {
                            break;
                        }

                }while (menuChoice2 !=4);


            case 2:
                do
                {
                        printf("---------------------------------------------------------\n");
                        printf("|                   Inventory Management                |\n");
                        printf("---------------------------------------------------------\n");
                        printf("|    1. Initialize Inventory (USE ONLY ON FIRST RUN)!   |\n");
                        printf("|    2. View Inventory                                  |\n");
                        printf("|    3. Purchase Inventory                              |\n");
                        printf("|    4. Exit                                            |\n");
                        printf("---------------------------------------------------------\n\n");

                        do
                        {
                        printf("Please select your choice (1 to 4): \n");
                        scanf("%d", &menuChoice2);

                        if (menuChoice2 < 0 || menuChoice2 > 4)
                        {
                            printf("Invalid choice! Please try again.\n");
                        }
                        } while (menuChoice2 < 0 || menuChoice2 > 4);

                        if (menuChoice2 == 1)//ONLY RUN ON FIRST USE, sets the default values for inventory
                        {
                            struct inv hospitalInventory[] =
                            {
                                { "Gauze", 3641},
                                { "Syringes", 1232},
                                { "Cotton Balls", 1256},
                                { "Alcohol Swabs", 2356},
                                { "Bandages", 4324},
                                { "Scalpels", 453},
                                { "Masks", 5234},
                                { "Gloves", 3894},
                                { "Scrubs", 362},
                                { "Insulin Pen", 37},
                                { "Panadol", 843},
                                { "Augmentin", 283},
                                { "Amoxicillin", 323},
                                { "Zithromax", 64},
                                { "Phenoxymethylpenicilin", 13},
                            };

                            int numItems = sizeof(hospitalInventory) / sizeof(hospitalInventory[0]); // Calculates the number of items in the hospitalInventory array
                            write_inventory_to_file("inventory.txt", hospitalInventory, numItems); //Function to write inventory to text file
                        }

                        else if (menuChoice2 == 2)//Reads inventory and displays it
                        {
                            struct inv *inventory = NULL;
                            int readItems = read_inventory_from_file("inventory.txt", &inventory);

                            if (readItems > 0) //Checks if data has been read
                            {
                                    printf("Inventory read from file:\n");
                                    for (int i = 0; i < readItems; i++)
                                    {
                                        printf("Item: %s, Quantity: %d\n", inventory[i].itemName, inventory[i].itemQuantity);
                                    }
                            }

                            free(inventory);
                        }

                        else if (menuChoice2 ==3)//Updates the values for the inventory
                        {
                            struct inv *inventory = NULL;
                            int numItems = read_inventory_from_file("inventory.txt", &inventory);
                            if (numItems > 0) //Checks if data has been read
                            {
                                    printf("Inventory read from file:\n");
                                    for (int i = 0; i < numItems; i++)
                                    {
                                        printf("Item: %s, Quantity: %d\n", inventory[i].itemName, inventory[i].itemQuantity);
                                    }
                            }

                            if (numItems > 0)
                            {
                                itemUpdate(inventory, numItems);
                                write_inventory_to_file("inventory.txt", inventory, numItems); // Write updated inventory back to file
                            }
                            else
                            {
                                printf("No items found in inventory.\n");
                            }

                            free(inventory); // Free allocated memory

                        }

                }while (menuChoice2 !=4);
                break;

            case 3:
                    printf("-------------------------------------\n");
                    printf("|        Reports and Analytics      |\n");
                    printf("-------------------------------------\n");
                    printf("|    1. View by PatientID           |\n");
                    printf("|    2. View by Illness             |\n");
                    printf("|    3. Exit                        |\n");
                    printf("-------------------------------------\n\n");

                    do
                    {
                        printf("Please select your choice (1 to 3): \n");
                        scanf("%d", &menuChoice3);

                            if (menuChoice3 < 0 || menuChoice3 > 3)
                            {
                                printf("Invalid choice! Please try again.\n");
                            }
                    } while (menuChoice3 < 0 || menuChoice3 > 3);

                    if (menuChoice3 == 1)
                            {
                                prompt = 0;
                                while(prompt == 0)
                                {
                                    char patientID[10]; // Buffer to store patient ID
                                    printf("Enter patient you want to search: ");
                                    scanf("%6s", patientID);
                                    viewTreatmentsByPatientID(patientID);
                                    printf("\nEnter 0 if you wish to search another patientID, enter 1 to return: ");
                                    if (scanf("%d", &prompt) != 1)
                                        {
                                        while (getchar() != '\n'); // Clear the input buffer
                                        printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
                                        choice = 0; // Reset choice to a neutral value that does not match the exit condition
                                        continue; // Skip the rest of the loop and prompt again
                                        }
                                }
                                    printf("ok");
                                    break;
                            }
                    else if (menuChoice3 == 2)
                        {
                            while(prompt == 0)
                            {
                                char condition[100]; // Buffer to store patient ID
                                printf("Enter condition you want to search: ");
                                scanf("%99s", condition);
                                viewTreatmentsByCondition(condition);
                                printf("\nEnter 0 if you wish to search another Condition, enter 1 to return: ");
                                if (scanf("%d", &prompt) != 1)
                                    {
                                        while (getchar() != '\n'); // Clear the input buffer
                                        printf("Invalid input. Please enter a number between 1 and 9.\n\n\n");
                                        choice = 0; // Reset choice to a neutral value that does not match the exit condition
                                        continue; // Skip the rest of the loop and prompt again
                                    }
                            }
                            printf("ok");
                            break;
                        }
                    else if (menuChoice3 == 3)
                    {
                        break;
                    }
            case 4:
                {
                    exit(0);
                }
        }
}


//////////////////////////
///Admin Functions
///Badr
///TP074644
/////////////////////////

// Initial appointment data
Appointment appointments[] = {
    {"2024-06-15", "09:30", "John Doe", "Dr. Smith", "Consultation", "Confirmed"},
    {"2024-06-18", "14:00", "Jane Smith", "Dr. Brown", "Check-up", "Confirmed"},
    {"2024-06-20", "11:15", "Michael Johnson", "Dr. Lee", "Procedure", "Cancelled"},
    {"2024-06-22", "10:00", "Emily Davis", "Dr. Wilson", "Follow-up", "Confirmed"},
    {"2024-06-25", "13:45", "David Miller", "Dr. Clark", "Consultation", "Confirmed"},
    {"2024-06-27", "15:30", "Sarah Wilson", "Dr. Martinez", "Check-up", "Confirmed"},
    {"2024-06-30", "08:45", "Jennifer Anderson", "Dr. Taylor", "Procedure", "Cancelled"},
    {"2024-07-03", "12:00", "Robert Moore", "Dr. White", "Consultation", "Confirmed"},
    {"2024-07-06", "16:20", "Lisa Garcia", "Dr. Allen", "Follow-up", "Confirmed"},
    {"2024-07-09", "09:00", "Daniel Wilson", "Dr. Harris", "Check-up", "Confirmed"}
};

// Define the structure for Medicine
typedef struct {
    char MedicineName[30];
    char BatchNumber[10];
    int Quantity;
    char ExpirationDate[11];
    char Supplier[30];
    char Status[15];
} Medicine;

// Initial inventory data
Medicine inventory[] = {
    {"Paracetamol", "B123456", 500, "2025-01-15", "ABC Pharma", "Available"},
    {"Amoxicillin", "A654321", 300, "2024-06-30", "XYZ Pharmaceuticals", "Available"},
    {"Ibuprofen", "I789012", 200, "2023-12-01", "HealthPlus", "Available"},
    {"Metformin", "M456789", 150, "2025-03-20", "MediSupply", "Available"},
    {"Atenolol", "A123987", 100, "2023-08-15", "LifeCare", "Expired"},
    {"Omeprazole", "O654123", 250, "2024-11-25", "Pharmaco", "Available"},
    {"Aspirin", "A321654", 0, "2023-09-10", "Wellness Inc.", "Out of Stock"},
    {"Losartan", "L987456", 180, "2025-05-05", "HealthFirst", "Available"},
    {"Insulin", "I123654", 50, "2023-07-01", "DiabeticsCare", "Expired"},
    {"Furosemide", "F789456", 75, "2024-10-10", "PharmaPlus", "Available"}
};

// Function prototypes for medicine inventory
int is_file_empty(FILE *file);
void initialize_inventory(const char *filename);
void write_medicineInventory(const char *filename);
void read_MedicineInventory(const char *filename);
void MedicineRemover(const char *filename);
void delete_file(const char *filename);
void edit_medicineInventory(const char *filename);
void search_medicine_by_name(const char *filename, const char *medicineName);
char *strcasestr_custom(const char *haystack, const char *needle);

// Function to check if the file is empty
int is_file_empty(FILE *file)
{
    fseek(file, 0, SEEK_END);  // Move the file pointer to the end of the file
    long size = ftell(file);   // Get the size of the file
    fseek(file, 0, SEEK_SET);  // Move the file pointer to the start of the file
    return size == 0;
}

// Function to initialize the file with the inventory if it's empty
void initialize_inventory(const char *filename)
{
    FILE *pF = fopen(filename, "a+");

    if (pF == NULL)
    {
        printf("Unable to open the file!\n");
        return;
    }

    if (is_file_empty(pF))
    {
        // Write the header to the file
        fprintf(pF, "            ==============={{ Management Medicine Inventory System }}==================                  \n");
        fprintf(pF, "+---------------+--------------+----------+------------------+------------------------+-----------------+\n");
        fprintf(pF, "| Medicine Name | Batch Number | Quantity | Expiration Date  |        Supplier        |      Status     |\n");
        fprintf(pF, "+---------------+--------------+----------+------------------+------------------------+-----------------+\n");

        // Write the initial inventory to the file
        for (int i = 0; i < (int)(sizeof(inventory) / sizeof(inventory[0])); i++)
        {
            fprintf(pF, "| %-13s | %-12s | %-8d | %-16s | %-22s | %-15s |\n",
                inventory[i].MedicineName, inventory[i].BatchNumber, inventory[i].Quantity,
                inventory[i].ExpirationDate, inventory[i].Supplier, inventory[i].Status);
        }
        fprintf(pF, "+---------------+--------------+----------+------------------+------------------------+-----------------+\n");
    }

    fclose(pF);
}


// Function to handle case sensitive matters
char *strcasestr_custom(const char *haystack, const char *needle) {
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return (char *)haystack;
    }
    while (*haystack) {
        if (strncasecmp(haystack, needle, needle_len) == 0) {
            return (char *)haystack;
        }
        haystack++;
    }
    return NULL;
}

// Function to write new entries to the file
void write_medicineInventory(const char *filename)
{
    FILE *pF = fopen(filename, "a+");

    if (pF == NULL)
    {
        printf("Unable to open the file!\n");
        return;
    }

    char MedicineName[30], BatchNumber[10], ExpirationDate[11], Supplier[30], Status[15];
    int Quantity;
    int numMedicine, i;

    printf("Enter the number of Medicines: ");
    while (scanf("%d", &numMedicine) != 1 || numMedicine <= 0)
    {
        printf("Invalid input. Please enter a positive integer: ");
        while (getchar() != '\n');  // clear the input buffer
    }

    for (i = 0; i < numMedicine; i++)
    {
        printf("\nEnter the details for Medicine %d:\n", i + 1);

        printf("Medicine name: ");
        scanf("%s", MedicineName);

        printf("Batch number: ");
        scanf("%s", BatchNumber);

        printf("Quantity: ");
        while (scanf("%d", &Quantity) != 1 || Quantity < 0)
        {
            printf("Invalid input. Please enter a non-negative integer: ");
            while (getchar() != '\n');  // clear the input buffer
        }

        printf("Expiration Date (YYYY-MM-DD): ");
        scanf("%s", ExpirationDate);

        printf("Supplier: ");
        scanf(" %[^\n]%*c", Supplier);  // to read a string with spaces

        printf("Status: ");
        scanf(" %[^\n]%*c", Status);  // to read a string with spaces

        // Write the medicine details to the file
        fseek(pF, 0, SEEK_END);  // Move file pointer to the end
        fprintf(pF, "| %-13s | %-12s | %-8d | %-16s | %-22s | %-15s |\n",
            MedicineName, BatchNumber, Quantity, ExpirationDate, Supplier, Status);
    }

    // Add the ending line
    fprintf(pF, "+---------------+--------------+----------+------------------+------------------------+-----------------+\n");

    fclose(pF);
    printf("Medicines successfully saved to %s\n", filename);
}

// Function to read inventory from file
void read_MedicineInventory(const char *filename)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        printf("%s", buffer);
    }
    fclose(pF);
}

// Function to delete a medicine record
void MedicineRemover(const char *filename)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open the file!\n");
        return;
    }

    char buffer[255];
    char tempFilename[] = "temp.txt";
    FILE *tempF = fopen(tempFilename, "w");
    if (tempF == NULL)
    {
        printf("Unable to open temporary file!\n");
        fclose(pF);
        return;
    }

    char targetedMedicine[30];
    printf("Enter the medicine name you want to remove from the inventory: ");
    scanf("%s", targetedMedicine);

    int found = 0;
    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        if (strstr(buffer, targetedMedicine) == NULL || strstr(buffer, "+---------------+") != NULL)
        {
            fputs(buffer, tempF);
        }
        else
        {
            found = 1;
        }
    }

    fclose(pF);
    fclose(tempF);

    remove(filename);
    rename(tempFilename, filename);

    if (found)
    {
        printf("Medicine '%s' has been removed from the inventory.\n", targetedMedicine);
    }
    else
    {
        printf("Medicine '%s' not found in the inventory.\n", targetedMedicine);
    }
}

// Function to edit the medicine with a selective method
void edit_medicineInventory(const char *filename)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    char tempFilename[] = "temp.txt";
    FILE *tempF = fopen(tempFilename, "w");
    if (tempF == NULL)
    {
        printf("Unable to open temporary file!\n");
        fclose(pF);
        return;
    }

    char buffer[255];
    char searchName[30];
    char newMedicineName[30] = "", newBatchNumber[10] = "", newExpirationDate[11] = "", newSupplier[30] = "", newStatus[15] = "";
    int newQuantity = -1;
    int found = 0;

    printf("Enter the medicine name to edit: ");
    scanf(" %[^\n]%*c", searchName); // to read a string with spaces

    // Print the header lines
    fprintf(tempF, "            ==============={{ Management Medicine Inventory System }}==================                  \n");
    fprintf(tempF, "+---------------+--------------+----------+------------------+------------------------+-----------------+\n");
    fprintf(tempF, "| Medicine Name | Batch Number | Quantity | Expiration Date  |        Supplier        |      Status     |\n");
    fprintf(tempF, "+---------------+--------------+----------+------------------+------------------------+-----------------+\n");

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        // Check for the medicine name in a case-insensitive manner
        if (strcasestr_custom(buffer, searchName) != NULL && !found)
        {
            found = 1;

            // Preserve old values if not updated
            char currentMedicineName[30], currentBatchNumber[10], currentExpirationDate[11], currentSupplier[30], currentStatus[15];
            int currentQuantity;

            sscanf(buffer, "| %29[^|] | %9[^|] | %d | %10[^|] | %29[^|] | %14[^|] |",
                   currentMedicineName, currentBatchNumber, &currentQuantity, currentExpirationDate, currentSupplier, currentStatus);

            int choice;
            printf("Choose the fields you want to edit:\n");
            printf("1. Medicine name\n");
            printf("2. Batch number\n");
            printf("3. Quantity\n");
            printf("4. Expiration Date\n");
            printf("5. Supplier\n");
            printf("6. Status\n");
            printf("Enter your choice (0 to finish): ");
            while (scanf("%d", &choice) && choice != 0)
            {
                switch (choice)
                {
                    case 1:
                        printf("New Medicine name: ");
                        scanf(" %[^\n]%*c", newMedicineName); // to read a string with spaces
                        break;
                    case 2:
                        printf("New Batch number: ");
                        scanf("%s", newBatchNumber);
                        break;
                    case 3:
                        printf("New Quantity: ");
                        while (scanf("%d", &newQuantity) != 1 || newQuantity < 0)
                        {
                            printf("Invalid input. Please enter a non-negative integer: ");
                            while (getchar() != '\n');  // clear the input buffer
                        }
                        break;
                    case 4:
                        printf("New Expiration Date (YYYY-MM-DD): ");
                        scanf("%s", newExpirationDate);
                        break;
                    case 5:
                        printf("New Supplier: ");
                        scanf(" %[^\n]%*c", newSupplier);  // to read a string with spaces
                        break;
                    case 6:
                        printf("New Status: ");
                        scanf("%s", newStatus);
                        break;
                    default:
                        printf("Invalid choice! Please choose a number between 1 and 6.\n");
                        break;
                }
                printf("Enter your choice (0 to finish): ");
            }

            // Preserve old values if not updated
            if (strlen(newMedicineName) == 0) strcpy(newMedicineName, currentMedicineName);
            if (strlen(newBatchNumber) == 0) strcpy(newBatchNumber, currentBatchNumber);
            if (newQuantity == -1) newQuantity = currentQuantity;
            if (strlen(newExpirationDate) == 0) strcpy(newExpirationDate, currentExpirationDate);
            if (strlen(newSupplier) == 0) strcpy(newSupplier, currentSupplier);
            if (strlen(newStatus) == 0) strcpy(newStatus, currentStatus);

            // Write the updated medicine details to the temporary file while maintaining the original formatting
            fprintf(tempF, "| %-13s | %-12s | %-8d | %-16s | %-22s | %-15s |\n",
                    newMedicineName, newBatchNumber, newQuantity, newExpirationDate, newSupplier, newStatus);
        }
        else
        {
            fputs(buffer, tempF);
        }
    }

    fclose(pF);
    fclose(tempF);

    if (found)
    {
        remove(filename);
        rename(tempFilename, filename);
        printf("Medicine '%s' has been updated.\n", searchName);
    }
    else
    {
        remove(tempFilename);
        printf("No medicine found with the name: %s\n", searchName);
    }
}

// Function to search for a medicine by name
void search_medicine_by_name(const char *filename, const char *medicineName)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    char buffer[255];
    int found = 0;

    // Print the header lines
    printf("            ==============={{ Management Medicine Inventory System }}==================                  \n");
    printf("+---------------+--------------+----------+------------------+------------------------+-----------------+\n");
    printf("| Medicine Name | Batch Number | Quantity | Expiration Date  |        Supplier        |      Status     |\n");
    printf("+---------------+--------------+----------+------------------+------------------------+-----------------+\n");

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        // Use strstr to find the medicine name case-insensitively
        if (strcasestr_custom(buffer, medicineName) != NULL)
        {
            found = 1;
            printf("%s", buffer);
        }
    }

    fclose(pF);

    if (!found)
    {
        printf("Medicine '%s' not found in the inventory.\n", medicineName);
    }
}

// Function to delete the file
void delete_file(const char *filename)
{
    char confirm;
    printf("Are you sure you want to delete the entire file? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y')
    {
        if (remove(filename) == 0)
        {
            printf("File deleted successfully.\n");
        }
        else
        {
            printf("Unable to delete the file.\n");
        }
    }
    else
    {
        printf("File deletion cancelled.\n");
    }
}

// Function to manage the inventory system
void manage_inventory_system(const char *filename) {
    int choice;

    do {
        printf("\n=========={{ Management Inventory System }}==========\n");
        printf("\nMedicine Management Menu:\n");
        printf("1. Add new medicines\n");
        printf("2. View all medicines\n");
        printf("3. Remove a medicine\n");
        printf("4. Edit a medicine\n");
        printf("5. Delete inventory file\n");
        printf("6. Search for a medicine\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                write_medicineInventory(filename);
                break;
            case 2:
                read_MedicineInventory(filename);
                break;
            case 3:
                MedicineRemover(filename);
                break;
            case 4:
                edit_medicineInventory(filename);
                break;
            case 5:
                delete_file(filename);
                break;
            case 6: {
                char searchName[50];
                printf("Enter the medicine name to search: ");
                scanf(" %[^\n]%*c", searchName);
                search_medicine_by_name(filename, searchName);
                break;
            }
            case 7:
                printf("Logged out successfully!. See you next time >w<\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 7.\n");
        }

    } while (choice != 7);
}

// Global arrays of credentials for each role
Credentials admins[MAX_CREDENTIALS] = {
    {"Badr", "12345", "ADMIN"},
    {"Hadi", "12345", "ADMIN"},
    {"Admin", "Admin", "ADMIN"}
};
int num_admins = 3;

Credentials doctors[MAX_CREDENTIALS] = {
    {"DC01", "12345", "DOCTOR"},
    {"DC02", "passwordB", "DOCTOR"},
    {"DC03", "passwordC", "DOCTOR"},
    {"DC04", "passwordC", "DOCTOR"},
    {"DC05", "passwordC", "DOCTOR"}
};
int num_doctors = 3;

Credentials nurses[MAX_CREDENTIALS] = {
    {"Abdi", "12345", "NURSE"},
    {"Nurse", "12345", "NURSE"},
    {"nurse", "12345", "NURSE"}
};
int num_nurses = 3;

Credentials patients[MAX_CREDENTIALS] = {
    {"Alfie", "12345", "PATIENT"},
    {"Patient", "12345", "PATIENT"},
    {"patient", "12345", "PATIENT"}
};
int num_patients = 3;



// Function prototypes for appointment trends
void initialize_appointment_trends(const char *filename);
void write_appointment_trends(const char *filename);
void read_appointment_trends(const char *filename);
void search_appointment_by_patient(const char *filename, const char *patientName);
void delete_appointment(const char *filename);











// Function to initialize the file with appointment trends if it's empty
void initialize_appointment_trends(const char *filename)
{
    FILE *pF = fopen(filename, "a+");
    if (pF == NULL)
    {
        printf("Unable to open the file!\n");
        return;
    }

    // Check if file is empty
    if (is_file_empty(pF))
    {
        // Write the header to the file
        fprintf(pF, "            ==============={{ Appointment Management System }}==================                  \n");
        fprintf(pF, "+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");
        fprintf(pF, "|    Date     | Time   |     Patient Name     |        Doctor        |  Appointment Type    |      Status      |\n");
        fprintf(pF, "+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");

        // Write the initial appointments to the file
        for (size_t i = 0; i < sizeof(appointments) / sizeof(appointments[0]); i++)
        {
            fprintf(pF, "| %-11s | %-6s | %-20s | %-20s | %-20s | %-16s |\n",
                    appointments[i].Date, appointments[i].Time, appointments[i].PatientName,
                    appointments[i].Doctor, appointments[i].AppointmentType, appointments[i].Status);
        }
        fprintf(pF, "+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");
    }

    fclose(pF);
}






// Function to write new appointment entries to the file
void write_appointment_trends(const char *filename)
{
    FILE *pF = fopen(filename, "a+");
    if (pF == NULL)
    {
        printf("Unable to open the file!\n");
        return;
    }

    char Date[11], Time[6], PatientName[50], Doctor[50], AppointmentType[50], Status[20];
    int numAppointments;

    printf("Enter the number of Appointments: ");
    while (scanf("%d", &numAppointments) != 1 || numAppointments <= 0)
    {
        printf("Invalid input. Please enter a positive integer: ");
        while (getchar() != '\n');  // clear the input buffer
    }

    for (int i = 0; i < numAppointments; i++)
    {
        printf("\nEnter the details for Appointment %d:\n", i + 1);

        printf("Date (YYYY-MM-DD): ");
        scanf("%s", Date);

        printf("Time (HH:MM): ");
        scanf("%s", Time);

        printf("Patient Name: ");
        scanf(" %[^\n]%*c", PatientName);  // to read a string with spaces

        printf("Doctor: ");
        scanf(" %[^\n]%*c", Doctor);  // to read a string with spaces

        printf("Appointment Type: ");
        scanf(" %[^\n]%*c", AppointmentType);  // to read a string with spaces

        printf("Status: ");
        scanf(" %[^\n]%*c", Status);  // to read a string with spaces

        // Write the appointment details to the file
        fseek(pF, 0, SEEK_END);  // Move file pointer to the end
        fprintf(pF, "| %-11s | %-6s | %-20s | %-20s | %-20s | %-16s |\n",
                Date, Time, PatientName, Doctor, AppointmentType, Status);
    }

    // Add the ending line
    fprintf(pF, "+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");

    fclose(pF);
    printf("Appointments successfully saved to %s\n", filename);
}

// Function to read appointment trends from file
void read_appointment_trends(const char *filename)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(pF);
}






// Function to search for appointments by patient name
void search_appointment_by_patient(const char *filename, const char *patientName)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    char buffer[255];
    int found = 0;

    // Print the header lines
    printf("               ==============={{ Appointment Trends }}==================                  \n");
    printf("+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");
    printf("|    Date     | Time   |     Patient Name     |        Doctor        |  Appointment Type    |      Status      |\n");
    printf("+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        // Use strstr to find the patient name case-insensitively
        if (strstr(buffer, patientName) != NULL)
        {
            found = 1;
            printf("%s", buffer);
        }
    }

    fclose(pF);

    if (!found)
    {
        printf("No appointments found for patient '%s'\n", patientName);
    }
}



// Function to edit appointment records
void edit_appointments(const char *filename)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    char tempFilename[] = "temp.txt";
    FILE *tempF = fopen(tempFilename, "w");
    if (tempF == NULL)
    {
        printf("Unable to open temporary file!\n");
        fclose(pF);
        return;
    }

    char buffer[255];
    char searchName[50];
    char newDate[11] = "", newTime[6] = "", newPatientName[50] = "", newDoctor[50] = "", newAppointmentType[50] = "", newStatus[20] = "";
    int found = 0;

    printf("Enter the patient name to edit appointment: ");
    scanf(" %[^\n]%*c", searchName); // Read patient name with spaces

    // Print the header lines to the temporary file
    fprintf(tempF, "            ==============={{ Appointment Management System }}==================                  \n");
    fprintf(tempF, "+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");
    fprintf(tempF, "|    Date     | Time   |     Patient Name     |        Doctor        |  Appointment Type    |      Status      |\n");
    fprintf(tempF, "+-------------+--------+----------------------+----------------------+----------------------+------------------+\n");

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        // Check for the patient name in a case-insensitive manner
        if (strcasestr_custom(buffer, searchName) != 0 && !found)
        {
            found = 1;

            // Preserve old values if not updated
            char currentDate[11], currentTime[6], currentPatientName[50], currentDoctor[50], currentAppointmentType[50], currentStatus[20];

            sscanf(buffer, "| %10[^|] | %5[^|] | %49[^|] | %49[^|] | %49[^|] | %19[^|] |",
                   currentDate, currentTime, currentPatientName, currentDoctor, currentAppointmentType, currentStatus);

            int choice;
            printf("\nChoose the fields you want to edit:\n");
            printf("1. Date\n");
            printf("2. Time\n");
            printf("3. Patient Name\n");
            printf("4. Doctor\n");
            printf("5. Appointment Type\n");
            printf("6. Status\n");
            printf("Enter your choice (0 to finish): ");
            while (scanf("%d", &choice) && choice != 0)
            {
                switch (choice)
                {
                    case 1:
                        printf("New Date (YYYY-MM-DD): ");
                        scanf("%s", newDate);
                        break;
                    case 2:
                        printf("New Time (HH:MM): ");
                        scanf("%s", newTime);
                        break;
                    case 3:
                        printf("New Patient Name: ");
                        scanf(" %[^\n]%*c", newPatientName);
                        break;
                    case 4:
                        printf("New Doctor: ");
                        scanf(" %[^\n]%*c", newDoctor);
                        break;
                    case 5:
                        printf("New Appointment Type: ");
                        scanf(" %[^\n]%*c", newAppointmentType);
                        break;
                    case 6:
                        printf("New Status: ");
                        scanf(" %[^\n]%*c", newStatus);
                        break;
                    default:
                        printf("Invalid choice! Please choose a number between 1 and 6.\n");
                        break;
                }
                printf("Enter your choice (0 to finish): ");
            }

            // Preserve old values if not updated
            if (strlen(newDate) == 0) strcpy(newDate, currentDate);
            if (strlen(newTime) == 0) strcpy(newTime, currentTime);
            if (strlen(newPatientName) == 0) strcpy(newPatientName, currentPatientName);
            if (strlen(newDoctor) == 0) strcpy(newDoctor, currentDoctor);
            if (strlen(newAppointmentType) == 0) strcpy(newAppointmentType, currentAppointmentType);
            if (strlen(newStatus) == 0) strcpy(newStatus, currentStatus);

            // Write the updated appointment details to the temporary file while maintaining the original formatting
            fprintf(tempF, "| %-11s | %-6s | %-20s | %-20s | %-20s | %-16s |\n",
                    newDate, newTime, newPatientName, newDoctor, newAppointmentType, newStatus);
        }
        else
        {
            fputs(buffer, tempF);
        }
    }

    fclose(pF);
    fclose(tempF);

    if (found)
    {
        remove(filename);
        rename(tempFilename, filename);
        printf("Appointment for '%s' has been updated.\n", searchName);
    }
    else
    {
        remove(tempFilename);
        printf("No appointment found for patient: %s\n", searchName);
    }
}








// Function to delete appointment records
void delete_appointment(const char *filename)
{
    FILE *pF = fopen(filename, "r");
    if (pF == NULL)
    {
        printf("Unable to open the file!\n");
        return;
    }

    char buffer[255];
    char tempFilename[] = "temp.txt";
    FILE *tempF = fopen(tempFilename, "w");
    if (tempF == NULL)
    {
        printf("Unable to open temporary file!\n");
        fclose(pF);
        return;
    }

    char targetedPatient[50];
    printf("Enter the patient name to remove appointments for: ");
    scanf(" %[^\n]%*c", targetedPatient);

    int found = 0;
    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        if (strstr(buffer, targetedPatient) == NULL || strstr(buffer, "+-------------+") != NULL)
        {
            fputs(buffer, tempF);
        }
        else
        {
            found = 1;
        }
    }

    fclose(pF);
    fclose(tempF);

    remove(filename);
    rename(tempFilename, filename);

    if (found)
    {
        printf("Appointments for patient '%s' have been removed.\n", targetedPatient);
    }
    else
    {
        printf("No appointments found for patient '%s'\n", targetedPatient);
    }
}


int admin_authenticated = 0; // Global variable to track admin authentication status
int is_authenticated = 0;    // Global variable to track general authentication status

// Function prototypes
void initializeRoles();
void initializeCredentials(const char *filename, Credentials *credentials, int *num_credentials, const char *role_header, Credentials initial_credentials[], int num_initial_credentials);
void initializeAdmins();
void initializeDoctors();
void initializeNurses();
void initializePatients();
int authenticate(Credentials credentials[], int num_credentials, const char *username, const char *password);
void displayMainMenu();
void login();
void registerAuthorityUser();
void registerPatient();
void registerUser(Credentials credentials[], int *num_credentials, const char *role, const char *filename);
void displayRoleMenu(const char *role, const char *username);
void removeNewlineChar(char *str);
int validateChoiceInput(const char *input, int min, int max);
int validateUsername(const char *username);
int validatePassword(const char *password);
void handleInvalidInput();


// Utility Functions
void removeNewlineChar(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int validateChoiceInput(const char *input, int min, int max) {
    for (int i = 0; input[i]; i++) {
        if (!isdigit((unsigned char)input[i])) {
            return 0;
        }
    }
    int choice = atoi(input);
    return choice >= min && choice <= max;
}

int validateUsername(const char *username) {
    if (strlen(username) == 0) {
        printf("Username cannot be empty.\n");
        return 0; // Invalid if the username is empty
    }
    for (int i = 0; username[i]; i++) {
        if (!isalnum((unsigned char)username[i])) {
            printf("Username can only contain letters and digits.\n");
            return 0; // Invalid if any non-alphanumeric character is found
        }
    }
    return 1; // Valid username
}

int validatePassword(const char *password) {
    if (strlen(password) < 5) {
        printf("Password must be at least 5 characters long.\n");
        return 0; // Invalid password
    }
    return 1; // Valid password
}

void handleInvalidInput() {
    printf("Invalid input. Please try again.\n");
}

// Initialization Functions
void initializeRoles() {
    initializeCredentials("admins.txt", admins, &num_admins, "Admin", admins, num_admins);
    initializeCredentials("doctors.txt", doctors, &num_doctors, "Doctor", doctors, num_doctors);
    initializeCredentials("nurses.txt", nurses, &num_nurses, "Nurse", nurses, num_nurses);
    initializeCredentials("patients.txt", patients, &num_patients, "Patient", patients, num_patients);
}

void initializeCredentials(const char *filename, Credentials *credentials, int *num_credentials, const char *role_header, Credentials initial_credentials[], int num_initial_credentials) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        file = fopen(filename, "w+");
        if (file == NULL) {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }

        fprintf(file, "          ========Credentials %s========\n", role_header);
        fprintf(file, "+----------------+----------+\n");
        fprintf(file, "| Username       | Password |\n");
        fprintf(file, "+----------------+----------+\n");

        for (int i = 0; i < num_initial_credentials; i++) {
            fprintf(file, "| %-14s | %-8s |\n", initial_credentials[i].username, initial_credentials[i].password);
            credentials[i] = initial_credentials[i];
        }
        fprintf(file, "+----------------+----------+\n");
        *num_credentials = num_initial_credentials;

        fclose(file);
    } else {
        char line[150];
        while (fgets(line, sizeof(line), file) != NULL) {
            if (line[0] == '|') {
                sscanf(line, "| %49s | %49s |", credentials[*num_credentials].username, credentials[*num_credentials].password);
                (*num_credentials)++;
            }
        }
        fclose(file);
    }
}

// Authentication and Registration Functions
int authenticate(Credentials credentials[], int num_credentials, const char *username, const char *password) {
    for (int i = 0; i < num_credentials; i++) {
        if (strcmp(credentials[i].username, username) == 0 && strcmp(credentials[i].password, password) == 0) {
            return 1; // Authentication successful
        }
    }
    return 0; // Authentication failed
}

void login() {
    char role[10];
    printf("Enter role to login (Admin, Doctor, Nurse): ");
    fgets(role, sizeof(role), stdin);
    removeNewlineChar(role);

    for (int i = 0; role[i]; i++) {
        role[i] = toupper((unsigned char)role[i]);
    }

    if (strcmp(role, "ADMIN") != 0 && strcmp(role, "DOCTOR") != 0 && strcmp(role, "NURSE") != 0 && strcmp(role, "PATIENT") != 0) {
        handleInvalidInput();
        return;
    }

    char input_username[50];
    char input_password[50];
    int max_attempts = 3;
    int authenticated = 0;

    do {
        printf("Please enter your User Name: ");
        fgets(input_username, sizeof(input_username), stdin);
        removeNewlineChar(input_username);

        if (!validateUsername(input_username)) {
            handleInvalidInput();
            continue;
        }

        printf("Please enter your Password: ");
        fgets(input_password, sizeof(input_password), stdin);
        removeNewlineChar(input_password);

        if (!validatePassword(input_password)) {
            handleInvalidInput();
            continue;
        }

        // Authenticate based on role
        if (strcmp(role, "ADMIN") == 0) {
            authenticated = authenticate(admins, num_admins, input_username, input_password);
            if (authenticated) {
                admin_authenticated = 1; // Set admin_authenticated to true
            }
        } else if (strcmp(role, "DOCTOR") == 0) {
            authenticated = authenticate(doctors, num_doctors, input_username, input_password);
        } else if (strcmp(role, "NURSE") == 0) {
            authenticated = authenticate(nurses, num_nurses, input_username, input_password);
        } else if (strcmp(role, "PATIENT") == 0) {
            authenticated = authenticate(patients, num_patients, input_username, input_password);
        }

        if (!authenticated) {
            printf("Authentication failed. Please try again.\n");
            max_attempts--;
            if (max_attempts == 0) {
                printf("Maximum login attempts exceeded.\n");
                return;
            }
        }

    } while (!authenticated);

    printf("Welcome, %s! You are now logged in as %s.\n", input_username, role);
    displayRoleMenu(role, input_username);
}

void registerAuthorityUser() {
    if (!admin_authenticated) {
        printf("Only authenticated admins can register new users.\n");
        return;
    }

    char role[10];
    printf("Enter role to register (Doctor, Nurse): ");
    fgets(role, sizeof(role), stdin);
    removeNewlineChar(role);

    for (int i = 0; role[i]; i++) {
        role[i] = toupper((unsigned char)role[i]);
    }

    if (strcmp(role, "DOCTOR") == 0) {
        registerUser(doctors, &num_doctors, "Doctor", "doctors.txt");
    } else if (strcmp(role, "NURSE") == 0) {
        registerUser(nurses, &num_nurses, "Nurse", "nurses.txt");
    } else {
        handleInvalidInput();
    }
}

void registerUser(Credentials credentials[], int *num_credentials, const char *role, const char *filename) {
    if (*num_credentials >= MAX_CREDENTIALS) {
        printf("Maximum number of %ss reached. Cannot register more users.\n", role);
        return;
    }

    char new_username[50];
    char new_password[50];

    printf("Enter new %s username: ", role);
    fgets(new_username, sizeof(new_username), stdin);
    removeNewlineChar(new_username);

    if (!validateUsername(new_username)) {
        handleInvalidInput();
        return;
    }

    printf("Enter new %s password: ", role);
    fgets(new_password, sizeof(new_password), stdin);
    removeNewlineChar(new_password);

    if (!validatePassword(new_password)) {
        handleInvalidInput();
        return;
    }

    // Add new user to credentials array
    strcpy(credentials[*num_credentials].username, new_username);
    strcpy(credentials[*num_credentials].password, new_password);
    strcpy(credentials[*num_credentials].role, role);
    (*num_credentials)++;

    // Append new user to file
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "| %-14s | %-8s |\n", new_username, new_password);
    fclose(file);

    printf("%s registered successfully!\n", role);
}

void registerPatient() {
    registerUser(patients, &num_patients, "Patient", "patients.txt");
}




                 ////////   Menu handling  Functions   /////////

// Function to display main menu
void displayMainMenu() {
    char choice[10];
    while (1) {
        printf("\n|----------------------------|\n");
        printf("|    ===={Main Menu}====     |\n");
        printf("|----------------------------|\n");
        printf("|1- Login.                   |\n");
        printf("|2- Logout.                  |\n");
        printf("|----------------------------|\n");
        printf("\n");
        printf("|Enter your choice:\n");

        fgets(choice, sizeof(choice), stdin);
        removeNewlineChar(choice);

        if (!validateChoiceInput(choice, 1, 2)) {
            handleInvalidInput();
            continue;
        }

        int choiceInt = atoi(choice);

        switch (choiceInt) {
            case 1:
                login();
                break;

            case 2:
                printf("Logged out\n");
                return; // Break out of the loop and function

            default:
                handleInvalidInput();
        }
    }
}


// Display Role Menus function
void displayRoleMenu(const char *role, const char *username) {
    char choice[10];

    if (strcmp(role, "ADMIN") == 0) {
        while (1) {
            printf("\n|----------------------------|\n");
            printf("|Administration Menu:        |\n");
            printf("|----------------------------|\n");
            printf("|1- Register Authority User. |\n");
            printf("|2- Appointment trends.      |\n");
            printf("|3- Register Patient.        |\n");
            printf("|4- Inventory Management.    |\n");
            printf("|5- Logout.                  |\n");
            printf("|----------------------------|\n");
            printf("\n");
            printf("|Enter your choice:\n");

            fgets(choice, sizeof(choice), stdin);
            removeNewlineChar(choice);

            int choiceInt = atoi(choice); // Directly convert choice to integer without validation

            switch (choiceInt) {
                case 1:
                    registerAuthorityUser();
                    break;

                case 2:{
                    const char *appointmentFile = "appointment.txt";
                    initialize_appointment_trends(appointmentFile);

                    int subChoice;

                    do {
                        printf("\n=========={{ Appointment Trends Management System }}==========\n");
                        printf("\nAppointment Trends Management Menu:\n");
                        printf("1. Add new appointments.\n");
                        printf("2. View all appointments.\n");
                        printf("3. Remove appointments by patient.\n");
                        printf("4. Search appointments by patient.\n");
                        printf("5. Change appointments.\n");
                        printf("6. Delete the appointments File.\n");
                        printf("7. Exit\n");
                        printf("Enter your choice: ");
                        scanf("%d", &subChoice);
                        getchar(); // Consume the newline character left by scanf

                        switch (subChoice) {
                            case 1:
                                write_appointment_trends(appointmentFile);
                                break;
                            case 2:
                                read_appointment_trends(appointmentFile);
                                break;
                            case 3:
                                delete_appointment(appointmentFile);
                                break;
                            case 4: {
                                char searchPatient[50];
                                printf("Enter the patient name to search: ");
                                scanf(" %[^\n]%*c", searchPatient);
                                search_appointment_by_patient(appointmentFile, searchPatient);
                                break;
                            }
                            case 5:
                                edit_appointments(appointmentFile);
                                break;
                            case 6:
                                delete_file(appointmentFile);
                                break;
                            case 7:
                                printf("Logged out\n");
                                break;
                            default:
                                handleInvalidInput();
                                printf("Invalid choice! Please enter a number between 1 and 7.\n");
                        }

                    } while (subChoice != 7);
                    break;
                }
                case 3:
                    registerPatient();
                    break;

                case 4: {
                    const char *inventoryFile = "inventory.txt";
                    initialize_inventory(inventoryFile);

                    int subChoice;

                    do {
                        printf("\n=========={{ Inventory Management System }}==========\n");
                        printf("Inventory Management Menu:\n");
                        printf("1. Add new medicine.\n");
                        printf("2. View all medicine.\n");
                        printf("3. Remove medicine.\n");
                        printf("4. Edit medicine.\n");
                        printf("5. Search medicine by name.\n");
                        printf("6. Delete medicine file.\n");
                        printf("7. Exit to Admin Menu\n");
                        printf("Enter your choice: ");

                        scanf("%d", &subChoice);
                        getchar(); // Consume the newline character left by scanf

                        switch (subChoice) {
                            case 1:
                                write_medicineInventory(inventoryFile);
                                break;
                            case 2:
                                read_MedicineInventory(inventoryFile);
                                break;
                            case 3:
                                MedicineRemover(inventoryFile);
                                break;
                            case 4:
                                edit_medicineInventory(inventoryFile);
                                break;
                            case 5: {
                                char searchName[50];
                                printf("Enter the medicine name to search: ");
                                fgets(searchName, sizeof(searchName), stdin);
                                removeNewlineChar(searchName);
                                search_medicine_by_name(inventoryFile, searchName);
                                break;
                            }
                            case 6:
                                delete_file(inventoryFile);
                                break;
                            case 7:
                                printf("Exiting to Admin Menu...\n");
                                break; // Break out of the loop to return to the Admin Menu
                            default:
                                handleInvalidInput();
                                printf("Invalid choice! Please enter a number between 1 and 7.\n");
                        }
                    } while (subChoice != 7);

                    break;
                }


                case 5:
                    printf("Logged out successfully!. see you next time >w<\n");
                    return; // Break out of the loop and function

                default:
                    handleInvalidInput();
            }
        }
    } else if (strcmp(role, "DOCTOR") == 0) {
        while (1) {
            doctorMainMenu(username);
            return;
        }
    } else if (strcmp(role, "NURSE") == 0) {
        while (1) {
            nurseMainMenu();
            return;
        }
    } else if (strcmp(role, "PATIENT") == 0) {
        while (1) {
            //patientMain();
        }
    }
}

void FirstMenu() {
    int inputPosition;
    int isValidInput = 0;

    while (!isValidInput) {
        printf("Choose what position you are:\n");
        printf("1. Staff\n");
        printf("2. Patient\n");
        printf("Enter the number corresponding to your position: ");
        if (scanf("%d", &inputPosition) == 1) {
            if (inputPosition == 1 || inputPosition == 2) {
                isValidInput = 1; // Valid input received
            } else {
                printf("Invalid input. Please enter 1 for Staff or 2 for Patient.\n");
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }

    if (inputPosition == 1) {
        printf("You selected Staff.\n");
        displayMainMenu();
        // Handle staff-related functionality
    } else if (inputPosition == 2) {
        printf("You selected Patient.\n");
        PatientMain();
        // Handle patient-related functionality
    }
}

int main() {
    initializeRoles();
    Initialise_EHRFile();
    Initialise_DoctorScheduleFile();
    Initialise_TreatmentsFile();
    FirstMenu();
    return 0;
}
