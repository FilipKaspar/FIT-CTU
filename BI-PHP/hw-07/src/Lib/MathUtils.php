<?php declare(strict_types=1);

namespace HW\Lib;

use HW\Interfaces\IMathUtils;
use InvalidArgumentException;

class MathUtils implements IMathUtils
{
    /**
     * Sum a list of numbers.
     */
    public function sum(array $list): int
    {
        $sum = 0;

        foreach ($list as $value) {
            if(!is_numeric($value)) throw new InvalidArgumentException();
            $sum += $value;
        }

        return $sum;
    }

    /**
     * Solve linear equation ax + b = 0.
     */
    public function solveLinear($a, $b): float|int
    {
        if (!is_numeric($a) || !is_numeric($b) || $a == 0) {
            throw new InvalidArgumentException();
        }

        return -$b / $a;
    }

    /**
     * Solve quadratic equation ax^2 + bx + c = 0.
     *
     * @return array Solution x1 and x2.
     */
    public function solveQuadratic($a, $b, $c): array
    {
        if (!is_numeric($a) || !is_numeric($b) || !is_numeric($c)) {
            throw new InvalidArgumentException();
        }

        $xa = [];
        if($a == 0){
            $xa [] = $this->solveLinear($b, $c);
            return $xa;
        }

        $d = sqrt(pow($b, 2) - 4 * $a * $c);
        if($d >= 0) $xa [] = (-$b + $d) / (2 * $a);
        if($d > 0) $xa [] = (-$b - $d) / (2 * $a);
        return $xa;
    }
}
