import datetime
import os
import shutil

from colors import Colors


def savingTimeline():
    name = datetime.date.today()
    temp = 1
    while os.path.exists(f"History/{name}.xlsx"):
        name = f'{datetime.date.today()}_{temp}'
        temp += 1

    if not os.path.exists('History'):
        os.makedirs('History')

    shutil.move("timeline.xlsx", f"History/{name}.xlsx")


def deleteFiles():
    os.remove("user_criteria.txt")

    if os.path.exists("cryptoinfo.xlsx"):
        os.remove("cryptoinfo.xlsx")

    if not os.path.exists('timeline.xlsx'):
        print("Didn't even found first one")
        return

    savingTimeline()

    print(Colors.bold + Colors.FGround.lightred +
          "DON'T forget to check the website for any unselled crypto!" +
          Colors.reset)
    print(Colors.FGround.cyan + "Thank you for using this MEGAOP "
                                "FREE TENDIES generator 3000!" + Colors.reset)
