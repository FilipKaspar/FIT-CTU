<?php declare(strict_types=1);

namespace HW\Tests;

use HW\Factory\LinkedListFactory;
use HW\Lib\LinkedList;
use PHPUnit\Framework\TestCase;

class LinkedListItemTest extends TestCase
{
    protected LinkedList $list;

    public function setUp(): void
    {
        parent::setUp();
        $this->list = LinkedListFactory::get();
    }

    public function testPrependAppend1 () {
        $this->assertNull($this->list->getFirst());
        $this->assertNull($this->list->getLast());

        $link1 = $this->list->prependList("1"); // 1
        $this->assertTrue($link1 === $this->list->getFirst());
        $this->assertTrue($link1 === $this->list->getLast());

        $link2 = $this->list->prependList("2"); // 2 - 1
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link1 === $this->list->getLast());

        $link3 = $this->list->appendList("3"); // 2 - 1 - 3
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link3 === $this->list->getLast());

        $link4 = $this->list->appendList("4"); // 2 - 1 - 3 - 4
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link4 === $this->list->getLast());

        $link5 = $this->list->prependItem($link2, "5"); // 5 - 2 - 1 - 3 - 4
        $this->assertTrue($link5 === $this->list->getFirst());
        $this->assertTrue($link4 === $this->list->getLast());

        $this->assertTrue(null === $link5->getPrev());
        $this->assertTrue($link2 === $link5->getNext());
        $this->assertTrue($link5 === $link2->getPrev());
        $this->assertTrue($link1 === $link2->getNext());
        $this->assertTrue($link2 === $link1->getPrev());
        $this->assertTrue($link3 === $link1->getNext());
        $this->assertTrue($link1 === $link3->getPrev());
        $this->assertTrue($link4 === $link3->getNext());
        $this->assertTrue($link3 === $link4->getPrev());
        $this->assertTrue(null === $link4->getNext());

    }

    public function testPrependAppend2 () {
        $this->assertNull($this->list->getFirst());
        $this->assertNull($this->list->getLast());

        $link1 = $this->list->appendList("1"); // 1
        $this->assertTrue($link1 === $this->list->getFirst());
        $this->assertTrue($link1 === $this->list->getLast());

        $link2 = $this->list->prependList("2"); // 2 - 1
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link1 === $this->list->getLast());

        $link3 = $this->list->appendItem($link2, "3"); // 2 - 3 - 1
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link1 === $this->list->getLast());

        $link4 = $this->list->appendItem($link1, "4"); // 2 - 3 - 1 - 4
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link4 === $this->list->getLast());

        $link5 = $this->list->prependItem($link1, "5"); // 2 - 3 - 5 - 1 - 4
        $this->assertTrue($link2 === $this->list->getFirst());
        $this->assertTrue($link4 === $this->list->getLast());


        $this->assertTrue(null === $link2->getPrev());
        $this->assertTrue($link3 === $link2->getNext());
        $this->assertTrue($link2 === $link3->getPrev());
        $this->assertTrue($link5 === $link3->getNext());
        $this->assertTrue($link3 === $link5->getPrev());
        $this->assertTrue($link1 === $link5->getNext());
        $this->assertTrue($link5 === $link1->getPrev());
        $this->assertTrue($link4 === $link1->getNext());
        $this->assertTrue($link1 === $link4->getPrev());
        $this->assertTrue(null === $link4->getNext());

        $this->assertSame('5', $link5->getValue());
        $link5->setValue('50');
        $this->assertSame('50', $link5->getValue());
    }
}
