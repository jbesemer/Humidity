
MaxActive = False
MinActive = False
Low = 68
High = 72

def NewReading( value ):
	global MaxActive, MinActive

	# Maximus => enforce maximum, e.g., AC

	if MaxActive:
		if value < High:
			print "Deactivate AC"
			MaxActive = False
	else:
		if value > High:
			print "Activate AC"
			MaxActive = True

	# ActiveLo => enforce minimum, e.g., Heater

	if MinActive:
		if value > Low:
			print "Deactivate Heat"
			MinActive = False
	else:
		if value < Low:
			print "Activate Heat"
			MinActive = True


def RegulatorUnitTest():

	hi = 76;
	low = 64;

	for t in xrange(hi, low,-1):
		print "Test: ", t
		NewReading( t );

	for t in xrange(low,hi):
		print "Test: ", t
		NewReading( t );

RegulatorUnitTest()

