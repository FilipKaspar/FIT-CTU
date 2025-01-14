<?php declare(strict_types=1);

namespace CVUT\PHP\tests;

use PHPUnit\Framework\TestCase;
use function CVUT\PHP\HW\sumList;

class TotalPriceTest extends TestCase
{
    protected const LIST_1 = [
        'Rohlík 5Kč',
        'CZK400 Knížka',
        'Pivo 42,-',
        'Houska 4 Kč',
        'Máslo 49,00 Kč',
    ];
    protected const LIST_2 = [
        'Máslo 49,30 Kč',
        'Pivo 42,-',
        'Rohlík 5Kč',
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
    protected const LIST_4 = [
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
    public function testSumPrice(): void
    {
        $this->assertSame(500.0, sumList(self::LIST_1));
        $this->assertSame(96.3, sumList(self::LIST_2));
        $this->assertSame(7550.59, sumList(self::LIST_3));
        $this->assertSame(14601.18, sumList(self::LIST_4));
    }
}
