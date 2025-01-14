<?php declare (strict_types=1);

class Bag
{
    protected array $items = [];

    public function add(mixed $item): void
    {
        $this->items[] = $item;
    }

    public function clear(): void
    {
        $this->items = [];
    }

    public function contains(mixed $item): bool
    {
        $key = array_search($item, $this->items, true);

        if($key === false) return false;
        return true;
    }

    public function elementSize(mixed $item): int
    {
        $count = 0;
        foreach ($this->items as $_ => $value) {
            if ($value === $item) {
                $count++;
            }
        }
        return $count;
    }

    public function isEmpty(): bool
    {
        if(count($this->items) === 0) return true;
        return false;
    }

    public function remove(mixed $item): void
    {
        $key = array_search($item, $this->items, true);

        if ($key !== false) {
            unset($this->items[$key]);
        }
    }

    public function size(): int
    {
        return count($this->items);
    }
}
