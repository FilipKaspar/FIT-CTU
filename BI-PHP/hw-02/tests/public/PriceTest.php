<?php declare(strict_types=1);

namespace CVUT\PHP\tests;

use function CVUT\PHP\HW\getPrice;
use PHPUnit\Framework\TestCase;

class PriceTest extends TestCase
{
    private const ITEM_1 = 'Rohlík 5Kč';
    private const ITEM_2 = 'CZK400 Knížka';
    private const ITEM_3 = 'Máslo 49,00 Kč';
    private const ITEM_4 = 'CZK 1.600,59 Natural 95';
    private const ITEM_5 = 'Herní konzole 4.900 CZK';
    private const ITEM_6 = 'CZK 8,90 Mouka hladká';
    private const ITEM_7 = 'Mouka polohladká - vagon 9.900,70 Kč \n';


    public function testPriceConversion(): void
    {
        $this->assertSame(5.0, getPrice(self::ITEM_1), 'Incorrect output');
        $this->assertSame(400.0, getPrice(self::ITEM_2), 'Incorrect output');
        $this->assertSame(49.0, getPrice(self::ITEM_3), 'Incorrect output');
        $this->assertSame(1600.59, getPrice(self::ITEM_4), 'Incorrect output');
        $this->assertSame(4900.0, getPrice(self::ITEM_5), 'Incorrect output');
        $this->assertSame(8.9, getPrice(self::ITEM_6), 'Incorrect output');
        $this->assertSame(9900.70, getPrice(self::ITEM_7), 'Incorrect output');
    }
}
