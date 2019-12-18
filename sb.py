import snowboydecoder
import sys
import signal

def hotWord(models):
	
	sensitivity = [0.5]*len(models)
	detector = snowboydecoder.HotwordDetector(models, sensitivity=sensitivity)
	print('Listening... Press Ctrl+C to exit')

	# main loop
	word = detector.start(detected_callback=snowboydecoder.play_audio_file,
					sleep_time=0.03)
	return(word)	

words = ['gohome.pmdl', 'livingroom.pmdl', 'kitchen.pmdl', 'bedroom.pmdl']
word = hotWord(words)
if word == '1':
	print "go to home\n"
elif word == '2':
	print "go to livingroom\n"
elif word == '3':
	print "go to kitchen\n"
elif word == '4':
	print "go to bedroom\n"
