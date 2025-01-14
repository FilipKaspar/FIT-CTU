<?php declare(strict_types=1);

namespace App;

use App\Invoice\Address;
use App\Invoice\BusinessEntity;
use App\Invoice\Item;

class Builder
{
    protected Invoice $invoice;

    public function __construct(){
        $this->invoice = new Invoice();
    }

    public function build(): Invoice
    {
        // TODO implement
        return $this->invoice;
    }

    public function setNumber(string $number): self
    {
        // TODO implement
        $this->invoice->setNumber($number);
        return $this;
    }

    public function setSupplier(
        string  $name,
        string  $vatNumber,
        string  $street,
        string  $number,
        string  $city,
        string  $zip,
        ?string $phone = null,
        ?string $email = null
    ): self
    {
        // TODO implement
        $address = new Address();
        $address->setStreet($street);
        $address->setNumber($number);
        $address->setCity($city);
        $address->setZipCode($zip);
        $address->setPhone($phone);
        $address->setEmail($email);

        $business = new BusinessEntity();
        $business->setName($name);
        $business->setVatNumber($vatNumber);
        $business->setAddress($address);

        $this->invoice->setSupplier($business);
        return $this;
    }

    public function setCustomer(
        string  $name,
        string  $vatNumber,
        string  $street,
        string  $number,
        string  $city,
        string  $zip,
        ?string $phone = null,
        ?string $email = null
    ): self
    {
        // TODO implement
        $address = new Address();
        $address->setStreet($street);
        $address->setNumber($number);
        $address->setCity($city);
        $address->setZipCode($zip);
        $address->setPhone($phone);
        $address->setEmail($email);

        $business = new BusinessEntity();
        $business->setName($name);
        $business->setVatNumber($vatNumber);
        $business->setAddress($address);

        $this->invoice->setCustomer($business);
        return $this;
    }

    public function addItem(string $description, ?float $quantity, ?float $price): self
    {
        // TODO implement
        $item = new Item();
        $item->setDescription($description);
        $item->setQuantity($quantity);
        $item->setUnitPrice($price);

        $this->invoice->addItem($item);
        return $this;
    }
}
