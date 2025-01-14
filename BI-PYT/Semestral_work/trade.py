import os
import sys
import datetime
import time
import requests
from bs4 import BeautifulSoup
import pandas as pd
import html5lib

import cleaning_files
import savedinfo
from colors import Colors

SELL_FEE = 0.99  # Depend on your broker
ADD_BLANK_EVERY = 3  # 12
# Determines after how many 5 minutes intervals
# will be the price recorded regardless if it's being
# sold or bought
SLEEP_DUR = 60  # 300
TAKE_LIMIT = 0.02  # Multiplier to when to take the profit
WEBSITE = "https://coinmarketcap.com/currencies/"

HEADER = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0'
                        '; rv:91.0) Gecko/20100101 Firefox/91.0'}


def getNumFromStr(data):
    res = str()
    for i in data:
        if i.isdigit() or i == ".":
            res += i

    return float(res)


class CryptoInfo:
    name = str()
    risk = float()
    money_invested = float()
    money_left = float()
    first_print = int()

    avg_bought_values = float()
    current_value = float()

    take_profit = float()
    stop_loss = float()
    supported_loss = int()

    fee = float()

    def __init__(self, user_criteria):
        self.name = getCrypto(user_criteria)
        self.money_left = user_criteria.money
        self.money_invested = 0
        self.iterations = 0
        self.supported_loss = 0
        self.fee = SELL_FEE

        if user_criteria.risk == 1:
            self.risk = 0.70
        elif user_criteria.risk == 2:
            self.risk = 0.85
        elif user_criteria.risk == 3:
            self.risk = 0.85

    def checkSite(self, site_response):
        if site_response.status_code == 200:
            if self.first_print == 0:
                print(Colors.bold + Colors.FGround.green +
                      f"Site for {self.name.title()} is reachable." +
                      Colors.reset)
                self.first_print = 1
        else:
            print(Colors.bold + Colors.FGround.red +
                  f"Couldn't connect to the site for {self.name.title()}." +
                  Colors.reset)
            sys.exit(1)

    def getSoup(self):
        site_response = requests.get(
            WEBSITE + self.name.lower(), headers=HEADER)
        self.checkSite(site_response)
        soup = BeautifulSoup(site_response.text, "html5lib")

        return soup

    def getCryptoValue(self, soup):
        data = (soup.find(class_="priceValue")).text
        self.current_value = float(getNumFromStr(data))

    def getMeans(self, soup):
        temp = 0
        for i in (soup.find_all('table')[3].find_all("div")):
            if temp == 6:
                break

            if temp % 2 == 0:
                self.stop_loss += float(getNumFromStr(i.text))

            else:
                self.take_profit += float(getNumFromStr(i.text))

            temp += 1

        self.take_profit = self.take_profit / 3
        self.stop_loss = self.stop_loss / 3

    def getMoneyFromPrice(self, action):
        total_money = self.money_invested * \
                      (self.current_value / self.avg_bought_values)
        if action == 2 or action == 3:
            total_money *= self.fee
        total_money += self.money_left

        return float(total_money)

    def updateTimeLine(self, action):
        str_time = datetime.datetime.now().strftime("%m/%d/%Y %H:%M:%S")
        df = pd.DataFrame(
            [{'Date': datetime.datetime.strptime(str_time,
                                                 "%m/%d/%Y %H:%M:%S"),
              'Price': self.current_value, 'Action': action,
              'Total_Money': self.getMoneyFromPrice(action)}])

        with pd.ExcelWriter("timeline.xlsx", mode="a", engine="openpyxl",
                            if_sheet_exists="overlay") as writer:
            df.to_excel(writer, sheet_name="Sheet1", header=None,
                        startrow=writer.sheets["Sheet1"].max_row,
                        index=False)

    def firstTrade(self, user_criteria):
        temp = 0
        while True:
            if temp == 5:
                print("Still looking for first trade")
                temp = 0

            checkDate(user_criteria)  # Check if bot is supposed to stop
            soup = self.getSoup()

            self.getCryptoValue(soup)
            self.getMeans(soup)

            if self.current_value < (self.take_profit + self.stop_loss) / 2:
                self.money_left /= 2
                self.money_invested = self.money_left
                self.avg_bought_values = self.current_value
                self.stop_loss = self.stop_loss * self.risk
                self.take_profit = self.current_value * (1 - SELL_FEE +
                                                         TAKE_LIMIT + 1)
                #  (self.current_value + self.take_profit) / 2

                df_info = pd.DataFrame([{'Money_in_Crypto': self.money_left,
                                         'Value': self.current_value,
                                         "Take": self.take_profit,
                                         "Stop": self.stop_loss,
                                         'Left_Money': self.money_left}])
                df_info.to_excel('cryptoinfo.xlsx')

                if not os.path.exists("timeline.xlsx"):
                    str_time = datetime.datetime.now().strftime(
                        "%m/%d/%Y %H:%M:%S")
                    df_timeline = pd.DataFrame([{
                        'Date': datetime.datetime.strptime(
                            str_time, "%m/%d/%Y %H:%M:%S"),
                        'Price': self.current_value, 'Action': 1,
                        'Total_Money': self.money_left * 2}])
                    df_timeline.to_excel("timeline.xlsx", index=False)
                else:
                    self.updateTimeLine(1)

                break

            time.sleep(SLEEP_DUR)
            temp += 1

    def loadValues(self):
        crypto_info = pd.read_excel("cryptoinfo.xlsx", engine='openpyxl')
        self.money_left = crypto_info['Left_Money'].iloc[0]
        self.money_invested = crypto_info['Money_in_Crypto'].iloc[0]
        self.take_profit = crypto_info['Take'].iloc[0]
        self.stop_loss = crypto_info['Stop'].iloc[0]
        self.avg_bought_values = crypto_info['Value'].iloc[0]

    def sellCrypto(self, action):
        self.updateTimeLine(action)

        while self.current_value > self.take_profit / (1 - SELL_FEE +
                                                       TAKE_LIMIT + 1):
            time.sleep(SLEEP_DUR)
            self.getCryptoValue(self.getSoup())

    def updateCryptoValues(self):
        self.money_left /= 2
        self.avg_bought_values = ((self.money_invested *
                                   self.avg_bought_values) +
                                  (self.money_left * self.current_value)) / \
                                 (self.money_left + self.money_invested)
        self.money_invested += self.money_left

    def buyCrypto(self):
        self.updateCryptoValues()
        self.updateTimeLine(4)

    def printValues(self):
        print("Name: " + self.name.title())
        print("Current Value: $" + str(self.current_value))
        print("Avg Stock Price: $" + str(self.avg_bought_values))
        print("Currently invested: $" + str(self.money_invested))
        print("Money Left To Invest: $" + str(self.money_left))
        print("Risk Level: " + str(self.risk))
        print("Take mean: $" + str(round(self.take_profit, 3)))
        print("Stop mean: $" + str(round(self.stop_loss, 3)))


class UserCriteria:
    risk = int()
    money = float()
    date = str()

    def __init__(self, risk, money, end_date):
        self.risk = risk
        self.money = money
        self.date = end_date

    def getValues(self):
        print(self.risk)
        print(self.money)
        print(self.date)


def checkDate(criteria):
    if criteria.date != "-1":
        tmp = datetime.date.today()
        if type(criteria.date) is not datetime.datetime:
            criteria.date = datetime.datetime.strptime(criteria.date, "%Y-%m-%d")
        if criteria.date <= \
                datetime.datetime(tmp.year, tmp.month, tmp.day):
            print(Colors.bold + Colors.FGround.green +
                  "The bot has finished it's time period that's been set." +
                  Colors.reset)
            cleaning_files.deleteFiles()
            sys.exit(0)


def getCrypto(user_criteria):
    with open("crypto.txt", "r") as f:
        cryptos = f.read()

    if user_criteria.risk == 1 or user_criteria.risk == 2:
        cryptos = cryptos.splitlines()[0]
    else:
        cryptos = cryptos.splitlines()[1]

    return cryptos


def updateUserCriteria(crypto):
    user = open('user_criteria.txt', 'r+')
    temp = 0
    updated = str()
    for i in user.readlines():
        if temp == 1:
            updated += str(crypto.getMoneyFromPrice(2)) + "\n"
        else:
            updated += i
        temp += 1
    user.seek(0)
    user.truncate()
    user.write(updated)
    user.close()


def reloadUserData():
    with open("user_criteria.txt", "r") as f:
        user_criteria = f.read()
    user_criteria = UserCriteria(int(user_criteria[2]),
                                 float((user_criteria.split('\n'))[1]),
                                 str((user_criteria.split('\n'))[0])[4:])
    if user_criteria.date != "-1":
        user_criteria.date = \
            datetime.datetime.strptime(user_criteria.date, "%Y-%m-%d")

    return user_criteria


def startTrade():
    user_criteria = prepareTrade()
    while True:
        crypto = CryptoInfo(user_criteria)
        if not os.path.exists("cryptoinfo.xlsx"):
            print(Colors.bold + Colors.FGround.cyan +
                  "FIRST TRADE" + Colors.reset)
            crypto.firstTrade(user_criteria)
        else:
            print(Colors.bold + Colors.FGround.cyan +
                  "Values loaded successfully" + Colors.reset)
            crypto.loadValues()

        time.sleep(SLEEP_DUR)

        counter_insert_zero = 0
        while True:
            checkDate(user_criteria)
            crypto.getCryptoValue(crypto.getSoup())

            if crypto.current_value <= crypto.stop_loss:
                print(Colors.bold + Colors.FGround.red +
                      "SOLD CRYPTO: LOSS" + Colors.reset)
                crypto.sellCrypto(2)
                break

            if crypto.current_value >= crypto.take_profit:
                print(Colors.bold + Colors.FGround.green +
                      "SOLD CRYPTO: GAIN" + Colors.reset)
                crypto.sellCrypto(3)
                break

            if crypto.current_value <= \
                    (crypto.avg_bought_values + crypto.stop_loss) \
                    / 2 and crypto.supported_loss == 0:
                print(Colors.bold + Colors.FGround.blue +
                      "BOUGHT (SUPPORTED) CRYPTO" + Colors.reset)
                crypto.buyCrypto()

                crypto.supported_loss = 1
                counter_insert_zero = 0
                time.sleep(SLEEP_DUR)
                continue

            if counter_insert_zero == ADD_BLANK_EVERY:
                print(Colors.bold + Colors.FGround.purple +
                      "CURRENT VALUE ADDED TO TIMELINE" + Colors.reset)
                crypto.printValues()
                crypto.updateTimeLine(0)
                counter_insert_zero = -1

            counter_insert_zero += 1
            time.sleep(SLEEP_DUR)

        updateUserCriteria(crypto)
        user_criteria = reloadUserData()
        os.remove("cryptoinfo.xlsx")
        time.sleep(SLEEP_DUR)


def prepareTrade():
    input_criteria = savedinfo.getSavedInfo()
    user_criteria = UserCriteria(int(input_criteria[2]),
                                 float((input_criteria.split('\n'))[1]),
                                 str((input_criteria.split('\n'))[0])[4:])
    if user_criteria.date != "-1":
        user_criteria.date = datetime.datetime.strptime(
            user_criteria.date, "%Y-%m-%d")

    return user_criteria
