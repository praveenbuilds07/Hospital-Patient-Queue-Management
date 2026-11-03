/*
 * Hospital Patient Queue Appointment Management System
 * Based on the mini-project implementation in the CIET Data Structures Lab PDF.
 *
 * Data structures:
 *   - Singly linked list: stores registered patients
 *   - Circular queue: processes appointment requests in FIFO order
 *   - File handling: saves patient records in patients.txt
 *
 * Compile:
 *   gcc hospital.c -o hospital
 * Run:
 *   ./hospital          (Linux/macOS)
 *   hospital.exe        (Windows)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "patients.txt"
#define QUEUE_SIZE 20

typedef struct Patient {
    int id;
    char name[30];
    int age;
    char symptom[40];
    char department[30];
    char room[40];
    struct Patient *next;
} Patient;

static Patient *head = NULL;
static int appointmentQueue[QUEUE_SIZE];
static int front = 0;
static int rear = 0;
static int queueCount = 0;

int containsIgnoreCase(const char *text, const char *word) {
    size_t n = strlen(word);
    if (n == 0) return 1;

    for (; *text; text++) {
        size_t i = 0;
        while (i < n && text[i] &&
               tolower((unsigned char)text[i]) ==
               tolower((unsigned char)word[i])) {
            i++;
        }
        if (i == n) return 1;
    }
    return 0;
}

Patient *findPatient(int id) {
    Patient *current = head;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

void classifyPatient(Patient *p) {
    if (p->age <= 12) {
        strcpy(p->department, "Paediatrics");
        strcpy(p->room, "Paediatric_OPD");
    } else if (containsIgnoreCase(p->symptom, "chest") ||
               containsIgnoreCase(p->symptom, "heart")) {
        strcpy(p->department, "Cardiology");
        strcpy(p->room, "Cardiology_OPD");
    } else if (containsIgnoreCase(p->symptom, "fracture") ||
               containsIgnoreCase(p->symptom, "joint")) {
        strcpy(p->department, "Orthopaedics");
        strcpy(p->room, "Orthopaedic_OPD");
    } else {
        strcpy(p->department, "General_Medicine");
        strcpy(p->room, "General_OPD");
    }
}

void savePatient(const Patient *p) {
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        printf("Warning: Could not open %s for saving.\n", FILE_NAME);
        return;
    }

    fprintf(fp, "%d %s %d %s %s %s\n",
            p->id, p->name, p->age, p->symptom,
            p->department, p->room);
    fclose(fp);
}

void addPatient(int id, const char name[], int age, const char symptom[]) {
    if (findPatient(id) != NULL) {
        printf("A patient with ID %d already exists.\n", id);
        return;
    }

    if (age <= 0 || age > 120) {
        printf("Please enter a valid age (1-120).\n");
        return;
    }

    Patient *p = (Patient *)malloc(sizeof(Patient));
    if (p == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    p->id = id;
    snprintf(p->name, sizeof(p->name), "%s", name);
    p->age = age;
    snprintf(p->symptom, sizeof(p->symptom), "%s", symptom);
    classifyPatient(p);

    p->next = head;
    head = p;

    savePatient(p);
    printf("Patient registered successfully!\n");
    printf("Department: %s | Room: %s\n", p->department, p->room);
}

void displayPatients(void) {
    Patient *current = head;

    if (current == NULL) {
        printf("\nNo patients available.\n");
        return;
    }

    printf("\n%-6s %-15s %-6s %-20s %-20s\n",
           "ID", "Name", "Age", "Department", "Room");
    printf("--------------------------------------------------------------------------\n");

    while (current != NULL) {
        printf("%-6d %-15s %-6d %-20s %-20s\n",
               current->id, current->name, current->age,
               current->department, current->room);
        current = current->next;
    }
}

void enqueue(int id) {
    if (findPatient(id) == NULL) {
        printf("Patient ID %d is not registered. Register the patient first.\n", id);
        return;
    }

    if (queueCount == QUEUE_SIZE) {
        printf("Appointment queue is full!\n");
        return;
    }

    appointmentQueue[rear] = id;
    rear = (rear + 1) % QUEUE_SIZE;
    queueCount++;

    printf("Appointment request for Patient %d added to queue.\n", id);
}

void dequeue(void) {
    if (queueCount == 0) {
        printf("No pending appointment requests.\n");
        return;
    }

    int id = appointmentQueue[front];
    front = (front + 1) % QUEUE_SIZE;
    queueCount--;

    printf("Processed appointment for Patient ID: %d\n", id);
}

void loadFromFile(void) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) return;

    int id, age;
    char name[30], symptom[40], department[30], room[40];

    while (fscanf(fp, "%d %29s %d %39s %29s %39s",
                  &id, name, &age, symptom, department, room) == 6) {
        if (findPatient(id) != NULL) continue;

        Patient *p = (Patient *)malloc(sizeof(Patient));
        if (p == NULL) {
            printf("Memory allocation failed while loading records.\n");
            break;
        }

        p->id = id;
        strcpy(p->name, name);
        p->age = age;
        strcpy(p->symptom, symptom);
        strcpy(p->department, department);
        strcpy(p->room, room);

        p->next = head;
        head = p;
    }

    fclose(fp);
}

void freePatients(void) {
    Patient *current = head;
    while (current != NULL) {
        Patient *next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    int choice, id, age;
    char name[30], symptom[40];

    loadFromFile();

    printf("Hospital Patient Queue Appointment Management System\n");

    while (1) {
        printf("\n--- HOSPITAL APPOINTMENT MANAGEMENT ---\n");
        printf("1. Register Patient\n");
        printf("2. View All Patients\n");
        printf("3. Request Appointment\n");
        printf("4. Process Appointment\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter ID, Name (no spaces), Age and Symptom (no spaces): ");
                if (scanf("%d %29s %d %39s", &id, name, &age, symptom) != 4) {
                    printf("Invalid patient details.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    break;
                }
                addPatient(id, name, age, symptom);
                break;

            case 2:
                displayPatients();
                break;

            case 3:
                printf("Enter Patient ID to queue for appointment: ");
                if (scanf("%d", &id) == 1) enqueue(id);
                else printf("Invalid patient ID.\n");
                break;

            case 4:
                dequeue();
                break;

            case 5:
                printf("Exiting...\n");
                freePatients();
                return 0;

            default:
                printf("Invalid option. Please choose 1-5.\n");
        }
    }

    freePatients();
    return 0;
}
