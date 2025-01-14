<?php declare(strict_types=1);

const DEAD = '.';
const ALIVE = 'X';

function readInput($string) {
    $matrix = array();
    $tmp = array();

    foreach (mb_str_split($string) as $char){
        if ($char === PHP_EOL){
            array_push($matrix, $tmp);
            $tmp = array();
        }
        else {
            $tmp[] = $char;
        }
    }
    $matrix[] = $tmp;
    return $matrix;
}

function writeOutput($matrix) {
    $str_version = "";

    foreach ($matrix as $inner){
        foreach($inner as $value){
            $str_version .= $value;
        }
        $str_version .= PHP_EOL;
    }

    return $str_version;
}

function checkContent($matrix, $height, $width, $max_height, $max_width): bool {
    if($height < 0 || $width < 0 || $height > $max_height || $width > $max_width) return false;
    if($matrix[$height][$width] === '.') return false;
    return true;
}

function countAliveAround($matrix, $height, $width, $max_height, $max_width): int{
    $alive = 0;

    if(checkContent($matrix,$height - 1, $width, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height - 1, $width + 1, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height, $width + 1, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height + 1, $width + 1, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height + 1, $width, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height + 1, $width - 1, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height, $width - 1, $max_height, $max_width)) $alive++;
    if(checkContent($matrix,$height - 1, $width - 1, $max_height, $max_width)) $alive++;

    return $alive;
}


function gameStep($matrix) {
    $updated_matrix = $matrix;

    $max_height = sizeof($matrix) - 1;
    $max_width = sizeof($matrix[0]) - 1;

    $height = 0;
    foreach ($matrix as $inner){
        $width = 0;
        foreach($inner as $point){
            $alive = countAliveAround($matrix, $height, $width, $max_height, $max_width);

            if($matrix[$height][$width] === 'X' && ( $alive < 2 || $alive > 3) ){
                $updated_matrix[$height][$width] = '.';
            }
            if($matrix[$height][$width] === '.' && $alive === 3){
                $updated_matrix[$height][$width] = 'X';
            }

            $width++;
        }
        $height++;
    }

    return $updated_matrix;
}
