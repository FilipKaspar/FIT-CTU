import os
import pytest

import trade
import graph


def test_get_num_from_str():
    num = trade.getNumFromStr("14,567")
    assert num == 14567

    num = trade.getNumFromStr("14,567.158")
    assert num == 14567.158

    num = trade.getNumFromStr("1,145,567.789")
    assert num == 1145567.789


def test_get_money_from_price():
    user_criteria = trade.UserCriteria(1, 20000, -1)
    crypto = trade.CryptoInfo(user_criteria)
    crypto.money_invested = 10000
    crypto.money_left = 10000
    crypto.current_value = 30000
    crypto.avg_bought_values = 35000
    assert pytest.approx(crypto.getMoneyFromPrice(1), 0.01) == 18571.4285

    user_criteria = trade.UserCriteria(1, 20000, -1)
    crypto = trade.CryptoInfo(user_criteria)
    crypto.money_invested = 10000
    crypto.money_left = 8500
    crypto.current_value = 34000
    crypto.avg_bought_values = 15000
    assert pytest.approx(crypto.getMoneyFromPrice(1), 0.01) == 31166.66666

    user_criteria = trade.UserCriteria(1, 20000, -1)
    crypto = trade.CryptoInfo(user_criteria)
    crypto.money_invested = 10000
    crypto.money_left = 32000
    crypto.current_value = 45000
    crypto.avg_bought_values = 25000
    assert crypto.getMoneyFromPrice(1) == 50000


def test_update_crypto_values():
    user_criteria = trade.UserCriteria(1, 20000, -1)
    crypto = trade.CryptoInfo(user_criteria)
    crypto.money_invested = 300
    crypto.money_left = 750
    crypto.current_value = 0.000350
    crypto.avg_bought_values = 0.000157
    crypto.updateCryptoValues()
    assert crypto.money_invested == 675
    assert crypto.money_left == 375
    assert pytest.approx(crypto.avg_bought_values, 0.00001) == 0.00026422

    user_criteria = trade.UserCriteria(1, 20000, -1)
    crypto = trade.CryptoInfo(user_criteria)
    crypto.money_invested = 50
    crypto.money_left = 5
    crypto.current_value = 156.000350
    crypto.avg_bought_values = 175.000157
    crypto.updateCryptoValues()
    assert crypto.money_invested == 52.5
    assert crypto.money_left == 2.5
    assert pytest.approx(crypto.avg_bought_values, 0.00001) == 174.0954042


def test_check_date():
    if not os.path.exists("user_criteria.txt"):
        return 1

    user_criteria = trade.UserCriteria(1, 20000, "-1")
    trade.checkDate(user_criteria)
    assert os.path.exists("timeline.xlsx")
    assert os.path.exists("user_criteria.txt")

    user_criteria = trade.UserCriteria(2, 20000, "2022-06-07")
    trade.checkDate(user_criteria)
    assert os.path.exists("timeline.xlsx")
    assert os.path.exists("user_criteria.txt")


def test_get_crypto():
    user_criteria = trade.UserCriteria(1, 20000, "-1")
    assert trade.getCrypto(user_criteria) == "bitcoin"

    user_criteria = trade.UserCriteria(2, 20000, "2024-07-15")
    assert trade.getCrypto(user_criteria) == "bitcoin"

    user_criteria = trade.UserCriteria(3, 20000, "-1")
    assert trade.getCrypto(user_criteria) == "dogecoin"


def test_show_graph():
    assert graph.show(1)
