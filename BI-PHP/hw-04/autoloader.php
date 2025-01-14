<?php declare(strict_types=1);

$autoloader = function (string $className): void
{
    echo $className. PHP_EOL;

    $path = str_replace("\\", DIRECTORY_SEPARATOR, $className);
    require_once __DIR__ . DIRECTORY_SEPARATOR . 'src/' . $path . '.php';
};

spl_autoload_register($autoloader);
