import os
import sys
import pandas as pd
import matplotlib.pyplot as plt
import openpyxl


from colors import Colors


def getXAxisLimits(df):  # Unused for now; used for previous plotting
    x_axis = list()
    tick = int(len(df['Date']) / 10)
    k = 0
    for i in df['Date']:
        if k == tick:
            x_axis.append(i)
            k = 0
        else:
            k += 1

    return x_axis


def show(test):
    while True:
        print(Colors.bold + Colors.FGround.green +
              "Type name (date) of the file you want to show "
              "or if you wish to quit, type" + Colors.bold +
              Colors.FGround.cyan + " y" + Colors.reset)

        if test:
            df = pd.read_excel('History/test.xlsx', engine='openpyxl')
        else:
            while True:
                file = input()
                if file == "y":
                    sys.exit(0)
                if not os.path.exists(f"History/{file}.xlsx"):
                    print(Colors.bold + Colors.FGround.red +
                          "There is no record for inputed date!\nType a "
                          "valid date or" +
                          Colors.bold + Colors.FGround.cyan + " y " +
                          Colors.bold + Colors.FGround.red +
                          "if you want to exit!" + Colors.reset +
                          Colors.reset)
                    continue
                break

            df = pd.read_excel(f'History/{file}.xlsx', engine='openpyxl')

        df1 = df[df['Action'] == 1]
        df2 = df[df['Action'] == 2]
        df3 = df[df['Action'] == 3]
        df4 = df[df['Action'] == 4]

        ax = df.reset_index().plot(
            x="Date", y="Price", color="grey", zorder=0, label='Price')
        df1.reset_index().plot(
            x="Date", y="Price", kind="scatter", marker="P", ax=ax,
            color='blue', label='Bought')
        df3.reset_index().plot(
            x="Date", y="Price", kind="scatter", marker="x", ax=ax,
            color='green', label='Sold_Gain')
        df2.reset_index().plot(
            x="Date", y="Price", kind="scatter", marker="x", ax=ax,
            color='red', label='Sold_Loss')
        df4.reset_index().plot(
            x="Date", y="Price", kind="scatter", marker="x", ax=ax,
            color='purple', label='Supported')
        plt.gcf().autofmt_xdate()

        df.reset_index().plot(
            x="Date", y="Total_Money", color='peru', label="Money")
        plt.gcf().autofmt_xdate()

        plt.legend()
        plt.show()

        if test:
            return 1


if __name__ == '__main__':
    show(0)
