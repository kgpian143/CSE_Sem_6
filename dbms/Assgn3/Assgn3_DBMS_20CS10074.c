#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
void do_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

int main()
{

    PGconn *conn = PQconnectdb("user='postgres' password='dakshana' dbname='mydb' hostaddr = '127.0.0.1' port = 5432 ");

    if (PQstatus(conn) == CONNECTION_BAD)
    {

        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        do_exit(conn);
    }

    // char *user = PQuser(conn);
    // char *db_name = PQdb(conn);
    // char *pswd = PQpass(conn);

    // printf("User: %s\n", user);
    // printf("Database name: %s\n", db_name);
    // printf("Password: %s\n", pswd);
    char qlist[12][5000] = {
        "SELECT Physician.Name FROM trained_in, Physician, procedure WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician;",
        "SELECT Physician.Name FROM trained_in, Physician, procedure , Department ,Affiliated_With WHERE procedure.name = 'bypass surgery' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician AND Affiliated_With.Department = Department.DepartmentID AND Department.NAME = 'cardiology' AND Affiliated_With.Physician = Physician.EmployeeID;",
        "SELECT NAME FROM Nurse WHERE EmployeeID IN ( SELECT On_Call.Nurse FROM  On_Call , Block , Room WHERE On_Call.BlockCode = Block.Code AND On_Call.BlockFloor = Block.Floor AND Room.BlockCode = Block.Code AND Room.BlockFloor = Block.Floor AND Room.Number = 123 );",
        "SELECT Patient.NAME , Patient.Address FROM Patient , Prescribes , Medication WHERE Prescribes.Patient = Patient.SSN AND Prescribes.Medication = Medication.Code AND Medication.NAME = 'Remdesivir' ; ",
        "SELECT Patient.NAME , Patient.Insurance_ID FROM Patient , Stay , Room WHERE Patient.SSN = Stay.Patient AND Stay.Room = Room.Number AND Room.Type = 'icu' AND Stay.EndTime - Stay.StartTime > '15 day' :: interval ;",
        "SELECT Nurse.NAME FROM Nurse , Procedure , Undergoes WHERE Procedure.Code = Undergoes.Procedure AND Undergoes.AssistNurse = Nurse.EmployeeID AND Procedure.NAME = 'bypass surgery' ;",
        "SELECT Nurse.NAME AS NURSE_NAME , Nurse.POSITION AS Nurse_Position , Physician.NAME AS physician_name  FROM Nurse , Procedure , Undergoes , Physician  WHERE Procedure.Code = Undergoes.Procedure AND Undergoes.AssistNurse = Nurse.EmployeeID AND Procedure.NAME = 'bypass surgery' AND Undergoes.Physician = Physician.EmployeeID ;",
        "SELECT Name FROM Physician WHERE EmployeeID IN (SELECT physician FROM Undergoes WHERE (physician, procedure) NOT IN (SELECT physician, treatment FROM trained_in));",
        "SELECT Name FROM Physician WHERE EmployeeID  IN (SELECT physician FROM Undergoes WHERE (physician, procedure) IN (SELECT physician, treatment FROM trained_in WHERE Undergoes.Date > Trained_In.certi_exp  ));",
        "SELECT Physician.NAME AS Physician_Name , Procedure.NAME AS procedure_name , Undergoes.DATE AS undergoes_date  , Patient.NAME AS patient_name FROM Physician , Patient , Procedure , Undergoes , Trained_In WHERE Physician.EmployeeID = Undergoes.Physician AND Physician.EmployeeID = Patient.PCP AND Undergoes.Patient = Patient.SSN  AND Procedure.Code = Undergoes.Procedure  AND Trained_In.Physician = Physician.EmployeeID AND Trained_In.Treatment = Procedure.Code AND Undergoes.Date > Trained_In.certi_exp ;",
        "SELECT DISTINCT patient.name AS patient_name, P1.name AS physician_name FROM patient, physician AS P1, physician AS P2, Department, Appointment, procedure, affiliated_with, undergoes, prescribes WHERE procedure.cost > 5000 AND undergoes.patient = patient.SSN AND undergoes.procedure = procedure.code AND Appointment.patient = patient.SSN AND Appointment.physician = P2.EmployeeID AND prescribes.physician = patient.PCP AND prescribes.patient = patient.SSN AND patient.PCP = P1.EmployeeID AND affiliated_with.physician = P2.EmployeeID AND affiliated_with.department = Department.DepartmentID AND NOT EXISTS (SELECT Head FROM Department WHERE Department.Head = P1.EmployeeID) AND Department.name = 'Cardiology' AND Appointment.patient IN (SELECT patient FROM Appointment GROUP BY patient HAVING COUNT(patient) > 1);",
        "WITH  TEMP ( Medication , value) AS (SELECT Medication , count(patient) FROM Prescribes  GROUP BY Medication ) , MAX_TABLE( value ) AS  ( SELECT max(value) from TEMP ) SELECT medication.name , medication.brand FROM medication , MAX_TABLE , temp WHERE medication.code = temp.Medication AND  MAX_TABLE.Value = TEMP.value;"};
    while (1)
    {
        int q;
        printf("Enter the query number : ");
        scanf("%d", &q);
        PGresult *res;
        if (q == 0)
            break;
        else if (q == 13)
        {
            char procd[100] ;
            printf("Enter the procedure : ");
            scanf("%*c%[^\n]",procd) ;
            char query[1000] = "SELECT Physician.Name FROM trained_in, Physician, procedure WHERE procedure.name = '" ;
            char query1[1000] = "' AND trained_in.treatment = procedure.code AND physician.EmployeeID = trained_in.physician;";
            strcat(query , procd ) ;
            strcat(query , query1 ) ;
            res = PQexec(conn, query);
        }
        else if (q > 13 || q < 0)
        {
            printf("Invalid query number \n");
            continue;
        }
        else
        {
            res = PQexec(conn, qlist[q - 1]);
        }
        int cols = PQnfields(res);
        int rows = PQntuples(res);
        for (int i = 0; i < cols; i++)
        {
            char *name = PQfname(res, i);
            printf("%20s   ", name);
        }
        printf("\n") ;
        for (int i = 0; i < rows; i++)
        {
            for( int j = 0 ; j < cols ; j++ )
            {
                printf("%20s   ",PQgetvalue(res, i, j)) ;
            }
            printf("\n");
        }

        printf("\n") ;
    }
    PQfinish(conn);
    return 0;
}