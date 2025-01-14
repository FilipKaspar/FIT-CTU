<?php

namespace App\Entity;

use App\Repository\EmployeeRepository;
use Doctrine\Common\Collections\ArrayCollection;
use Doctrine\Common\Collections\Collection;
use Doctrine\DBAL\Types\Types;
use Symfony\Component\Validator\Constraints as Assert;
use Doctrine\ORM\Mapping as ORM;

#[ORM\Entity(repositoryClass: EmployeeRepository::class)]
class Employee
{
    #[ORM\Id]
    #[ORM\GeneratedValue]
    #[ORM\Column]
    private ?int $id = null;

    #[ORM\Column(length: 255)]
    #[Assert\Length(max: 255)]
    private ?string $firstName = null;

    #[ORM\Column(length: 255)]
    #[Assert\Length(max: 255)]
    private ?string $lastName = null;

    #[ORM\ManyToMany(targetEntity: Role::class, inversedBy: "employees")]
    private Collection $positions;

    #[ORM\Column(length: 255)]
    #[Assert\Length(max: 255)]
    private ?string $telephone = null;

    #[ORM\Column(length: 255)]
    #[Assert\Email(
        message: 'The email {{ value }} is not a valid email.',
    )]
    private ?string $email = null;

    #[ORM\Column(length: 255)]
    #[Assert\Url]
    private ?string $webPage = null;

    #[ORM\Column(length: 255)]
    #[Assert\Length(max: 255)]
    private ?string $info = null;

    #[ORM\OneToMany(mappedBy: 'employee', targetEntity: Account::class)]
    private Collection $accounts;

    public function __construct()
    {
        $this->positions = new ArrayCollection();
        $this->accounts = new ArrayCollection();
    }


    public function getId(): ?int
    {
        return $this->id;
    }

    public function setId(string $id): static
    {
        $this->id = $id;

        return $this;
    }

    public function getFirstName(): ?string
    {
        return $this->firstName;
    }

    public function setFirstName(string $firstName): static
    {
        $this->firstName = $firstName;

        return $this;
    }

    public function getLastName(): ?string
    {
        return $this->lastName;
    }

    public function setLastName(string $lastName): static
    {
        $this->lastName = $lastName;

        return $this;
    }

    public function getName(): ?string{
        return $this->firstName . " " . $this->lastName;
    }

    public function getPositions(): Collection
    {
        return $this->positions;
    }

    public function addPositions(Role $position): static
    {
        if (!$this->positions->contains($position)) {
            $this->positions[] = $position;
            $position->addEmployee($this);
        }

        return $this;
    }

    public function getTelephone(): ?string
    {
        return $this->telephone;
    }

    public function setTelephone(string $telephone): static
    {
        $this->telephone = $telephone;

        return $this;
    }

    public function getEmail(): ?string
    {
        return $this->email;
    }

    public function setEmail(string $email): static
    {
        $this->email = $email;

        return $this;
    }

    public function getWebPage(): ?string
    {
        return $this->webPage;
    }

    public function setWebPage(string $webPage): static
    {
        $this->webPage = $webPage;

        return $this;
    }

    public function getInfo(): ?string
    {
        return $this->info;
    }

    public function setInfo(string $info): static
    {
        $this->info = $info;

        return $this;
    }

    public function getAccount(): Collection
    {
        return $this->accounts;
    }

    public function addAccount(Account $account): static
    {
        if (!$this->accounts->contains($account)) {
            $this->accounts[] = $account;
            $account->setEmployee($this);
        }

        return $this;
    }

}
