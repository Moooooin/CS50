-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find Crime scene description
SELECT description FROM crime_scene_reports
    WHERE month = 7 AND day = 28
    AND street = 'Humphrey Street';

-- Look for more information in the interviews during the day
SELECT * FROM interviews
    WHERE month = 7 AND day = 28;

-- Look for information about withdrwas from atm at legget street
-- (Because in the interview Eugene said that the thief did that)
SELECT * FROM atm_transactions
    WHERE year = 2021 AND month = 7 AND day = 28
    AND atm_location LIKE '%Leggett%';

-- Narrow search down (based on things already known and interview from Ruth)
SELECT * FROM people
    WHERE id IN (
        SELECT person_id FROM bank_accounts
        WHERE account_number IN (
            SELECT account_number
            FROM atm_transactions
             WHERE year = 2021 AND month = 7 AND day = 28
             AND transaction_type = 'withdraw'
             AND atm_location = 'Leggett Street'
        )
        AND license_plate IN (
            SELECT license_plate
            FROM bakery_security_logs
             WHERE day = 28 AND month = 7 AND year = 2021
             AND hour = 10
             AND minute > 15
             AND minute < 26
             AND activity = 'exit'
        )
    );

-- Look at implementation from phone_calls table
SELECT * FROM phone_calls
    LIMIT 5;

-- Look at implementation from flights table
SELECT * FROM flights
    LIMIT 5;

-- Find thief
-- (Based on things alreadsy known and interview from Raymond)
SELECT name FROM people
    WHERE phone_number IN (
        SELECT caller FROM phone_calls
            WHERE year = 2021 AND month = 7 AND day = 28
            AND duration < 60
            AND caller IN (
                SELECT phone_number FROM people
                    WHERE id IN (
                        SELECT person_id FROM bank_accounts
                        WHERE account_number IN (
                            SELECT account_number
                            FROM atm_transactions
                            WHERE year = 2021 AND month = 7 AND day = 28
                            AND transaction_type = 'withdraw'
                            AND atm_location = 'Leggett Street'
                        )
                    )
                    AND license_plate IN (
                        SELECT license_plate
                        FROM bakery_security_logs
                        WHERE day = 28 AND month = 7 AND year = 2021
                        AND hour = 10 AND minute > 15 AND minute < 26
                        AND activity = 'exit'
                    )
            )
    )
    AND passport_number IN (
        SELECT passport_number FROM passengers
         WHERE flight_id = (
            SELECT id FROM flights
             WHERE year = 2021 AND month = 7 AND day = 29
             AND origin_airport_id = (
                SELECT id FROM airports
                 WHERE city = 'Fiftyville'
             )
             ORDER BY hour ASC, minute ASC
         )
    );

-- Find the city he escaped to
SELECT city FROM airports
    WHERE id = (
        SELECT destination_airport_id
        FROM flights
         WHERE year = 2021 AND month = 7 AND day = 29
         AND origin_airport_id = (
            SELECT id FROM airports
             WHERE city = 'Fiftyville'
         )
         ORDER BY hour, minute
    );

-- Find accomplice
SELECT name FROM people
    WHERE phone_number IN (
        SELECT receiver FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28
        AND duration < 60
        AND caller = (
            SELECT phone_number FROM people
            WHERE name = 'Bruce'
        )
    );