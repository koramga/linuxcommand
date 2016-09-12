

currentTime : timerRes
	gcc -o currentTime currentTime.c

timerRes : timerSignal
	gcc -o timerRes timerRes.c

timerSignal : newTimer 
	gcc -o timerSignal timerSignal.c

newTimer : 
	gcc -o new_timer new_timer.c -lrt

clean :
	rm currentTime
	rm timerRes
	rm timerSignal
	rm new_timer
