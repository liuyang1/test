set theStartDate to (current date) + (1 * days)
set hours of theStartDate to 15
set minutes of theStartDate to 0
set seconds of theStartDate to 0
set theEndDate to theStartDate + (1 * hours)

tell application "Calendar"
    tell calendar "Work"
        make new event with properties {summary:"Important Meeting!", start date:theStartDate, end date:theEndDate}
    end tell
end tell
