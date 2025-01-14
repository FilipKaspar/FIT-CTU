<?php declare(strict_types=1);

namespace App\Model;

use App\Db;
use PDO;
use PHPUnit\Event\Runtime\PHP;

class Account
{
    public function __construct(
        protected int    $id,
        protected string $number,
        protected string $code
    )
    {
    }

    /**
     * Creates DB table using CREATE TABLE ...
     */
    public static function createTable(): void
    {
        $db = Db::get();
        $db->query('CREATE TABLE IF NOT EXISTS `accounts` (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            number VARCHAR(255),
            code VARCHAR(255)
        )');
    }

    /**
     * Drops DB table using DROP TABLE ...
     */
    public static function dropTable(): void
    {
        $db = Db::get();
        $db->query('DROP TABLE IF EXISTS `accounts`');
    }

    /**
     * Find account record by number and bank code
     */
    public static function find(string $number, string $code): ?self
    {
        $db = Db::get();
        $query_res = $db->query("SELECT * FROM accounts WHERE number = '$number' AND code = '$code'");
        $result = $query_res->fetchAll(5);

        if(!$result) return null;
        return new self($result[0]->id, $result[0]->number, $result[0]->code);
    }

    /**
     * Find account record by id
     */
    public static function findById(int $id): ?self
    {
        $db = Db::get();
        $query_res = $db->query("SELECT * FROM accounts WHERE id = '$id'");
        $result = $query_res->fetchAll(5);

        if(!$result) return null;
        return new self($result[0]->id, $result[0]->number, $result[0]->code);
    }

    /**
     * Inserts new account record and returns its instance; or returns existing account instance
     */
    public static function findOrCreate(string $number, string $code): self
    {
        $acc = self::find($number, $code);
        if($acc === null) {
            $acc = new self(0,$number, $code);

            $db = Db::get();
            $db->query("INSERT INTO accounts (number, code) VALUES ('$number', '$code')");
        }
        return $acc;
    }

    /**
     * Returns iterable of Transaction instances related to this Account, consider both transaction direction
     *
     * @return iterable<Transaction>
     */
    public function getTransactions(): iterable
    {
        $db = Db::get();
        $query_res = $db->query("SELECT * FROM transactions WHERE account_from = '$this->id' OR account_to = '$this->id'");

        $trans = [];
        foreach ($query_res->fetchAll(5) as $q){
            $trans[] = new Transaction(self::findById((int)$q->account_from), self::findById((int)$q->account_to), $q->amount);
        }

//        print_r($trans);
//        sleep(1);

        return $trans;
    }

    /**
     * Returns transaction sum (using SQL aggregate function). Treat outgoing transactions as 'minus' and incoming as 'plus'.
     */
    public function getTransactionSum(): float
    {
        $total = 0;
        foreach(self::getTransactions() as $transaction){
            if($transaction->getFrom()->getId() === $this->getId()) $total -= $transaction->getAmount();
            else $total += $transaction->getAmount();
        }

        return $total;
    }

    public function getId(): int
    {
        return $this->id;
    }

    public function setId(int $id): Account
    {
        $this->id = $id;
        return $this;
    }

    public function getNumber(): string
    {
        return $this->number;
    }

    public function setNumber(string $number): Account
    {
        $this->number = $number;
        return $this;
    }

    public function getCode(): string
    {
        return $this->code;
    }

    public function setCode(string $code): Account
    {
        $this->code = $code;
        return $this;
    }

    public function __toString(): string
    {
        return "{$this->number}/{$this->code}";
    }
}
