-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Get information on the crime that took place
-- There is another report that is not relevant to the case that took place on that date

SELECT * FROM crime_scene_reports
WHERE day = 28 AND month = 7 AND street = "Humphrey Street" and id = 295;

-- Get the interviews at that date which mention bakery
SELECT * FROM interviews
WHERE day = 28 and month = 7 AND transcript LIKE "%bakery%";


-- CUE 1:  Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot,
-- you might want to look for cars that left the parking lot in that time frame.
-- Get car leaving 10 minutes after the theft

SELECT name FROM bakery_security_logs
INNER JOIN people
ON bakery_security_logs.license_plate = people.license_plate
WHERE day = 28 AND month = 7 AND hour = 10 AND minute <= 25 AND activity = "exit";

-- CUE 2: I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on
-- Leggett Street and saw the thief there withdrawing some money.

SELECT * FROM atm_transactions
INNER JOIN bank_accounts
ON atm_transactions.account_number = bank_accounts.account_number
INNER JOIN people
ON bank_accounts.person_id = people.id
WHERE transaction_type = "withdraw" AND month = 7 AND day = 28 AND atm_location = "Leggett Street";

-- CUE 3: As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Get the phone calls on day 28 month 7 around that where less than 60 seconds
-- ANd where on the earliest flight out of fiftyville on the 29th
SELECT * from people
    INNER JOIN phone_calls
    ON people.phone_number = phone_calls.caller
                WHERE day = 28
                AND month = 7
                AND duration < 60
                AND name in (SELECT name FROM passengers
                                            INNER JOIN flights
                                            ON passengers.flight_id = flights.id
                                            INNER JOIN airports
                                            ON flights.origin_airport_id = airports.id
                                            INNER JOIN people
                                            ON passengers.passport_number = people.passport_number
                                            WHERE city = "Fiftyville"
                                            AND day = 29
                                            AND month = 7
                                            AND hour = 8);

-- Combine the queries and find their common name holder
-- Thief is Bruce
SELECT * FROM people
    WHERE name in
        (SELECT name FROM bakery_security_logs
            INNER JOIN people
            ON bakery_security_logs.license_plate = people.license_plate
            WHERE day = 28 AND month = 7 AND hour = 10 AND minute <= 25 AND activity = "exit")

    AND name in
        (SELECT name FROM atm_transactions
            INNER JOIN bank_accounts
            ON atm_transactions.account_number = bank_accounts.account_number
            INNER JOIN people
            ON bank_accounts.person_id = people.id
            WHERE transaction_type = "withdraw" AND month = 7 AND day = 28 AND atm_location = "Leggett Street")

    AND name in
        (SELECT name from people
            INNER JOIN phone_calls
            ON people.phone_number = phone_calls.caller
                        WHERE day = 28
                        AND month = 7
                        AND duration < 60
                        AND name in (SELECT name FROM passengers
                                                    INNER JOIN flights
                                                    ON passengers.flight_id = flights.id
                                                    INNER JOIN airports
                                                    ON flights.origin_airport_id = airports.id
                                                    INNER JOIN people
                                                    ON passengers.passport_number = people.passport_number
                                                    WHERE city = "Fiftyville"
                                                    AND day = 29
                                                    AND month = 7
                                                    AND hour = 8));

-- Bruce called Robin
SELECT * FROM people
WHERE phone_number in (
SELECT receiver from people
    INNER JOIN phone_calls
    ON people.phone_number = phone_calls.caller
    WHERE day = 28
    AND month = 7
    AND duration < 60
    AND name = "Bruce");

-- They fly to New York City
SELECT * FROM airports
    WHERE id = 4;