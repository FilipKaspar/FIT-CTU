<?php declare(strict_types=1);

namespace Iterator;

use Node;
use PHPUnit\Event\Runtime\PHP;

abstract class AbstractOrderIterator implements \Iterator
{
    protected array $prev = [];
    protected Node $root;
    protected ?Node $current;

    public function __construct(Node $root)
    {
        $this->root = $root;
        $this->current = $root;
    }

    public function current(): ?Node
    {
//        echo "VYPIS: " . $this->current->getValue() . PHP_EOL;
        return $this->current;
    }

    public function next(): void
    {
    }

    public function key(): bool|int|float|string|null
    {
        return null;
    }

    public function valid(): bool
    {
        if(!empty($this->stack) || $this->current !== null){
            return true;
        }
        return false;
    }

    public function rewind(): void
    {
    }

    public function getLeftMostNode(Node $node) : Node {
        while($node !== null){
            $this->prev[] = $node;
            $node = $node->getLeft();
        }

        return array_pop($this->prev);
    }
}
