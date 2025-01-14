<?php declare(strict_types=1);

class Node extends BaseNode implements IteratorAggregate
{
    public function getIterator(): \Iterator\InOrderIterator {
        return new \Iterator\InOrderIterator($this);
    }
}
