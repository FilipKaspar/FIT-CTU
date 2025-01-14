<?php declare(strict_types=1);

namespace HW\Lib;

use HW\Interfaces\ILinkedList;
use HW\Interfaces\ILinkedListItem;

class LinkedList implements ILinkedList
{
    protected ?ILinkedListItem $first = null;

    protected ?ILinkedListItem $last = null;

    public function getFirst(): ?ILinkedListItem
    {
        return $this->first;
    }

    public function setFirst(?ILinkedListItem $first): LinkedList
    {
        $this->first = $first;
        return $this;
    }

    public function getLast(): ?ILinkedListItem
    {
        return $this->last;
    }

    public function setLast(?ILinkedListItem $last): LinkedList
    {
        $this->last = $last;
        return $this;
    }

    /**
     * Place new item at the begining of the list
     */
    public function prependList(string $value): ILinkedListItem
    {
        $item = new LinkedListItem($value);
        $second = $this->getFirst();
        $this->setFirst($item);
        $item->setNext($second);
        if($second !== null) $second->setPrev($item);
        else $this->setLast($item);

        return $item;
    }

    /**
     * Place new item at the end of the list
     */
    public function appendList(string $value): ILinkedListItem
    {
        $item = new LinkedListItem($value);
        $penultimate = $this->getLast();
        $this->setLast($item);
        $item->setPrev($penultimate);
        if($penultimate !== null) $penultimate->setNext($item);
        else $this->setFirst($item);

        return $item;
    }

    /**
     * Insert item before $nextItem and maintain continuity
     */
    public function prependItem(ILinkedListItem $nextItem, string $value): ILinkedListItem
    {
        $item = new LinkedListItem($value);
        $item->setPrev($nextItem->getPrev());
        $item->setNext($nextItem);
        $nextItem->setPrev($item);

        if($item->getPrev() !== null) $item->getPrev()->setNext($item);
        if($this->first === $nextItem) $this->first = $item;
        return $item;
    }

    /**
     * Insert item after $prevItem and maintain continuity
     */
    public function appendItem(ILinkedListItem $prevItem, string $value): ILinkedListItem
    {
        $item = new LinkedListItem($value);
        $item->setPrev($prevItem);
        $item->setNext($prevItem->getNext());
        $prevItem->setNext($item);

        if($item->getNext() !== null) $item->getNext()->setPrev($item);
        if($this->last === $prevItem) $this->last = $item;

        return $item;
    }
}
