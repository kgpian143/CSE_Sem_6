CREATE TABLE Physician(
	EmployeeID INT NOT NULL,
	Name varchar(255),
	Position varchar(255),
	SSN INT NOT NULL,
	PRIMARY KEY (EmployeeID)
);

CREATE TABLE Department (
	DepartmentID INT NOT NULL,
	Name VARCHAR(255),
	Head INT NOT NULL,
	PRIMARY KEY(DepartmentID),
	FOREIGN KEY(Head) REFERENCES Physician(EmployeeID)
);

CREATE TABLE Affiliated_with(
	Physician INT NOT NULL,
	Department INT NOT NULL,
	PrimaryAffiliation BOOLEAN NOT NULL,
	PRIMARY KEY(Physician, Department),
	FOREIGN KEY(Physician) REFERENCES Physician(EmployeeID),
	FOREIGN KEY(Department) REFERENCES Department(DepartmentID)
);

CREATE TABLE procedure (
	code INT NOT NULL,
	Name VARCHAR(255),
	cost INT NOT NULL,
	PRIMARY KEY(code)
);

CREATE TABLE patient (
	SSN INT NOT NULL,
	NAME VARCHAR(255),
	address text,
	Phone Text,
	Insurance_ID INT NOT NULL,
	PCP INT NOT NULL,
	PRIMARY KEY(SSN),
	constraint fk_physician foreign key(pcp) references Physician(employeeid)
);

CREATE TABLE Nurse(
	EmployeeID INT NOT NULL,
	Name VARCHAR(255),
	Position VARCHAR(255),
	Registered BOOLEAN NOT NULL,
	SSN INT NOT NULL,
	PRIMARY KEY(EmployeeID)
);

CREATE TABLE Appointment(
	AppointmentID INT NOT NULL,
	Patient INT NOT NULL,
	PrepNurse INT NOT NULL,
	Physician INT NOT NULL,
	StartTime TIMESTAMP NOT NULL,
	EndTime TIMESTAMP NOT NULL,
	ExaminationRoom TEXT,
	PRIMARY KEY (AppointmentID),
	FOREIGN KEY (PrepNurse) REFERENCES Nurse(EmployeeID),
	FOREIGN KEY (Patient) REFERENCES Patient(SSN),
	FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID)
);

CREATE TABLE medication(
	code INT NOT NULL,
	Name VARCHAR(255),
	Brand VARCHAR(255),
	Description VARCHAR(255),
	PRIMARY KEY(code)
);

CREATE TABLE Prescribes(
	Physician INT NOT NULL,
	Patient INT NOT NULL,
	Medication INT NOT NULL,
	Date TIMESTAMP NOT NULL,
	Appointment INT NOT NULL,
	Dose TEXT,
	PRIMARY KEY (Physician, Patient, Medication, Date),
	FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
	FOREIGN KEY (Patient) REFERENCES Patient(SSN),
	FOREIGN KEY (Medication) REFERENCES Medication(Code),
	FOREIGN KEY (Appointment) REFERENCES Appointment(AppointmentID)
);

CREATE TABLE Block(
	Floor INT NOT NULL,
	Code INT NOT NULL,
	PRIMARY KEY(Floor, Code)
);

CREATE TABLE Room (
	Number INT NOT NULL,
	Type TEXT,
	BlockFloor INT NOT NULL,
	Blockcode INT NOT NULL,
	Unavailable BOOLEAN NOT NULL,
	PRIMARY KEY(Number),
	CONSTRAINT fk_Block FOREIGN KEY(BlockFloor, Blockcode) REFERENCES Block(Floor, Code)
);

CREATE TABLE On_Call(
	Nurse INT NOT NULL,
	BlockFloor INT NOT NULL,
	BlockCode INT NOT NULL,
	StartTime TIMESTAMP NOT NULL,
	EndTime TIMESTAMP NOT NULL,
	PRIMARY KEY (
		Nurse,
		BlockFloor,
		Blockcode,
		StartTime,
		EndTime
	),
	FOREIGN KEY (Nurse) REFERENCES Nurse(EmployeeID),
	FOREIGN KEY (BlockFloor, BlockCode) REFERENCES Block(Floor, Code)
);

CREATE TABLE Stay(
	stayID INT NOT NULL,
	Patient INT NOT NULL,
	Room INT NOT NULL,
	StartTime TIMESTAMP NOT NULL,
	EndTime TIMESTAMP NOT NULL,
	PRIMARY KEY(stayID),
	FOREIGN KEY(Room) REFERENCES Room(Number),
	FOREIGN KEY(Patient) REFERENCES Patient(SSN)
);

CREATE TABLE undergoes(
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
	FOREIGN KEY (Physician) REFERENCES Physician(employeeid),
	FOREIGN KEY (AssistNurse) REFERENCES Nurse(employeeid)
);

CREATE TABLE Trained_In(
	Physician INT NOT NULL,
	treatment INT NOT NULL,
	certi_date TIMESTAMP NOT NULL,
	certi_exp TIMESTAMP NOT NULL,
	PRIMARY KEY(Physician, treatment),
	CONSTRAINT fk_physician FOREIGN KEY(Physician) REFERENCES Physician(employeeid),
	CONSTRAINT fk_procedure FOREIGN KEY(treatment) REFERENCES procedure(code)
);

INSERT INTO Physician VALUES(1,'Alan Donald','Intern',111111111);
INSERT INTO Physician VALUES(2,'Bruce Reid','Attending Physician',222222222);
INSERT INTO Physician VALUES(3,'Courtney Walsh','Surgeon Physician',333333333);
INSERT INTO Physician VALUES(4,'Malcom Marshall','Senior Physician',444444444);
INSERT INTO Physician VALUES(5,'Dennis Lillee','Head Chief of Medicine',555555555);
INSERT INTO Physician VALUES(6,'Jeff Thomson','Surgeon Physician',666666666);
INSERT INTO Physician VALUES(7,'Richard Hadlee','Surgeon Physician',777777777);
INSERT INTO Physician VALUES(8,'Kapil  Dev','Resident',888888888);
INSERT INTO Physician VALUES(9,'Ishant Sharma','Psychiatrist',999999999);

INSERT INTO Department VALUES(1,'medicine',4);
INSERT INTO Department VALUES(2,'surgery',7);
INSERT INTO Department VALUES(3,'psychiatry',9);
INSERT INTO Department VALUES(4,'cardiology',8);

INSERT INTO Affiliated_With VALUES(1,1,TRUE);
INSERT INTO Affiliated_With VALUES(2,1,TRUE);
INSERT INTO Affiliated_With VALUES(3,1,FALSE);
INSERT INTO Affiliated_With VALUES(3,2,TRUE);
INSERT INTO Affiliated_With VALUES(4,1,TRUE);
INSERT INTO Affiliated_With VALUES(5,1,TRUE);
INSERT INTO Affiliated_With VALUES(6,2,TRUE);
INSERT INTO Affiliated_With VALUES(7,1,FALSE);
INSERT INTO Affiliated_With VALUES(7,2,TRUE);
INSERT INTO Affiliated_With VALUES(8,1,TRUE);
INSERT INTO Affiliated_With VALUES(9,3,TRUE);

INSERT INTO Procedure VALUES(1,'bypass surgery',1500.0);
INSERT INTO Procedure VALUES(2,'angioplasty',3750.0);
INSERT INTO Procedure VALUES(3,'arthoscopy',4500.0);
INSERT INTO Procedure VALUES(4,'carotid endarterectomy',10000.0);
INSERT INTO Procedure VALUES(5,'cholecystectomy',4899.0);
INSERT INTO Procedure VALUES(6,'tonsillectomy',5600.0);
INSERT INTO Procedure VALUES(7,'cataract surgery',25.0);

INSERT INTO Patient VALUES(100000001,'Dilip Vengsarkar','42 Foobar Lane','555-0256',68476213,1);
INSERT INTO Patient VALUES(100000002,'Richie Richardson','37 Infinite Loop','555-0512',36546321,2);
INSERT INTO Patient VALUES(100000003,'Mark Waugh','101 Parkway Street','555-1204',65465421,2);
INSERT INTO Patient VALUES(100000004,'Ramiz Raza','1100 Sparks Avenue','555-2048',68421879,3);

INSERT INTO Nurse VALUES(101,'Eknath Solkar','Head Nurse',TRUE,111111110);
INSERT INTO Nurse VALUES(102,'David Boon','Nurse',TRUE,222222220);
INSERT INTO Nurse VALUES(103,'Andy Flowers','Nurse',FALSE,333333330);

INSERT INTO Appointment VALUES(13216584,100000001,101,1,'2018-04-24 10:00','2018-04-24 11:00','A');
INSERT INTO Appointment VALUES(26548913,100000002,101,2,'2018-04-24 10:00','2018-04-24 11:00','B');
INSERT INTO Appointment VALUES(36549879,100000001,102,1,'2018-04-25 10:00','2018-04-25 11:00','A');
INSERT INTO Appointment VALUES(46846589,100000004,103,4,'2018-04-25 10:00','2018-04-25 11:00','B');
INSERT INTO Appointment VALUES(69879231,100000003,103,2,'2018-04-26 11:00','2018-04-26 12:00','C');
INSERT INTO Appointment VALUES(86213939,100000004,102,9,'2018-04-27 10:00','2018-04-21 11:00','A');
INSERT INTO Appointment VALUES(93216548,100000002,101,2,'2018-04-27 10:00','2018-04-27 11:00','B');

INSERT INTO Medication VALUES(1,'Paracetamol','Z','N/A');
INSERT INTO Medication VALUES(2,'Actemra','Foolki Labs','N/A');
INSERT INTO Medication VALUES(3,'Molnupiravir','Bale Laboratories','N/A');
INSERT INTO Medication VALUES(4,'Paxlovid','Bar Industries','N/A');
INSERT INTO Medication VALUES(5,'Remdesivir','Donald Pharmaceuticals','N/A');

INSERT INTO Prescribes VALUES(1,100000001,1,'2018-04-24 10:47',13216584,'5');
INSERT INTO Prescribes VALUES(9,100000004,2,'2018-04-27 10:53',86213939,'10');
INSERT INTO Prescribes VALUES(8,100000002,2,'2018-04-27 10:53',86213939,'10');
INSERT INTO Prescribes VALUES(6,100000003,5,'2018-04-30 16:53',46846589,'5');

INSERT INTO Block VALUES(1,1);
INSERT INTO Block VALUES(1,2);
INSERT INTO Block VALUES(1,3);
INSERT INTO Block VALUES(2,1);
INSERT INTO Block VALUES(2,2);
INSERT INTO Block VALUES(2,3);
INSERT INTO Block VALUES(3,1);
INSERT INTO Block VALUES(3,2);
INSERT INTO Block VALUES(3,3);
INSERT INTO Block VALUES(4,1);
INSERT INTO Block VALUES(4,2);
INSERT INTO Block VALUES(4,3);

INSERT INTO Room VALUES(102,'Single',1,1,FALSE);
INSERT INTO Room VALUES(101,'Single',1,1,FALSE);
INSERT INTO Room VALUES(103,'Single',1,1,FALSE);
INSERT INTO Room VALUES(111,'Single',1,2,FALSE);
INSERT INTO Room VALUES(112,'Single',1,2,TRUE);
INSERT INTO Room VALUES(113,'Single',1,2,FALSE);
INSERT INTO Room VALUES(121,'Single',1,3,FALSE);
INSERT INTO Room VALUES(122,'Single',1,3,FALSE);
INSERT INTO Room VALUES(123,'Single',1,3,FALSE);
INSERT INTO Room VALUES(201,'Single',2,1,TRUE);
INSERT INTO Room VALUES(202,'Single',2,1,FALSE);
INSERT INTO Room VALUES(203,'Single',2,1,FALSE);
INSERT INTO Room VALUES(211,'Single',2,2,FALSE);
INSERT INTO Room VALUES(212,'Single',2,2,FALSE);
INSERT INTO Room VALUES(213,'Single',2,2,TRUE);
INSERT INTO Room VALUES(221,'Single',2,3,FALSE);
INSERT INTO Room VALUES(222,'Single',2,3,FALSE);
INSERT INTO Room VALUES(223,'Single',2,3,FALSE);
INSERT INTO Room VALUES(301,'Single',3,1,FALSE);
INSERT INTO Room VALUES(302,'Single',3,1,TRUE);
INSERT INTO Room VALUES(303,'Single',3,1,FALSE);
INSERT INTO Room VALUES(311,'Single',3,2,FALSE);
INSERT INTO Room VALUES(312,'Single',3,2,FALSE);
INSERT INTO Room VALUES(313,'Single',3,2,FALSE);
INSERT INTO Room VALUES(321,'Single',3,3,TRUE);
INSERT INTO Room VALUES(322,'Single',3,3,FALSE);
INSERT INTO Room VALUES(323,'Single',3,3,FALSE);
INSERT INTO Room VALUES(401,'Single',4,1,FALSE);
INSERT INTO Room VALUES(402,'Single',4,1,TRUE);
INSERT INTO Room VALUES(403,'Single',4,1,FALSE);
INSERT INTO Room VALUES(411,'Single',4,2,FALSE);
INSERT INTO Room VALUES(412,'Single',4,2,FALSE);
INSERT INTO Room VALUES(413,'Single',4,2,FALSE);
INSERT INTO Room VALUES(421,'Single',4,3,TRUE);
INSERT INTO Room VALUES(422,'Single',4,3,FALSE);
INSERT INTO Room VALUES(423,'Single',4,3,FALSE);
INSERT INTO Room VALUES(424,'icu',4,3,FALSE);

INSERT INTO On_Call VALUES(101,1,1,'2018-11-04 11:00','2018-11-04 19:00');
INSERT INTO On_Call VALUES(101,1,2,'2018-11-04 11:00','2018-11-04 19:00');
INSERT INTO On_Call VALUES(102,1,3,'2018-11-04 11:00','2018-11-04 19:00');
INSERT INTO On_Call VALUES(103,1,1,'2018-11-04 19:00','2018-11-05 03:00');
INSERT INTO On_Call VALUES(103,1,2,'2018-11-04 19:00','2018-11-05 03:00');
INSERT INTO On_Call VALUES(103,1,3,'2018-11-04 19:00','2018-11-05 03:00');

INSERT INTO Stay VALUES(3215,100000001,111,'2018-05-01','2018-05-04');
INSERT INTO Stay VALUES(3216,100000003,123,'2018-05-03','2018-05-14');
INSERT INTO Stay VALUES(3217,100000004,112,'2018-05-02','2018-05-03');
INSERT INTO Stay VALUES(3218,100000002,424,'2018-05-02','2018-05-18');

INSERT INTO Undergoes VALUES(100000001,6,3215,'2018-05-02',3,101);
INSERT INTO Undergoes VALUES(100000001,2,3215,'2018-05-03',7,101);
INSERT INTO Undergoes VALUES(100000004,1,3217,'2018-05-07',3,102);
INSERT INTO Undergoes VALUES(100000004,5,3217,'2018-05-09',6,NULL);
INSERT INTO Undergoes VALUES(100000001,7,3217,'2018-05-10',7,101);
INSERT INTO Undergoes VALUES(100000004,4,3217,'2018-05-13',3,103);

INSERT INTO Trained_In VALUES(3,1,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(3,2,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(3,5,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(3,6,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(3,7,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(6,2,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(6,5,'2017-01-01','2017-12-31');
INSERT INTO Trained_In VALUES(6,6,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,1,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,2,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,3,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,4,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,5,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,6,'2018-01-01','2018-12-31');
INSERT INTO Trained_In VALUES(7,7,'2018-01-01','2018-12-31');


-- Q1  

SELECT Physician.Name FROM trained_in, Physician, procedure WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician;


-- Q2

SELECT Physician.Name FROM trained_in, Physician, procedure , Department ,Affiliated_With WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician AND Affiliated_With.Department = Department.DepartmentID AND Department.NAME = 'cardiology' AND Affiliated_With.Physician = Physician.EmployeeID;


-- Q3 
SELECT NAME FROM Nurse WHERE EmployeeID IN ( SELECT On_Call.Nurse FROM  On_Call , Block , Room WHERE On_Call.BlockCode = Block.Code AND On_Call.BlockFloor = Block.Floor AND Room.BlockCode = Block.Code AND Room.BlockFloor = Block.Floor AND Room.Number = 123 );


-- Q4 

SELECT Patient.NAME , Patient.Address FROM Patient , Prescribes , Medication WHERE Prescribes.Patient = Patient.SSN AND Prescribes.Medication = Medication.Code AND Medication.NAME = 'Remdesivir' ; 

-- Q5 
SELECT Patient.NAME , Patient.Insurance_ID FROM Patient , Stay , Room WHERE Patient.SSN = Stay.Patient AND Stay.Room = Room.Number AND Room.Type = 'icu' AND Stay.EndTime - Stay.StartTime > '15 day' :: interval ;

-- Q6 

SELECT Nurse.NAME FROM Nurse , Procedure , Undergoes WHERE Procedure.Code = Undergoes.Procedure AND Undergoes.AssistNurse = Nurse.EmployeeID AND Procedure.NAME = 'bypass surgery' ;

-- Q7 

SELECT Nurse.NAME AS NURSE_NAME , Nurse.POSITION AS Nurse_Position , Physician.NAME AS physician_name  FROM Nurse , Procedure , Undergoes , Physician  WHERE Procedure.Code = Undergoes.Procedure AND Undergoes.AssistNurse = Nurse.EmployeeID AND Procedure.NAME = 'bypass surgery' AND Undergoes.Physician = Physician.EmployeeID ;

-- Q8 

SELECT Name FROM Physician WHERE EmployeeID IN (SELECT physician FROM Undergoes WHERE (physician, procedure) NOT IN (SELECT physician, treatment FROM trained_in));

-- Q9

SELECT Name FROM Physician WHERE EmployeeID  IN (SELECT physician FROM Undergoes WHERE (physician, procedure) IN (SELECT physician, treatment FROM trained_in WHERE Undergoes.Date > Trained_In.certi_exp  ));

-- Q10

SELECT Physician.NAME AS Physician_Name , Procedure.NAME AS procedure_name , Undergoes.DATE AS undergoes_date  , Patient.NAME AS patient_name FROM Physician , Patient , Procedure , Undergoes , Trained_In WHERE Physician.EmployeeID = Undergoes.Physician AND Physician.EmployeeID = Patient.PCP AND Undergoes.Patient = Patient.SSN  AND Procedure.Code = Undergoes.Procedure  AND Trained_In.Physician = Physician.EmployeeID AND Trained_In.Treatment = Procedure.Code AND Undergoes.Date > Trained_In.certi_exp ;

-- Q11 

SELECT DISTINCT patient.name AS patient_name, P1.name AS physician_name FROM patient, physician AS P1, physician AS P2, Department, Appointment, procedure, affiliated_with, undergoes, prescribes WHERE procedure.cost > 5000 AND undergoes.patient = patient.SSN AND undergoes.procedure = procedure.code AND Appointment.patient = patient.SSN AND Appointment.physician = P2.EmployeeID AND prescribes.physician = patient.PCP AND prescribes.patient = patient.SSN AND patient.PCP = P1.EmployeeID AND affiliated_with.physician = P2.EmployeeID AND affiliated_with.department = Department.DepartmentID AND NOT EXISTS (SELECT Head FROM Department WHERE Department.Head = P1.EmployeeID) AND Department.name = 'Cardiology' AND Appointment.patient IN (SELECT patient FROM Appointment GROUP BY patient HAVING COUNT(patient) > 1);

-- Q12

WITH  TEMP ( Medication , value) AS (SELECT Medication , count(patient) FROM Prescribes  GROUP BY Medication ) , MAX_TABLE( value ) AS  ( SELECT max(value) from TEMP ) SELECT medication.name , medication.brand FROM medication , MAX_TABLE , temp WHERE medication.code = temp.Medication AND  MAX_TABLE.Value = TEMP.value;