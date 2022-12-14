import requests
import os
from datetime import date
import browser_cookie3
import sys

#Get cookies from the browser
cookiejar = browser_cookie3.chrome('C:\\Users\\derek\\AppData\\Local\\Google\\Chrome\\User Data\\Profile 2\\Network\\Cookies')
    
#Get today number of day
day_today = date.today().strftime("%d").lstrip("0")

#If we provide an argument, use it as the desired day. Ex: ./startDay.py 5. Otherwise use day_today
if len(sys.argv) > 1:
    day = int(sys.argv[1])
    if day<0 or day>31 or day>int(day_today):
        exit("Day is not valid")
else:
    day = day_today


print(f"Downloading day {day}...")

r = requests.get(f"https://adventofcode.com/2022/day/{day}/input", cookies = cookiejar)
with open(f"inputs/day_{day}.txt","w") as f:
    f.write(r.text)

print("done.")


