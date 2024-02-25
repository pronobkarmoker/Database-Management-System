-- CREATE TABLE TAKEN (
--     STUDENT VARCHAR2(20),
--     COURSE VARCHAR2(20)
-- );

-- CREATE TABLE REQUIRED (
--     COURSE VARCHAR2(20)
-- );

-- INSERT INTO TAKEN VALUES ('ROBERT', 'DATABASE');
-- INSERT INTO TAKEN VALUES ('ROBERT', 'PROGRAMMING LANGUAGE');
-- INSERT INTO TAKEN VALUES ('SUSIE', 'DATABASE');
-- INSERT INTO TAKEN VALUES ('SUSIE', 'OPERATING SYSTEM');
-- INSERT INTO TAKEN VALUES ('JULIE', 'PROGRAMMING LANGUAGE');
-- INSERT INTO TAKEN VALUES ('JULIE', 'MACHINE LEARNING');
-- INSERT INTO TAKEN VALUES ('EMILIE', 'OPERATING SYSTEM');

-- INSERT INTO REQUIRED VALUES ('DATABASE');
-- INSERT INTO REQUIRED VALUES ('PROGRAMMING LANGUAGE');

-- SELECT STUDENT FROM TAKEN 
-- MINUS 
-- SELECT STUDENT FROM (
--     SELECT STUDENT, COURSE 
--     FROM (SELECT STUDENT FROM TAKEN), REQUIRED 
--     MINUS 
--     SELECT STUDENT, COURSE FROM TAKEN);

-- SELECT STUDENT
-- FROM TAKEN
-- WHERE COURSE IN (SELECT COURSE FROM REQUIRED)
-- GROUP BY STUDENT
-- HAVING COUNT(*) = (SELECT COUNT(*) FROM REQUIRED);

select customer_name from depositor where account_number in 
(select account_number from account where branch_name In 
( select branch_name from branch where branch_city = 'BROOKLYN') 
minus 
select distinct account_number from 
(select account_number,branch_name from
(select account_number from account where branch_name In 
( select branch_name from branch where branch_city = 'BROOKLYN')),
(select branch_name from branch where branch_city ='BROOKLYN') 
minus 
( select account_number, branch_name from account)));
