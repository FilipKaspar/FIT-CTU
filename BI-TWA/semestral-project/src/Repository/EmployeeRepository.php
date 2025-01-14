<?php

namespace App\Repository;

use App\Entity\Employee;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Doctrine\Persistence\ManagerRegistry;

/**
 * @extends ServiceEntityRepository<Employee>
 *
 * @method Employee|null find($id, $lockMode = null, $lockVersion = null)
 * @method Employee|null findOneBy(array $criteria, array $orderBy = null)
 * @method Employee[]    findAll()
 * @method Employee[]    findBy(array $criteria, array $orderBy = null, $limit = null, $offset = null)
 */
class EmployeeRepository extends ServiceEntityRepository
{
    public function __construct(ManagerRegistry $registry)
    {
        parent::__construct($registry, Employee::class);
    }

    public function findByAnything(string $query): array
    {
        return $this->createQueryBuilder('e')
            ->leftJoin('e.positions', 'r')
            ->andWhere('e.firstName LIKE :val OR
						e.lastName LIKE :val OR
						CONCAT(e.firstName, \' \', e.lastName) LIKE :val OR
						e.telephone LIKE :val OR
						e.email LIKE :val OR
						e.webPage LIKE :val OR
						e.info LIKE :val OR
                        r.title LIKE :val OR
                        r.description LIKE :val')
            ->setParameter('val', '%' . $query . '%')
            ->addOrderBy('e.lastName', 'ASC')
            ->addOrderBy('e.firstName', 'ASC')
            ->getQuery()
            ->getResult();
    }
}
