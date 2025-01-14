<?php declare(strict_types=1);

namespace HW\Tests;

use HW\Factory\UserServiceFactory;
use HW\Interfaces\IStorage;
use HW\Interfaces\IUserService;
use PHPUnit\Framework\TestCase;

class UserServiceTest extends TestCase
{
    private function getUserService(IStorage $storage) {
        return UserServiceFactory::get($storage);
    }
    private IUserService $userService;

    public function setUp(): void {
        $storage = $this->createStub(IStorage::class);
        $storage->method('get')->willReturnMap([
            ['correct', json_encode(["username" => "filip", "email" => "filip@mail.com"])],
            ['wrong', json_encode(["username" => 12, "email" => 564])],
            ['empty', null]
        ]);
        $this->userService = $this->getUserService($storage);
    }

    public function testUserServiceInvalid1() {
        $this->expectException(\InvalidArgumentException::class);
        $this->userService->createUser(1, 'test');
    }

    public function testUserServiceInvalid2() {
        $this->expectException(\JsonException::class);
        $this->userService->getEmail('wrong');
    }

    public function testUserServiceInvalid3() {
        $this->expectException(\JsonException::class);
        $this->userService->getUsername('wrong');
    }

    public function testUserServiceInvalid4() {
        $this->expectException(\InvalidArgumentException::class);
        $this->userService->getUsername(5);
    }

    public function testUserServiceInvalid5() {
        $this->expectException(\InvalidArgumentException::class);
        $this->userService->createUser('filip', 'filipmail.com');
    }

    public function testUserServiceCorrect() {
        $this->userService->createUser('filip', 'filip@mail.com');
        $this->assertSame(null, $this->userService->getUsername('empty'));
        $this->assertSame(null, $this->userService->getEmail('empty'));
        $this->assertSame('filip', $this->userService->getUsername('correct'));
        $this->assertSame('filip@mail.com', $this->userService->getEmail('correct'));
    }

}
