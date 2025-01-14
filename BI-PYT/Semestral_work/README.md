# Cryptocurrency Trading Bot

### About
As described in my semestral report, my project is focused on trading with 
cryptocurrencies. The bot works in a few steps.  
&emsp; When initially launched, the user needs to input the amount of money and 
either the risk, which determines with 
what currencies the bot is going to be trading or for how long the bot 
should be trading with the money.  
&emsp; After that, the bot begins to scrape a certain website for the currency 
details 
and make decisions based on the scraped values.  
&emsp; While scraping and analyzing, the bot is saving the values into 
files from where they can be loaded back when the bot is relaunched or at the 
end 
when we want to plot the results.  
&emsp; Last part is the plotting part. With a separate python program, we can 
plot how money and price changed over time.  

The bot is working and can be used for actual trading after connecting to 
the broker's site. Although the options and math behind it can be improved. I 
think the project has potential, and I'll continue working on it.

### Necessary Python Modules To Install
`requests` - used to connect to certain sites  
`html5lib` - format for scraping sites  
`beautifulsoup4` - used for scraping sites; to get the html source  
`pandas` - for storing information about the currency  
`matplotlib` - used to plot the statistic about the currency  
`openpyxl` - engine for handling .xlsx files

### How To Start The Bot
The main program can be run with the command `python main.py`. How to interact 
with the bot is described below.  
The plotting program can be run with the command `python graph.py`. 2 Matlab 
windows will open. From there you can control the graph (zoom and move).

### How To Interact With The Bot
The only time when a user interacts with the bot is at the begging when the user 
sets the conditions for the bot, or when the bot is relaunched after closing, in 
that case, the program offers to delete current conditions and set new ones. In 
terms of how to input the conditions, the program will guide you. 

All information is being saved continuously to corresponding files, so it is 
safe to exit 
the program at any time by pressing CTRL+C.  
(better solution will be 
implemented later on)

### Running Pytest
Unit tests are run simply by typing `pytest` in the main folder with all 
python files.  
Unit tests are testing if the limits are being calculated correctly, other 
smaller math functions, correct conditions for deleting files, and plotting 
the final timeline.

Test #4 works correctly only if the bot has been previously run. It's testing 
if created files are being handled correctly.
