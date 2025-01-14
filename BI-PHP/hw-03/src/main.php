<?php declare (strict_types=1);

// TODO - Implement autoloader
$autoloader = function(string $class): void {
    require_once __DIR__ . "/" . "classes/" . $class . '.php';
};

spl_autoload_register($autoloader);