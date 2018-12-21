tell application "Finder"
	set subscriberFile to (POSIX path of (container of (path to me) as alias)) & "Subscribers.txt" as POSIX file
	
	set subscribers to paragraphs of (read file subscriberFile)
	
	repeat with subscriber in subscribers
		if (length of subscriber) > 0 then
			display alert subscriber
		end if
	end repeat
end tell

tell application "Messages"
	
	
	# set targetBuddy to "somePhoneNumber "
	# set targetService to id of 1st service whose service type = iMessage
	# set textMessage to "Hello World!"
	# set theBuddy to buddy targetBuddy of service id targetService
	# send textMessage to theBuddy
end tell