import psycopg2
#establishing the connection
conn = psycopg2.connect(
   database="mydb", user='postgres', password='dakshana', host='127.0.0.1', port= '5432'
)
#Creating a cursor object using the cursor() method
cursor = conn.cursor()

#Executing an MYSQL function using the execute() method
# cursor.execute("select version()")

# # Fetch a single row using fetchone() method.
# data = cursor.fetchone()
# print("Connection established to: ",data)
qlist = ["SELECT Physician.Name FROM trained_in, Physician, procedure WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician;",
 "SELECT Physician.Name FROM trained_in, Physician, procedure , Department ,Affiliated_With WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician AND Affiliated_With.Department = Department.DepartmentID AND Department.NAME = 'cardiology' AND Affiliated_With.Physician = Physician.EmployeeID;" ,
 "SELECT NAME FROM Nurse WHERE EmployeeID IN ( SELECT On_Call.Nurse FROM  On_Call , Block , Room WHERE On_Call.BlockCode = Block.Code AND On_Call.BlockFloor = Block.Floor AND Room.BlockCode = Block.Code AND Room.BlockFloor = Block.Floor AND Room.Number = 123 );" , 
 "SELECT Patient.NAME , Patient.Address FROM Patient , Prescribes , Medication WHERE Prescribes.Patient = Patient.SSN AND Prescribes.Medication = Medication.Code AND Medication.NAME = 'Remdesivir' ; " , 
 "SELECT Patient.NAME , Patient.Insurance_ID FROM Patient , Stay , Room WHERE Patient.SSN = Stay.Patient AND Stay.Room = Room.Number AND Room.Type = 'icu' AND Stay.EndTime - Stay.StartTime > '15 day' :: interval ;" ,
 "SELECT Nurse.NAME FROM Nurse , Procedure , Undergoes WHERE Procedure.Code = Undergoes.Procedure AND Undergoes.AssistNurse = Nurse.EmployeeID AND Procedure.NAME = 'bypass surgery' ;",
 "SELECT Nurse.NAME AS NURSE_NAME , Nurse.POSITION AS Nurse_Position , Physician.NAME AS physician_name  FROM Nurse , Procedure , Undergoes , Physician  WHERE Procedure.Code = Undergoes.Procedure AND Undergoes.AssistNurse = Nurse.EmployeeID AND Procedure.NAME = 'bypass surgery' AND Undergoes.Physician = Physician.EmployeeID ;",
 "SELECT Name FROM Physician WHERE EmployeeID IN (SELECT physician FROM Undergoes WHERE (physician, procedure) NOT IN (SELECT physician, treatment FROM trained_in));",
 "SELECT Name FROM Physician WHERE EmployeeID  IN (SELECT physician FROM Undergoes WHERE (physician, procedure) IN (SELECT physician, treatment FROM trained_in WHERE Undergoes.Date > Trained_In.certi_exp  ));",
 "SELECT Physician.NAME AS Physician_Name , Procedure.NAME AS procedure_name , Undergoes.DATE AS undergoes_date  , Patient.NAME AS patient_name FROM Physician , Patient , Procedure , Undergoes , Trained_In WHERE Physician.EmployeeID = Undergoes.Physician AND Physician.EmployeeID = Patient.PCP AND Undergoes.Patient = Patient.SSN  AND Procedure.Code = Undergoes.Procedure  AND Trained_In.Physician = Physician.EmployeeID AND Trained_In.Treatment = Procedure.Code AND Undergoes.Date > Trained_In.certi_exp ;" ,
 "SELECT DISTINCT patient.name AS patient_name, P1.name AS physician_name FROM patient, physician AS P1, physician AS P2, Department, Appointment, procedure, affiliated_with, undergoes, prescribes WHERE procedure.cost > 5000 AND undergoes.patient = patient.SSN AND undergoes.procedure = procedure.code AND Appointment.patient = patient.SSN AND Appointment.physician = P2.EmployeeID AND prescribes.physician = patient.PCP AND prescribes.patient = patient.SSN AND patient.PCP = P1.EmployeeID AND affiliated_with.physician = P2.EmployeeID AND affiliated_with.department = Department.DepartmentID AND NOT EXISTS (SELECT Head FROM Department WHERE Department.Head = P1.EmployeeID) AND Department.name = 'Cardiology' AND Appointment.patient IN (SELECT patient FROM Appointment GROUP BY patient HAVING COUNT(patient) > 1);" ,
 "WITH  TEMP ( Medication , value) AS (SELECT Medication , count(patient) FROM Prescribes  GROUP BY Medication ) , MAX_TABLE( value ) AS  ( SELECT max(value) from TEMP ) SELECT medication.name , medication.brand FROM medication , MAX_TABLE , temp WHERE medication.code = temp.Medication AND  MAX_TABLE.Value = TEMP.value;"]
while (True):

    inp = int(input("Enter the query number : "))
    if inp == 0 :
        break
    elif inp < 13 and inp > 0 :
        print(f"Query {inp} executes succefully \n")
        cursor.execute(qlist[inp-1])

    elif inp == 13:
        str = input("Enter the name procedure : ")
        query = f"SELECT Physician.Name FROM trained_in, Physician, procedure WHERE procedure.name = '{str}' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician;"
        cursor.execute(query)
    else :
        print("Invalid query number \n")
        continue
    # columns = [desc[0] for desc in cursor.description]

    # print("\t".join(columns))
    # rows = cursor.fetchall()
    # for row in rows:
    #     print("\t".join([str(x) for x in row]))
    result = cursor.fetchall()
    print( *result , sep="\n")
    print("\n")
conn.close()
# Connection established to: (
#    'PostgreSQL 11.5, compiled by Visual C++ build 1914, 64-bit',
# )