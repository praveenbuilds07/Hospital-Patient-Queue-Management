# Hospital Patient Queue Appointment Management System

A beginner-friendly C mini project based on the CIET Data Structures Lab .

## Features
- Register patients with a unique patient ID
- Automatically classify patients into a department and consultation room
- View all registered patients
- Add appointment requests to a FIFO queue
- Process the next appointment
- Save patient details in `patients.txt` and load them on the next run

## Data structures used
- **Singly linked list:** stores patient records dynamically.
- **Circular queue:** processes appointment requests in First-In-First-Out (FIFO) order.
- **File handling:** keeps patient records between program runs.

## Department classification
- Age 12 or below -> Paediatrics / `Paediatric_OPD`
- Symptom containing `chest` or `heart` -> Cardiology / `Cardiology_OPD`
- Symptom containing `fracture` or `joint` -> Orthopaedics / `Orthopaedic_OPD`
- Other symptoms -> General Medicine / `General_OPD`

Symptom matching is case-insensitive. Enter names and symptoms as one word (no spaces), for example `Ravi` and `Chestpain`.

## How to compile and run

### Windows (GCC / MinGW)
Open Command Prompt in this folder:
```bash
gcc hospital.c -o hospital.exe
hospital.exe
```

### Linux
```bash
gcc hospital.c -o hospital
./hospital
```

### macOS
```bash
clang hospital.c -o hospital
./hospital
```

You can also open `hospital.c` in VS Code and use its terminal to run the commands.

## Menu
1. Register Patient
2. View All Patients
3. Request Appointment
4. Process Appointment
5. Exit

## Example test
1. Register: `101 Ravi 52 Chestpain`
2. Register: `102 Meena 8 Fever`
3. Select `2` to view patients.
4. Select `3`, enter `101` to request an appointment.
5. Select `4` to process the appointment.

## Notes
- `patients.txt` is created automatically when the first patient is registered.
- Patient records remain saved after closing the program.
- The appointment queue is in memory and starts empty each time the program starts.
- This is an academic demonstration project, not a real hospital or clinical decision system.
