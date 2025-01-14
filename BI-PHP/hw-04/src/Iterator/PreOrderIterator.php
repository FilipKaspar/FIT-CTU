<?php declare(strict_types=1);

namespace Iterator;

use Node;

class PreOrderIterator extends AbstractOrderIterator
{
    public function next(): void
    {
        if ($this->current->getRight() !== null) {
            $this->prev[] = $this->current->getRight();
        }
        if ($this->current->getLeft() !== null) {
            $this->prev[] = $this->current->getLeft();
        }

//        echo "[";
//        foreach ($this->prev as $v){
//            echo $v->getValue() . ", ";
//        }
//        echo "]" . PHP_EOL;

        $this->current = array_pop($this->prev);
    }
    public function rewind(): void
    {
        $this->prev = [];
        $this->current = $this->root;
    }
}
