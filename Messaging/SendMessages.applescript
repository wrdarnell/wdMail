tell application "Finder"
	set subscriberFile to (POSIX path of (container of (path to me) as alias)) & "Subscribers.txt" as POSIX file
	set subscribers to paragraphs of (read file subscriberFile)
	
	repeat with subscriber in subscribers
		if (length of subscriber) > 0 then
			tell application "Messages"
				set targetService to (id of 1st service whose service type = iMessage)
				set textMessage to "We've Got Mail!"
				set theBuddy to buddy subscriber of service id targetService
				send textMessage to theBuddy
			end tell
		end if
	end repeat
end tell
