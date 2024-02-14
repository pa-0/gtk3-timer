![timer_screenshot.png](https://notabug.org/aussie114/timer/raw/master/screenshots/timer_screenshot.png)

Timer Written in C/GTK

## Dependencies 
GTK3  

## Usage
Takes in a time as a command line argument and starts a timer.  ie (./timer 00:10:00)  
Time can be formatted as SS, MM:SS or HH:MM:SS. Each field is not limited so SSSSSS or MMM:SSS is also permitted.  
Times exceeding 99:59:59 will be set to 99:59:59 due to digital display limitation.  
Program depends on images in data folder and accuracy depends on glibs g_timer.

## Problems  
digital display looks horrific when using a GTK light theme... not my problem.
