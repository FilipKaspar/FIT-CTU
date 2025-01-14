import os
from colors import Colors

import trade


def main():
    print(Colors.bold + Colors.FGround.pink +
          "\tWelcome to Crypto Trading Bot 1.0\n" + Colors.reset)

    print("Current working directory:", os.getcwd())

    trade.startTrade()


if __name__ == '__main__':
    main()
