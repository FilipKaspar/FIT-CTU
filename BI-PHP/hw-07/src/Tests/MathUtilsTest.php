<?php declare(strict_types=1);

namespace HW\Tests;

use HW\Factory\MathUtilsFactory;
use PHPUnit\Framework\TestCase;

class MathUtilsTest extends TestCase
{
    private function getMathUtils() {
        return MathUtilsFactory::get();
    }

    /**
     * @dataProvider dataSum
     */
    public function testSum(array $input, int $res) {
        $this->assertSame($res, $this->getMathUtils()->sum($input));
    }
    public function dataSum() : array {
        return [
            'Empty' => [ [], 0 ],
            'One' => [ [7], 7 ],
            'Multiple' => [ [5, 80, -75], 10 ],
            'Negative' => [ [-10, 7, 8, -3], 2 ],
            'All Negative' => [ [-1, -1, -1, -1], -4]
        ];
    }

    public function testSolveLinearInvalid() {
        $this->expectException(\InvalidArgumentException::class);
        $this->getMathUtils()->solveLinear(0, 20);
    }

    /**
     * @dataProvider dataSolveLinear
     */
    public function testSolveLinear(int $a, int $b, int $res){
        $this->assertSame($res, $this->getMathUtils()->solveLinear($a, $b));
    }

    public function dataSolveLinear() : array {
        return [
            'Positive' => [ 1, 7, -7 ],
            'Negative' => [ 1, -7, 7 ],
            'Zero' => [ 1, 0, 0 ],
            'Bigger x' => [ -2, 42, 21 ],
            'All Negative' => [ -5, -25, -5]
        ];
    }

    public function testSolveQuadraticInvalid() {
        $this->expectException(\InvalidArgumentException::class);
        $this->getMathUtils()->solveQuadratic("", 10, 30);
    }

    /**
     * @dataProvider dataSolveQuadratic
     */
    public function testSolveQuadratic(int $a, int $b, int $c, array $res){
        $solved = $this->getMathUtils()->solveQuadratic($a, $b, $c);
        $this->assertSame(count($res), count($solved));
        foreach ($res as $x){
            $this->assertContainsEquals($x, $solved);
        }
    }

    public function dataSolveQuadratic() : array {
        return [
            'Positive' => [ 1, 0, -16, [4, -4] ],
            'Bigger x' => [ 1, 3, -4, [1, -4] ],
            'Bigger x^2' => [ 2, 12, 16, [-2, -4] ],
            'Negative x^2' => [ -1, 1, 2, [-1, 2] ],
            'Negative determinant' => [ 1, 0, 1, []],
            'Zero x^2' => [0, 1, 7, [-7]]
        ];
    }
}
