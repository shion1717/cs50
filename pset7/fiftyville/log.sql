-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check how the crime_scene_reports database look like
SELECT * FROM crime_scene_reports LIMIT 5;

-- Retrieve records that happened on July 28, 2020 on Chamberlin Street
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day = 28 AND street = "Chamberlin Street";

-- Transcript:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time —
-- each of their interview transcripts mentions the courthouse.

-- Check how the crime_scene_reports database look like
SELECT * FROM interviews LIMIT 2;

-- Retrieve mentions of courthouse
SELECT * FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28 AND transcript LIKE "%courthouse%";

-- 3 clues:
-- 1: Left courthouse parking lot during 10:15am to 10:25am
-- 2: Thief withdraw money from ATM on Fifer Street before 10:15am
-- 3: Phone call with someone less than a minute. Taking earliest flight out of Fiftyville tomorrow, booked under called name.

-- Check how the courthouse_security_logs database look like
SELECT * FROM courthouse_security_logs LIMIT 2;

-- Retrieve licence plates of cars exiting courthouse on 28 July 2020 before 10:26am
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND activity = "exit" AND hour = 10 AND minute < 26;

-- Check how the atm_transactions database look like
SELECT * FROM atm_transactions LIMIT 2;

-- Retrieve atm transactions on Fifer street on crime day
SELECT * FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location = "Fifer Street" AND transaction_type = "withdraw";

-- Retrieve person_ID who withdrew money on fifer street on crime day
SELECT person_id FROM bank_accounts
WHERE account_number IN
    (SELECT account_number FROM atm_transactions
    WHERE year = 2020 AND month = 7 AND day = 28
    AND atm_location = "Fifer Street" AND transaction_type = "withdraw");

-- Retrieve phone calls less than a minute on crime day
SELECT * FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28
AND duration < 60;

-- Get ID of fiftyville airport, which is 8
SELECT * FROM airports WHERE full_name LIKE "%fiftyville%";

-- Get flights out of fiftyville on day after crime.
SELECT * FROM flights
WHERE year = 2020 AND month = 7 AND day = 29
AND origin_airport_id = 8;

-- Earliest flight id is 36
-- Get passengers on this flight
SELECT * FROM passengers
WHERE flight_id = 36;

-- Get person whose phone_number, passport number and license_plate coincides with the above
SELECT * FROM people
WHERE phone_number IN
    (SELECT caller FROM phone_calls
    WHERE year = 2020 AND month = 7 AND day = 28
    AND duration < 60)
AND passport_number IN
    (SELECT passport_number FROM passengers
    WHERE flight_id = 36)
AND license_plate IN
    (SELECT license_plate FROM courthouse_security_logs
    WHERE year = 2020 AND month = 7 AND day = 28
    AND activity = "exit" AND hour = 10 AND minute < 26)
AND id IN
    (SELECT person_id FROM bank_accounts
    WHERE account_number IN
        (SELECT account_number FROM atm_transactions
        WHERE year = 2020 AND month = 7 AND day = 28
        AND atm_location = "Fifer Street" AND transaction_type = "withdraw"));

-- The thief is Ernest! id - 686048
-- Get destination airport ID
SELECT * FROM flights WHERE id = 36;

-- Get destination airport name - London
SELECT * FROM airports WHERE id = 4;

-- Get phone number of accomplice
SELECT * FROM phone_calls
    WHERE year = 2020 AND month = 7 AND day = 28
    AND duration < 60 AND caller = "(367) 555-5533";

-- Get name of accomplice - Berthold
SELECT * FROM people
WHERE phone_number = "(375) 555-8161";