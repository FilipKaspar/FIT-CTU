<?php declare(strict_types=1);

namespace App;

use Dompdf\Dompdf;
use SebastianBergmann\CodeCoverage\Report\PHP;

class Renderer extends Dompdf
{
    public function makeHtml(Invoice $invoice): string
    {
        // TODO implement

        $contacts = "";
        $supplier = ["Tel: " . $invoice->getSupplier()->getAddress()->getPhone(), "Email: " . $invoice->getSupplier()->getAddress()->getEmail()];
        $customer = ["Tel: " . $invoice->getCustomer()->getAddress()->getPhone(), "Email: " . $invoice->getCustomer()->getAddress()->getEmail()];

        if($supplier[0] === "Tel: ") {
            array_shift($supplier);
            if($supplier[0] === "Email: ") $supplier[0] = "<br>";
            $supplier[] = "<br>";
        }

        if($supplier[1] === "Email: ") {
            $supplier[1] = "<br>";
        }

        if($customer[0] === "Tel: ") {
            array_shift($customer);
            if($customer[0] === "Email: ") $customer[0] = "<br>";
            $customer[] = "<br>";
        }

        if($customer[1] === "Email: ") {
            $customer[1] = "<br>";
        }

        for($i = 0; $i < count($supplier); $i++){
            if($supplier[$i] === "<br>" && $customer[$i] === "<br>"){
                continue;
            }
            $contacts .= "<tr>" . PHP_EOL;
            $contacts .= "<td>" . $supplier[$i] . "</td>" . PHP_EOL
                . "<td>" . $customer[$i] . "</td>" . PHP_EOL;
            $contacts .= "</tr>" . PHP_EOL;
        }

        echo $contacts;

        $items = "";

        foreach ($invoice->getItems() as $item){
            $items .= "<tr>" . PHP_EOL;
            $items .= "<td>" . $item->getDescription() . "</td>" . PHP_EOL
                . "<td style='text-align: right'>" . $item->getQuantity() . "</td>" . PHP_EOL
                . "<td style='text-align: right'>" . number_format($item->getUnitPrice(), 2, ",", " ") . "</td>" . PHP_EOL
                . "<td style='text-align: right'>" . number_format($item->getTotalPrice(), 2, ",", " ") . "</td>" . PHP_EOL;
            $items .= "</tr>" . PHP_EOL;
        }

        $total_price = number_format($invoice->getTotalPrice(), 2, ",", " ");

        return "<html>
<style>
.table1 {
    border: 1px solid black;
    border-collapse: collapse;
    width:100%;
}
.table1 >* td:first-child,.table1 >* th {
    border-right: 1px solid black;
    border-left: none;
    border-bottom: none;
    border-top: none
}
.table1 >* th, .table1 >* td {
    text-align: left;
    padding-left: 7px;
    width:50%;
}
.table2 {
    border: 1px solid black;
    border-collapse: collapse;
    width:100%;
} 

.table2 >* th, .table2 >* td{
    border: 1px solid black;
    border-collapse: collapse;
    text-align: left;
    padding: 7px;
}
</style>
<body style='margin: 3em'>

<p>FAKTURA – DOKLAD c. {$invoice->getNumber()}</p>

<table class='table1'>
  <tr>
    <th>Dodavatel</th>
    <th>Odberatel</th>
  </tr>
  <tr>
    <td><br></td>
    <td><br></td>
  </tr>
  <tr>
    <td>{$invoice->getSupplier()->getName()}</td>
    <td>{$invoice->getCustomer()->getName()}</td>
  </tr>
  <tr>
    <td>{$invoice->getSupplier()->getAddress()->getStreet()} {$invoice->getSupplier()->getAddress()->getNumber()}</td>
    <td>{$invoice->getCustomer()->getAddress()->getStreet()} {$invoice->getCustomer()->getAddress()->getNumber()}</td>
  </tr>
  <tr>
    <td>{$invoice->getSupplier()->getAddress()->getZipCode()} {$invoice->getSupplier()->getAddress()->getCity()}</td>
    <td>{$invoice->getCustomer()->getAddress()->getZipCode()} {$invoice->getCustomer()->getAddress()->getCity()}</td>
  </tr>
  <tr>
    <td><br></td>
    <td><br></td>
  </tr>
  <tr>
    <td>{$invoice->getSupplier()->getVatNumber()}</td>
    <td>{$invoice->getCustomer()->getVatNumber()}</td>
  </tr>
  <tr>
    <td><br></td>
    <td><br></td>
  </tr>
  {$contacts}
</table>

<br>

<table class='table2'>
  <tr>
    <th>Polozka</th>
    <th>Pocet m.j.</th>
    <th>Cena za m.j.</th>
    <th>Celkem</th>
  </tr>
  {$items}
  <tr>
    <th colspan='3'>Celkem</th>
    <th style='text-align: right'>{$total_price}</th>
  </tr>
</table>

</body>
</html>";
    }
    public function makePdf(Invoice $invoice): string
    {
        // TODO implement
        $dompdf = new Dompdf();
        $dompdf->loadHtml($this->makeHtml($invoice));
        $dompdf->render();
        return $dompdf->output();
    }
}
