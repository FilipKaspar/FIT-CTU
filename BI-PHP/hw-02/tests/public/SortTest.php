<?php declare(strict_types=1);

namespace CVUT\PHP\tests;

use PHPUnit\Framework\TestCase;
use function CVUT\PHP\HW\sortList;

class SortTest extends TestCase
{
    protected const LIST_1 = [
        'Rohlík 5Kč',
        'CZK400 Knížka',
        'Pivo 42,-',
        'Houska 4 Kč',
        'Máslo 49,00 Kč',
    ];
    protected const LIST_2 = [
        'Máslo 49,00 Kč',
        'CZK400 Knížka',
        'Houska 4 Kč',
        'Pivo 42,-',
        'Rohlík 5Kč',
    ];
    protected const RESULT_12 = [
        'Houska 4 Kč',
        'Rohlík 5Kč',
        'Pivo 42,-',
        'Máslo 49,00 Kč',
        'CZK400 Knížka',
    ];


    protected const LIST_3 = [
        'Rádio CZK550',
        'CZK 1.600,59 Natural 95',
        'Herní konzole 4.900 CZK',
        'Máslo 49,00 Kč',
        'CZK400 Knížka',
        'Houska 4 Kč',
        'Pivo 42,-',
        'Rohlík 5Kč',
    ];
    protected const LIST_DUPLICATED = [
        'Máslo 49,00 Kč',
        'CZK400 Knížka',
        'Houska 4 Kč',
        'Pivo 42,-',
        'Rohlík 5Kč',
        'Rádio CZK550',
        'CZK 1.600,59 Natural 95',
        'Herní konzole 4.900 CZK',
        'Rádio CZK550',
        'CZK 1.600,59 Natural 95',
        'Herní konzole 4.900 CZK',
    ];
    protected const RESULT_3 = [
        'Houska 4 Kč',
        'Rohlík 5Kč',
        'Pivo 42,-',
        'Máslo 49,00 Kč',
        'CZK400 Knížka',
        'Rádio CZK550',
        'CZK 1.600,59 Natural 95',
        'Herní konzole 4.900 CZK',
    ];
    protected const RESULT_DUPLICATED = [
        'Houska 4 Kč',
        'Rohlík 5Kč',
        'Pivo 42,-',
        'Máslo 49,00 Kč',
        'CZK400 Knížka',
        'Rádio CZK550',
        'Rádio CZK550',
        'CZK 1.600,59 Natural 95',
        'CZK 1.600,59 Natural 95',
        'Herní konzole 4.900 CZK',
        'Herní konzole 4.900 CZK',
    ];

    public function testSorting(): void
    {
        $this->assertEquals(self::RESULT_12, sortList(self::LIST_1), 'Sort is invalid');
        $this->assertEquals(self::RESULT_12, sortList(self::LIST_2), 'Sort is invalid');
        $this->assertEquals(self::RESULT_3, sortList(self::LIST_3), 'Sort is invalid');
        $this->assertEquals(self::RESULT_DUPLICATED, sortList(self::LIST_DUPLICATED), 'Sort duplicated items is invalid');
    }
}
