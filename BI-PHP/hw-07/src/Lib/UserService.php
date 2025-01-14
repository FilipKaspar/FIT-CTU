<?php declare(strict_types=1);

namespace HW\Lib;

use HW\Interfaces\IStorage;
use HW\Interfaces\IUserService;
use JsonException;

class UserService implements IUserService
{
    private IStorage $storage;

    public function __construct(IStorage $storage)
    {
        $this->storage = $storage;
    }

    /**
     * @param $username
     * @param $email
     * @return string
     * @throws JsonException
     */
    public function createUser($username, $email)
    {
        if(!is_string($username) || !is_string($email) || empty(trim($username)) || empty(trim($email))
           || !preg_match("/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/", $email)) throw new \InvalidArgumentException();
        $id = uniqid('', true);
        $this->storage->save($id, json_encode([
            'username' => $username,
            'email' => $email
        ], JSON_THROW_ON_ERROR));
        return $id;
    }

    /**
     * @throws JsonException
     */
    public function getUsername($id)
    {
        if(!is_string($id)) throw new \InvalidArgumentException();
        $user = $this->storage->get($id);
        if (!$user) {
            return null;
        }
        $decode = json_decode($user, true, 512, JSON_THROW_ON_ERROR)['username'];
        if(!is_string($decode)) throw new JsonException();
        return $decode;
    }

    /**
     * @throws JsonException
     */
    public function getEmail($id)
    {
        if(!is_string($id)) throw new \InvalidArgumentException();
        $user = $this->storage->get($id);
        if (!$user) {
            return null;
        }
        $decode = json_decode($user, true, 512, JSON_THROW_ON_ERROR)['email'];
        if(!is_string($decode)) throw new JsonException();
        return $decode;
    }
}
