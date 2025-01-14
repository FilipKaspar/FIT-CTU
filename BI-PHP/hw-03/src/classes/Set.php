<?php

class Set extends Bag {

    public function add(mixed $item): void
    {
        if(!$this->elementSize($item)) $this->items[] = $item;
    }

    public function elementSize(mixed $item): int
    {
        $key = array_search($item, $this->items, true);

        if ($key !== false) {
            return 1;
        }
        return 0;
    }
}