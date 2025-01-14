<?php declare(strict_types=1);

namespace Iterator;

use Node;

class PostOrderIterator extends AbstractOrderIterator
{
    private array $visited = [];

    public function next(): void
    {
        $this->findDeepestChildren();
    }
    public function rewind(): void
    {
        $this->current = $this->root;
        $this->prev = [$this->current];
        $this->visited = [];
        $this->findDeepestChildren();
    }

    public function valid(): bool{
        $tmp = $this->prev;
        array_pop($this->prev);
        return !empty($tmp);
    }

    public function findDeepestChildren(): void
    {
        while (true) {
            if(end($this->prev) === false) $this->current = null;
            else $this->current = end($this->prev);

            if (empty($this->prev)) return;
            if ($this->current->getRight() !== null && !in_array($this->current->getRight(), $this->visited)) {
                $this->prev[] = $this->current->getRight();
            }
            if ($this->current->getLeft() !== null && !in_array($this->current->getLeft(), $this->visited)) {
                $this->prev[] = $this->current->getLeft();
            }
            if (($this->current->getLeft() === null || in_array($this->current->getLeft(), $this->visited))
                && ($this->current->getRight() === null || in_array($this->current->getRight(), $this->visited))) {
                $this->visited[] = $this->current;
                break;
            }
        }
    }
}
