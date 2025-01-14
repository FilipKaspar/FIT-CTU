import socket
import sys
import threading


def msgEncode(client_Action):
    client_Action = str(client_Action) + "\a\b"
    return client_Action.encode('UTF-8')


def serverHash(name, key):
    # print(key)
    hash = 0
    for i in name:
        # print(f"{type(i)}, {i}")
        hash += ord(i)
        # print(hash)
    hash *= 1000
    hash %= 65536
    tmp = hash
    # print(hash)
    hash += key
    hash %= 65536
    # print(hash)
    return tmp, hash


def compareHash(hash, key, server_Hash):
    temp = 65536 - int(key) + int(hash)
    temp %= 65536
    if temp == server_Hash:
        print("\tSending 200 OK")
        return "200 OK"
    print("\tSending 300 LOGIN FAILED")
    return "300 LOGIN FAILED"


def getKeys(key=int()):
    match key:
        case 0:
            return 23019, 32037
        case 1:
            return 32037, 29295
        case 2:
            return 18789, 13603
        case 3:
            return 16443, 29533
        case 4:
            return 18189, 21952


def getCords(client_Action, conn):
    if client_Action[0] != "O" and client_Action[1] != "K":
        print("ERROR IN GETTING CORDS")
        conn.sendall(msgEncode("301 SYNTAX ERROR"))
        conn.close()
        return 0, 0, -1

    client_Action = client_Action[3:len(client_Action)]
    if client_Action.find("~|~") != -1:
        pos = client_Action.find("~|~")
        client_Action = client_Action[:pos]

    res = str()
    x = str()
    y = str()

    for i in client_Action:
        if i == "-" or i.isdigit():
            res += i
        elif i == " ":
            x = res
            res = ""
        else:
            print("\tChyba v COORDS")
            conn.sendall(msgEncode("301 SYNTAX ERROR"))
            conn.close()
            return 0, 0, -1

    y = res

    print(f"{x} {y}")
    # print(f"LENGTH: {len(x)} {len(y)}")

    try:
        return int(x), int(y), 0
    except:
        print("\tChyba v COORDS")
        conn.sendall(msgEncode("301 SYNTAX ERROR"))
        conn.close()
        return 0, 0, -1


def login(key_Client, server_Hash, name, client_Action, status, conn):
    # Establishing connection with a robot

    if status == 0:
        print("Client: " + client_Action)
        name = client_Action
        # print(len(tmp))
        if len(name) <= 18:
            print("\tRequesting key_ID")
            conn.sendall(msgEncode("107 KEY REQUEST"))
        else:
            print("\tSending syntax error 301")
            conn.sendall(msgEncode("301 SYNTAX ERROR"))
            return 0, 0, 0, -1
        print("Waiting for key response")

    elif status == 1:
        try:
            key_ID = int(client_Action)
        except:
            print("\tSending syntax error 301")
            conn.sendall(msgEncode("301 SYNTAX ERROR"))
            return 0, 0, 0, -1

        print(f"KEY ID: {key_ID}")
        # print(type(client_Action))
        if key_ID > 4 or key_ID < 0:
            print("\tSending error: 303 KEY OUT OF RANGE")
            conn.sendall(msgEncode("303 KEY OUT OF RANGE"))
            conn.close()
            return 0, 0, 0, -1

        key_Server, key_Client = getKeys(key_ID)
        server_Hash, hash = serverHash(name, key_Server)
        # print(msgEncode(hash))
        print("\tSending server hash")
        print(f"HASH JE: {hash}")
        conn.sendall(msgEncode(hash))
        print("Waiting for client hash to check it")

    elif status == 2:
        if client_Action[len(client_Action) - 1] == " " or len(client_Action) > 5:
            print("\tSending syntax error 301")
            conn.sendall(msgEncode("301 SYNTAX ERROR"))
            return 0, 0, 0, -1

        compare = compareHash(client_Action, key_Client, server_Hash)
        print("\tSending hash for comparing")
        conn.sendall(msgEncode(compare))
        if compare == "300 LOGIN FAILED":
            print("While logging error, Closing connection")
            conn.close()
            return 0, 0, 0, -1

        print("Login successful")

    return key_Client, server_Hash, name, 0


def move(client_Action, x, y, dirrection, goal, conn):
    while dirrection != goal:
        print("\tSENDING 104 TURN RIGHT IN MOVE")
        conn.sendall(msgEncode("104 TURN RIGHT"))
        client_Action = getData(client_Action, conn, -1)
        pos = client_Action.find("~|~")
        if client_Action[:pos] == "RECHARGING":
            client_Action, req = recharge(client_Action, conn)
            if req == -1:
                return client_Action, dirrection, x, y, 2
            client_Action = getData(client_Action, conn, -1)

        client_Action = splitActions(client_Action)

        dirrection += 1
        dirrection %= 4
        print(f"CURRENT DIRECTION: {dirrection}")

    print("\tSending 102 MOVE IN MOVE")
    conn.sendall(msgEncode("102 MOVE"))
    print("JESTE NECRASHLO!!!")
    client_Action = getData(client_Action, conn, -1)
    pos = client_Action.find("~|~")
    if client_Action[:pos] == "RECHARGING":
        client_Action, req = recharge(client_Action, conn)
        if req == -1:
            return client_Action, dirrection, x, y, 2
        client_Action = getData(client_Action, conn, -1)
        return client_Action, dirrection, x, y, 0
    # print(client_Action)
    tx = x
    ty = y
    x, y, code = getCords(client_Action, conn)

    client_Action = splitActions(client_Action)

    if tx == x and ty == y:
        return client_Action, dirrection, x, y, 1

    return client_Action, dirrection, x, y, 0


def stuck(dirrection, client_Action, conn):
    dirrection += 1
    dirrection %= 4

    print("\tSENDING 104 TURN RIGHT IN STUCK")
    conn.sendall(msgEncode("104 TURN RIGHT"))
    client_Action = getData(client_Action, conn, -1)
    client_Action = splitActions(client_Action)
    print(f"CURRENT DIRECTION: {dirrection}")

    print("\tSending 102 MOVE IN STUCK")
    conn.sendall(msgEncode("102 MOVE"))
    client_Action = getData(client_Action, conn, -1)

    x, y, code = getCords(client_Action, conn)

    client_Action = splitActions(client_Action)

    return dirrection, x, y


def pickUp(client_Action, conn):
    print("\tPicking up message")
    conn.sendall(msgEncode("105 GET MESSAGE"))
    client_Action = getData(client_Action, conn, 4)

    pos = client_Action.find("~|~")
    if client_Action[:pos] == "RECHARGING":
        client_Action, req = recharge(client_Action, conn)
        if req == -1:
            print("\t Sending 302 LOGIC ERROR")
            conn.sendall(msgEncode("302 LOGIC ERROR"))
            conn.close()
            return
        client_Action = getData(client_Action, conn, 4)

    if client_Action == "optimiz":
        print("\tCHYBA V SECRET MESSAGE")
        conn.sendall(msgEncode("301 SYNTAX ERROR"))
    else:
        print(f"Secret message is: {client_Action}")
        conn.sendall(msgEncode("106 LOGOUT"))

    conn.close()


def getData(client_Action, conn, status):
    end = 0
    temp = ""
    opt = 0
    if status == 0:
        opt = 1
    elif status == 4:
        opt = 4

    if client_Action == "":
        while end == 0:
            client_Action = conn.recv(1000)

            print(f"KAZDEJ SENT PACKET: {client_Action}")
            print(f"KAZDEJ SENT PACKET DECODED: {client_Action.decode('UTF-8')}")

            if not client_Action:
                print("Program failed the test!")
                sys.exit()

            check = client_Action.decode('UTF-8')
            if check.endswith("\x07\x08"):
                end = 1
                temp += client_Action.decode('UTF-8')
            elif check.endswith("\x08"):
                if temp[(len(temp) - 1)] == "\x07":
                    end = 1
                temp += client_Action.decode('UTF-8')
            else:
                end = 0
                temp += client_Action.decode('UTF-8')

            print(f"END: {end}")
            try:
                if opt == 1 and temp[len(temp) - 1] == "\x07" or opt == 1 and temp[len(temp) - 1] == "\x07\x08":
                    if len(temp) > 18:
                        return "optimiz"
                    opt = 0
                elif opt == 4 and temp[len(temp) - 1] == "\x07" or opt == 4 and temp[len(temp) - 1] == "\x07\x08":
                    if len(temp) > 98:
                        return "optimiz"
                    opt = 0
                if temp[len(temp) - 1] == "\x07" and temp[len(temp) - 2] == "\x07":
                    return "optimiz"
            except:
                print("OPTIMIZ SKIPPED")

        client_Action = temp.encode('UTF-8')

        # client_Action = ""
        temp = temp[:len(temp) - 2]
        # print(f"MEZI SEGMENTACE A MERGE: {temp}")
        # print(f"MEZI SEGMENTACE A MERGE ENCODED: {temp.encode('UTF-8')}")
        # print(f"CLIENT_ACTION IS STILL: {client_Action}")
        if temp.find("\x07\x08") != -1:  # \a\b and \x07\x08 is the same
            final = ""
            while temp.find("\x07\x08") != -1:
                pos = temp.find("\a\b")
                # print(f"\t NA MISTE: {pos}")
                # print(client_Action[:pos])
                final += client_Action[:pos].decode("UTF-8") + "~|~"
                client_Action = client_Action[pos + 2:]
                temp = client_Action.decode('UTF-8')

            client_Action = final

        else:
            client_Action = temp + "~|~"

    # print(f"FINAL JE: {final}")
    # print(f"FINAL JE v ENCODED: {final.encode('UTF-8')}")

    # client_Action = temp[:len(temp)-2]
    print(f"FROM GETDATA: {client_Action}")
    print(f"ENCODED VERSION: {client_Action.encode('UTF-8')}")
    # print(f"LENGTH FROM GETDATA: {len(client_Action)}\n")

    return client_Action


def splitActions(client_Action):
    if client_Action.find("~|~") != -1:
        pos = client_Action.find("~|~")
        client_Action = client_Action[pos + 3:]  # Smazani jedne akce do ~|~

    return client_Action


def recharge(client_Action, conn):
    conn.settimeout(5)
    client_Action = ""
    client_Action = getData(client_Action, conn, -1)
    print(client_Action)

    pos = client_Action.find("~|~")
    if client_Action[:pos] != "FULL POWER":
        print("SPATNE NENI FULL POWER")
        conn.sendall(msgEncode("302 LOGIC ERROR"))
        conn.close()
        return client_Action, -1

    client_Action = splitActions(client_Action)

    print(client_Action)
    conn.settimeout(1)
    return client_Action, 0


def checkRecharge(client_Action, conn):
    pos = client_Action.find("~|~")
    if client_Action[:pos] == "RECHARGING":
        client_Action, req = recharge(client_Action, conn)
        if req == -1:
            print("\t Sending 302 LOGIC ERROR")
            conn.sendall(msgEncode("302 LOGIC ERROR"))
            conn.close()
    client_Action = getData(client_Action, conn, -1)

    return client_Action


def startCommunication(conn):
    x = 0
    y = 0
    dirrection = 0
    status = 0
    key_Client = 0
    server_Hash = 0
    name = ""
    client_Action = ""

    while True:
        print("Waiting for username")
        print("\tWent to loop")
        try:
            client_Action = ""
            status = 0
            # Logging robot in to the network
            while status < 3:
                print(client_Action)
                print(f"status JE: {status}")
                client_Action = getData(client_Action, conn, status)
                if client_Action == "optimiz":
                    print("\tKKKKKKKKKKKKCHYBA VE VSTUPU V PRUBEHU")
                    conn.sendall(msgEncode("301 SYNTAX ERROR"))
                    err = -1
                    break

                pos = client_Action.find("~|~")
                print(f"SENDING INFO TO VALIDATE: {client_Action[:pos]}")
                if client_Action[:pos] == "RECHARGING":
                    client_Action, req = recharge(client_Action, conn)
                    if req == -1:
                        err = -1
                        break
                    continue
                key_Client, server_Hash, name, err = login(key_Client, server_Hash, name, client_Action[:pos], status,
                                                           conn)
                client_Action = client_Action[pos + 3:]

                if err == -1:
                    status = 0
                    break
                status += 1

            if err == -1:
                err = 0
                print("\tClosing connection")
                conn.close()
                continue

            # Followed up with move command to determine where the robot is located
            status = 0
            print("Getting current position of the robot")
            print("\tSending 102 MOVE for first time")
            conn.sendall(msgEncode("102 MOVE"))

            client_Action = getData(client_Action, conn, -1)
            client_Action = checkRecharge(client_Action, conn)

            pos = client_Action.find("~|~")

            if client_Action[:pos] == "optimiz":
                print("\tCHYBA VE VSTUPU V PRUBEHU")
                conn.sendall(msgEncode("301 SYNTAX ERROR"))
                conn.close()
                continue

            print(f"Po prvnim movu: {client_Action}")
            x, y, code = getCords(client_Action[:pos], conn)
            if code == -1:
                continue

            client_Action = splitActions(client_Action)

            if x == 0 and y == 0:
                pickUp(client_Action, conn)
                continue

            # More moves needed

            print("\t Sending 102 MOVE")
            conn.sendall(msgEncode("102 MOVE"))

            tx = x
            ty = y
            client_Action = getData(client_Action, conn, -1)
            client_Action = checkRecharge(client_Action, conn)

            pos = client_Action.find("~|~")

            x, y, code = getCords(client_Action[:pos], conn)
            if code == -1:
                continue
            client_Action = splitActions(client_Action)

            if tx == x and ty == y:
                print("\tSENDING 104 TURN RIGHT")
                conn.sendall(msgEncode("104 TURN RIGHT"))
                client_Action = getData(client_Action, conn, -1)
                client_Action = checkRecharge(client_Action, conn)

                pos = client_Action.find("~|~")

                x, y, code = getCords(client_Action[:pos], conn)
                if code == -1:
                    continue
                client_Action = splitActions(client_Action)

                print("\tSENDING 102 MOVE")
                conn.sendall(msgEncode("102 MOVE"))
                client_Action = getData(client_Action, conn, -1)
                client_Action = checkRecharge(client_Action, conn)

                pos = client_Action.find("~|~")

                x, y, code = getCords(client_Action[:pos], conn)
                if code == -1:
                    continue
                client_Action = splitActions(client_Action)

            if tx == x:
                if ty > y:
                    dirrection = 3
                else:
                    dirrection = 1
            else:
                if tx > x:
                    dirrection = 0
                else:
                    dirrection = 2

            print(f"Direction is: {dirrection}")

            while (x != 0 or y != 0):
                print("MAIN WHILE")
                while x != 0:
                    print("Inner for loop in XXX")
                    if x > 0:
                        goalX = 0
                    else:
                        goalX = 2
                    print(f"GOAL X IS: {goalX}")

                    client_Action, dirrection, x, y, err = move(client_Action, x, y, dirrection, goalX, conn)
                    client_Action = splitActions(client_Action)

                    if err == 1 and y == 0:
                        dirrection, x, y = stuck(dirrection, client_Action, conn)
                    elif err == 1:
                        break

                while y != 0:
                    print("Inner for loop in YYY")
                    if y > 0:
                        goalY = 3
                    else:
                        goalY = 1
                    print(f"GOAL Y IS: {goalY}")

                    client_Action, dirrection, x, y, err = move(client_Action, x, y, dirrection, goalY, conn)
                    client_Action = splitActions(client_Action)

                    if err == 1 and x == 0:
                        dirrection, x, y = stuck(dirrection, client_Action, conn)
                    elif err == 1:
                        break

                print(f"RETURNUTY CORDS: X:{x} a Y:{y}")

            print("END WHILE")
            pickUp(client_Action, conn)

        except socket.timeout:
            print("\tSocket Timeout. Connection ended")
            conn.close()
            status = 0

        except:
            conn.close()
            status = 0


def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("127.0.0.1", 3999))
    server.listen(1)
    print("\n")

    while True:
        conn, client_adr = server.accept()
        conn.settimeout(1)
        # print("\n\nNOVY PRIPOJENI BOTA")
        # print(f"Device connected from ip: {str(client_adr)}")
        thr = threading.Thread(target=startCommunication, args=(conn,))
        thr.start()


if __name__ == "__main__":
    main()