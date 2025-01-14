<?php declare(strict_types=1);

namespace CVUT\PHP\HW;

function getPrice(string $item): float {
    $pattern = '/[0-9,\,,.]+ ?(CZK|Kč|,-)($| [^0-9])|(CZK|CZK )[0-9,\,,.]+/';
    preg_match($pattern, $item, $matches);

    $pattern_price = '/[0-9,r\,,.]+/';
    preg_match($pattern_price, $matches[0], $matches);

    $matches[0] = rtrim($matches[0], ",");

    $final = str_replace('.','',$matches[0]);
    $final = str_replace(',','.',$final);

    return (float)$final;
}

/**
 * @param string[] $list
 * @return string[]
 */
function sortList(array $list): array {
    usort($list, function ($a, $b){
        return (getPrice($a) <=> getPrice($b)); #floating hmmmm? :D
    });
    return $list;
}

/**
 * @param string[] $list
 */
function sumList(array $list): float {
    $sum = 0;
    foreach ($list as $value){
        $sum += getPrice($value);
    }
    return  $sum;
}

// this disables the CLI interface when PHPUnit, the automated testing framework, runs
// do not remove, otherwise automated testing will fail. Thanks!
if (!defined('PHPUNIT_COMPOSER_INSTALL')) {
    if (count($argv) !== 2) {
        echo "Usage: php shopping.php <input>" . PHP_EOL;
        exit(1);
    }
    $input = trim(file_get_contents(end($argv)));
    $list = explode(PHP_EOL, $input);
    $list = sortList($list);

//    $t = ["Rohlík 5Kč", "5Kč Rohlík", "CZK400 Knížka", "Pivo 42,-",
//        "Houska 4 Kč", "Máslo 49,00 Kč", "Herní konzole 4.900 CZK", "Rádio CZK550",
//        "CZK 1.600,59 Natural 95", "98 Natural 95 CZK 156", "Natural 95 156 Kč",
//        "Natural 95 156,-"];
//
//    foreach ($t as $value) {
//        getPrice($value);
//    }

    print_r($list);
    print_r(sumList($list) . PHP_EOL);
}
