import datetime

from colors import Colors

MAXIMUM_DAYS = 11000
MINIMUM_DAYS = 30
MINIMUM_MONEY = 20
LIMIT_RISK_FROM_TIME = 91


def getUserInvestinTime():
    print("Input desired time period for which the bot is gonna be trading.\n"
          "Input number in days with minimum being" +
          Colors.bold + Colors.FGround.blue + f" {MINIMUM_DAYS} " +
          Colors.reset + "and maximum days being" +
          Colors.bold + Colors.FGround.blue +
          f" {MAXIMUM_DAYS}" + Colors.reset)

    time = int()

    while 1:
        try:
            time = int(input())
        except ValueError:
            print(f"Wrong input! Allowed input is from {MINIMUM_DAYS} "
                  f"to {MAXIMUM_DAYS}!")
            continue

        if time < MINIMUM_DAYS or time > MAXIMUM_DAYS:
            print(f"Wrong input! Allowed input is from {MINIMUM_DAYS} "
                  f"to {MAXIMUM_DAYS}!")
        else:
            break

    return time


def getUserRiskLevel():
    print("Choose from risk level" + Colors.bold + Colors.FGround.blue +
          " 1 " + Colors.reset + "," +
          Colors.bold + Colors.FGround.blue + " 2 " + Colors.reset +
          "or" + Colors.bold + Colors.FGround.blue + " 3 " +
          Colors.reset + "while 3 being the riskiest")

    while 1:
        risk = input()
        if risk < "1" or risk > "3":
            print("Wrong input. Allowed input is either 1, 2, or 3!")
        else:
            break

    return risk


def getRiskFromTime(time):
    if time > LIMIT_RISK_FROM_TIME:
        return 1
    return 2


def setUp():
    print("First input amount of money you want to trade with. At least" +
          Colors.bold + Colors.FGround.blue +
          f" ${MINIMUM_MONEY}." + Colors.reset)

    money = int()

    while 1:
        try:
            money = int(input())
        except ValueError:
            print("Wrong input! Allowed input is either" + Colors.bold +
                  Colors.FGround.blue +
                  f" ${MINIMUM_MONEY}" + Colors.reset + " and more!")
            continue

        if money < MINIMUM_MONEY:
            print("Wrong input! Allowed input is either" + Colors.bold +
                  Colors.FGround.blue +
                  f" ${MINIMUM_MONEY}" + Colors.reset + " and more!")
        else:
            break

    print("Second, input" + Colors.bold + Colors.FGround.blue + "\n1 " +
          Colors.reset + "if you wish the bot work "
          "based on by you inputed risk level or" + Colors.bold +
          Colors.FGround.blue + "\n2 " + Colors.reset +
          "if you wish the bot work based on by you inputed time, where time "
          "is for how long you want to invest.")

    while 1:
        condition = input()
        if condition == "1":
            user_risk_level = getUserRiskLevel()
            with open("user_criteria.txt", "w") as f:
                f.write(f"R {str(user_risk_level)} -1\n{money}\n{money}")
            return
        elif condition == "2":
            user_time = getUserInvestinTime()
            risk = getRiskFromTime(user_time)
            time = datetime.date.today() + datetime.timedelta(days=user_time)
            with open("user_criteria.txt", "w") as f:
                f.write(f"T {risk} {str(time)}\n{money}\n{money}")
            return
        else:
            print("Wrong input. Allowed input is either 1 or 2!")
