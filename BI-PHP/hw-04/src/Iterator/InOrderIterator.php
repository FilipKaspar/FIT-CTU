<?php declare(strict_types=1);

namespace Iterator;

use Node;

class InOrderIterator extends AbstractOrderIterator
{
    public function next(): void
    {
        if($this->current->getRight() !== null){
            $this->current = $this->getLeftMostNode($this->current->getRight());
        }
        else {
            $this->current = array_pop($this->prev);
        }
    }
    public function rewind(): void
    {
        $this->prev = [];
        $this->current = $this->getLeftMostNode($this->root);
    }
}
