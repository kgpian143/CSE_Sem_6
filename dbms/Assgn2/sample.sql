CREATE TABLE physician(EID INT NOT NULL, NAME VARCHAR(255), POSITION VARCHAR(255), SSN INT NOT NULL, PRIMARY KEY(EID));

CREATE TABLE dept (
DID INT NOT NULL,
Name VARCHAR(255),
Head INT NOT NULL,
PRIMARY KEY(DID),
CONSTRAINT fk_physician
	FOREIGN KEY(Head)
		REFERENCES physician(EID)
);

INSERT INTO physician(EID, NAME, POSITION, SSN)
VALUES(10023, 'abc', 'Surgeon', 12345),
	  (25712, 'def', 'OPD', 456789),
	  (45879, 'deg', 'Medicine', 13579);
	  
INSERT INTO dept(DID, Name, Head)
VALUES(45217, 'Heart', 10023),
	  (14785, 'ENT', 25712),
	  (45712, 'Pathology', 45879);

CREATE TABLE procedure (
	code INT NOT NULL,
	Name VARCHAR(255),
	cost INT NOT NULL,
	PRIMARY KEY(code)
);

CREATE TABLE medication(
	code INT NOT NULL,
	Name VARCHAR(255),
	Brand VARCHAR(255),
	Description VARCHAR(255),
	PRIMARY KEY(code)
);

CREATE TABLE Nurse(
	EID INT NOT NULL,
	Name VARCHAR(255),
	Position VARCHAR(255),
	Registered BOOLEAN NOT NULL,
	SSN INT NOT NULL,
	PRIMARY KEY(EID)
);

CREATE TABLE Block(
	Floor INT NOT NULL,
	Code INT NOT NULL,
	PRIMARY KEY(Floor, Code)
);
-- done 

CREATE TABLE Patient(
	SSN INT NOT NULL,
	Name VARCHAR(255),
	Address TEXT,
	Phone TEXT,
	InsuranceID INT NOT NULL,
	PCP INT NOT NULL,
	PRIMARY KEY(SSN),
	CONSTRAINT fk_physician
		FOREIGN KEY(PCP)
			REFERENCES physician(EID)
);

CREATE TABLE affiliated_with(
	physician INT NOT NULL,
	department INT NOT NULL,
	primaryaffiliation BOOLEAN NOT NULL,
	PRIMARY KEY(physician, department),
	CONSTRAINT fk_physician
		FOREIGN KEY(physician)
			REFERENCES physician(EID),
	CONSTRAINT fk_dept
		FOREIGN KEY(department)
			REFERENCES dept(DID)
);

CREATE TABLE undergoes(
	physician INT NOT NULL,
	treatment INT NOT NULL,
	certidate TIMESTAMP NOT NULL,
	certiexp TIMESTAMP NOT NULL,
	PRIMARY KEY(physician, treatment),
	CONSTRAINT fk_physician
		FOREIGN KEY(physician)
			REFERENCES physician(EID),
	CONSTRAINT fk_procedure
		FOREIGN KEY(treatment)
			REFERENCES procedure(code)
);

CREATE TABLE Room (
	Number INT NOT NULL,
	Type TEXT,
	BlockFloor INT NOT NULL,
	Blockcode INT NOT NULL,
	Unavailable BOOLEAN NOT NULL,
	PRIMARY KEY(Number),
	CONSTRAINT fk_Block
		FOREIGN KEY(BlockFloor, Blockcode)
			REFERENCES Block(Floor, Code)
);

CREATE TABLE Stay(
	stayID INT NOT NULL,
	Patient INT NOT NULL,
	Room INT NOT NULL,
	St TIMESTAMP NOT NULL,
	En TIMESTAMP NOT NULL,
	PRIMARY KEY(stayID),
	CONSTRAINT fk_Room
		FOREIGN KEY(Room)
			REFERENCES Room(Number),
	CONSTRAINT fk_Patient
		FOREIGN KEY(Patient)
			REFERENCES Patient(SSN)
);

CREATE TABLE Appointment(
	AID INT NOT NULL,
	Patient INT NOT NULL,
	PrepNurse INT,
	Physician INT NOT NULL,
	St TIMESTAMP NOT NULL,
	En TIMESTAMP NOT NULL,
	ExamRoom TEXT,
	PRIMARY KEY(AID),
	FOREIGN KEY (Patient) REFERENCES Patient(SSN),
	FOREIGN KEY (PrepNurse) REFERENCES Nurse(EID),
	FOREIGN KEY (Physician) REFERENCES Physician(EID)
);

CREATE TABLE Prescribes(
	Physician INT NOT NULL,
	Patient INT NOT NULL,
	Medication INT NOT NULL,
	Date TIMESTAMP NOT NULL,
	Appointment INT,
	Dose TEXT,
	PRIMARY KEY (Physician, Patient, Medication),
	FOREIGN KEY (Physician) REFERENCES Physician(EID),
	FOREIGN KEY (Patient) REFERENCES patient(SSN),
	FOREIGN KEY (Medication) REFERENCES Medication(code),
	FOREIGN KEY (Appointment) REFERENCES Appointment(AID)
);

CREATE TABLE Undergoes (
	Patient INT NOT NULL,
	Procedure INT NOT NULL,
	Stay INT NOT NULL,
	Date TIMESTAMP NOT NULL,
	Physician INT NOT NULL,
	AssistNurse INT,
	PRIMARY KEY (Patient, Procedure, Stay, Date),
	FOREIGN KEY (Patient) REFERENCES Patient(SSN),
	FOREIGN KEY (Procedure) REFERENCES Procedure(code),
	FOREIGN KEY (Stay) REFERENCES Stay(stayID),
	FOREIGN KEY (Physician) REFERENCES Physician(EID),
	FOREIGN KEY (AssistNurse) REFERENCES Nurse(EID)
);

CREATE TABLE OnCall(
	Nurse INT NOT NULL,
	BlockFloor INT NOT NULL,
	BlockCode INT NOT NULL,
	St TIMESTAMP NOT NULL,
	En TIMESTAMP NOT NULL,
	PRIMARY KEY (Nurse, BlockFloor, BlockCode, St, En),
	FOREIGN KEY (Nurse) REFERENCES Nurse(EID),
	FOREIGN KEY (BlockFloor, BlockCode) REFERENCES Block(Floor, Code)
);

CREATE TABLE trained_in (
	Physician INT NOT NULL,
	Treatment INT NOT NULL,
	CertiDate TIMESTAMP NOT NULL,
	CertiExp TIMESTAMP NOT NULL,
	PRIMARY KEY (Physician, Treatment),
	FOREIGN KEY (Physician) REFERENCES Physician(EID),
	FOREIGN KEY (Treatment) REFERENCES Procedure(Code)
);

-- Q1
SELECT Physician.Name
FROM trained_in, Physician, procedure
WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EID = trained_in.physician;

INSERT INTO procedure(code, name, cost)
VALUES(10054, 'bypass surgery', 1000),
	  (25717, 'covid', 1478),
	  (45847, 'cancer', 1000);
	  
	  
INSERT INTO trained_in(physician, treatment, CertiDate, CertiExp)
VALUES(10023, 10054, '2020-06-22 19:10:25-07', '2020-07-22 19:10:25-07'),
	  (25712, 10054, '2020-06-22 19:10:25-07', '2020-09-22 19:10:25-07'),
	  (45879, 45847, '2020-06-22 19:10:25-07', '2020-08-22 19:10:25-07');
	  
INSERT INTO affiliated_with(physician, department, primaryaffiliation)
VALUES(10023, 45217, TRUE),
	  (25712, 14785, TRUE),
	  (45879, 45712, FALSE);
	  
-- Q2
SELECT Name FROM Physician WHERE EID IN ((SELECT Physician FROM trained_in WHERE treatment IN (SELECT Code FROM Procedure WHERE name = 'bypass surgery')) INTERSECT (SELECT Physician FROM affiliated_with WHERE department IN (SELECT DID FROM dept WHERE name = 'Cardiology')));

SELECT Physician.Name
FROM affiliated_with, dept, trained_in, Physician, procedure
WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EID = trained_in.physician AND trained_in.physician = affiliated_with.physician AND affiliated_with.department = dept.DID AND dept.name = 'Cardiology';

-- Q3
SELECT name FROM Nurse WHERE EID IN (SELECT Nurse FROM OnCall WHERE (BlockFloor, BlockCode) IN (SELECT BlockFloor, BlockCode FROM Room WHERE Number = 123));

-- Q4
SELECT Name, Address FROM patient WHERE SSN IN (SELECT Patient FROM Prescribes WHERE Medication IN (SELECT Code FROM Medication WHERE Name = 'remdesivir'));

-- Q5
SELECT Name, InsuranceID FROM Patient WHERE SSN IN ((SELECT Patient FROM Stay WHERE Room IN (SELECT Number FROM Room WHERE Type = 'icu'))INTERSECT (SELECT Patient FROM Stay WHERE En - St > '15 day'::interval));
SELECT Name, InsuranceID FROM Patient WHERE SSN IN ((SELECT Patient FROM Stay WHERE Room IN (SELECT Number FROM Room WHERE Type = 'icu'))INTERSECT (SELECT Patient FROM Stay WHERE En > St + '15 day'::interval));

-- Q6
SELECT Name FROM Nurse WHERE EID IN (SELECT AssistNurse FROM Undergoes WHERE Procedure IN (SELECT Code FROM Procedure WHERE Name  = 'bypass surgery'));

-- Q7
SELECT Nurse.Name AS Nurse_Name, Nurse.Position AS Nurse_Position, physician.name AS Physician_Name FROM Nurse, Physician WHERE  (Nurse.EID, physician.EID) IN (SELECT AssistNurse, physician FROM Undergoes WHERE Procedure IN (SELECT Code FROM Procedure WHERE Name  = 'bypass surgery'));

-- Q8
SELECT Name FROM Physician WHERE EID IN (SELECT physician FROM Undergoes WHERE (physician, procedure) NOT IN (SELECT physician, treatment FROM trained_in));

-- Q9
SELECT Name FROM Physician WHERE EID IN (SELECT physician FROM trained_in WHERE (physician, treatment) IN (SELECT physician, procedure FROM Undergoes WHERE trained_in.certiexp < Date));

-- Q10
SELECT physician.name AS physician_name, procedure.name AS procedure_name, undergoes.date::DATE AS undergoes_date, patient.name AS patient_name 
FROM physician, procedure, trained_in, undergoes, patient
WHERE physician.EID = trained_in.physician AND
	  undergoes.procedure = procedure.code AND
	  undergoes.procedure = trained_in.treatment AND
	  undergoes.date > trained_in.certiexp AND
	  undergoes.patient = patient.SSN AND
	  undergoes.physician = trained_in.physician;

-- Q11
SELECT DISTINCT patient.name AS patient_name, P1.name AS physician_name
FROM patient, physician AS P1, physician AS P2, dept, Appointment, procedure, affiliated_with, undergoes, prescribes
WHERE procedure.cost > 5000 AND
	  undergoes.patient = patient.SSN AND
	  undergoes.procedure = procedure.code AND
	  Appointment.patient = patient.SSN AND
	  Appointment.physician = P2.EID AND
	  prescribes.physician = patient.PCP AND
	  prescribes.patient = patient.SSN AND
	  patient.PCP = P1.EID AND
	  affiliated_with.physician = P2.EID AND
	  affiliated_with.department = dept.DID AND
	  NOT EXISTS (SELECT Head FROM dept WHERE dept.Head = P1.EID) AND
	  dept.name = 'Cardiology' AND
	  Appointment.patient IN
		(SELECT patient FROM Appointment
		GROUP BY patient
		HAVING COUNT(patient) >= 2);

-- Q12

SELECT name, brand
FROM medication
WHERE code IN (SELECT medication FROM (SELECT medication, COUNT(medication) AS "medcnt" FROM prescribes GROUP BY medication ORDER BY "medcnt" DESC )AS TEMP LIMIT 1);

-- Inserts Needs to look into it
INSERT INTO Physician (EmployeeID, Name, Position, SSN) VALUES
(10001, 'Ajay', 'Chief Surgeon', 987650456),
(10002, 'Vijay', 'Assistant Surgeon', 564321987),
(10003, 'Ajit', 'MD', 379423890);

INSERT INTO Department (DepartmentID, Name, Head) VALUES
(100, 'cardiology', 10001),
(101, 'orthopaedics', 10002),
(102, 'dermatology', 10001);

INSERT INTO Patient (SSN, Name, Address, Phone, InsuranceID, PCP) VALUES
(644875996, 'Raman', 'Indore', '9780442379', 347934, 10003),
(697417949, 'Manik', 'Pune', '837893478', 894338, 10002),
(734798348, 'Barun', 'Mumbai', '842788923', 490054, 10003);

INSERT INTO Procedure (Code, Name, Cost) VALUES
(1, 'bypass surgery', 300000),
(2, 'hip transplant', 350000),
(3, 'lasik', 30000);

INSERT INTO Medication (Code, Name, Brand, Description) VALUES
(2310, 'remdesivir', 'Rx', 'antiviral'),
(2103, 'chloroquine', 'SII', 'antimalarial');

INSERT INTO Nurse (EmployeeID, Name, Position, Registered, SSN) VALUES
(10001, 'Jaya', 'Surgical Assistant', FALSE, 576286829),
(10002, 'Simi', 'Emergency', TRUE, 365781920);

INSERT INTO Block (Floor, Code) VALUES
(1, 23),
(2, 44);

INSERT INTO Room (Number, Type, BlockFloor, BlockCode, Unavailable) VALUES
(123, 'icu', 1, 23, FALSE),
(244, 'general', 2, 44, TRUE);

INSERT INTO Stay (StayID, Patient, Room, StartTime, EndTime) VALUES
(901, 697417949, 123, '2020-09-07 07:09:50', '2020-09-25 09:00:00'),
(803, 734798348, 123, '2020-08-08 08:07:54', '2020-08-20 09:00:00');

INSERT INTO Appointment (AppointmentID, Patient, PrepNurse, Physician, StartTime, EndTime, ExaminationRoom) VALUES
(29, 644875996, 10002, 10001, '2020-10-26 09:35:00', '2020-10-26 09:55:00', 'Doc Cabin'),
(38, 734798348, 10002, 10001, '2020-10-26 09:35:00', '2020-10-26 09:55:00', 'Doc Cabin'),
(40, 734798348, 10002, 10001, '2020-10-26 09:35:00', '2020-10-26 09:55:00', 'Doc Cabin'),
(34, 644875996, NULL, 10003, '2020-10-30 08:40:00', '2020-10-30 09:00:00', 'Doc Cabin');

INSERT INTO Affiliated_with (Physician, Department, PrimaryAffiliation) VALUES
(10001, 100, TRUE),
(10002, 101, TRUE),
(10003, 100, FALSE),
(10003, 102, TRUE);

INSERT INTO Trained_In (Physician, Treatment, CertificationDate, CertificationExpires) VALUES
(10001, 1, '2008-01-01 00:00:00', '2020-06-05 12:00:00'),
(10002, 2, '2011-05-09 00:00:00', '2021-08-23 07:00:00');

INSERT INTO Undergoes (Patient, Procedure, Stay, Date, Physician, AssistingNurse) VALUES
(734798348, 1, 803, '2020-08-09 10:00:00', 10001, 10002),
(644875996, 1, 803, '2020-08-10 10:00:00', 10001, 10002),
(697417949, 1, 901, '2020-09-08 10:00:00', 10002, 10001);

INSERT INTO Prescribes (Physician, Patient, Medication, Date, Appointment, Dose) VALUES
(10003, 644875996, 2310, '2020-10-30 09:00:00', 34, '5 days'),
(10003, 697417949, 2103, '2020-10-24 09:00:00', 29, '4 days'),
(10003, 734798348, 2103, '2020-11-09 09:00:00', 29, '7 days'); --Needs change

INSERT INTO On_Call (Nurse, BlockFloor, BlockCode, StartTime, EndTime) VALUES
(10001, 2, 44, '2020-08-08 10:30:00', '2020-08-09 22:30:00'),
(10002, 1, 23, '2020-07-23 12:00:00', '2020-07-25 06:00:00');
