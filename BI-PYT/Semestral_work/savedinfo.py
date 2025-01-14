from time import sleep
import sys

from colors import Colors
import cleaning_files
import setup


def checkReset():
    print(Colors.bold + Colors.FGround.cyan +
          "Program has been rerunned, do you wish to set new criteria "
          "for the bot?" + Colors.reset + "\nIf so, type" + Colors.bold +
          Colors.FGround.blue + " y " + Colors.reset + "otherwise press "
          "anything else")

    reset = input()

    if reset == "y":
        print("Program is gonna close. After rerun, you gonna be able "
              "to choose new criteria")
        sleep(2)
        print(Colors.bold + Colors.FGround.green + "Criteria Reset" +
              Colors.reset)

        cleaning_files.deleteFiles()
        sys.exit(0)


def getSavedInfo():
    print(Colors.bold + Colors.FGround.orange +
          "WARNINGS (There will always be warning for the first time "
          "running the program):" + Colors.reset)
    try:
        with open("user_criteria.txt", "r") as f:
            user_criteria = f.read()
        print(Colors.bold + Colors.FGround.green +
              "\tNo Errors while looking for files\n\n" + Colors.reset)
        checkReset()
        return user_criteria
    except FileNotFoundError:
        print(Colors.bold + Colors.FGround.orange +
              "\t - No user_risk.txt file\n\n" + Colors.reset)
        setup.setUp()
        with open("user_criteria.txt", "r") as f:
            user_criteria = f.read()
        return user_criteria
