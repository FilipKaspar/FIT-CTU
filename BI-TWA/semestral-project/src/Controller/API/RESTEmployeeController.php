<?php

namespace App\Controller\API;

use App\Entity\Employee;
use App\Type\EmployeeType;
use Doctrine\ORM\EntityManagerInterface;
use FOS\RestBundle\Controller\AbstractFOSRestController;
use FOS\RestBundle\Controller\Annotations\RequestParam;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

#[Route('/api')]
class RESTEmployeeController extends AbstractFOSRestController{

    #[Route('/employees', methods: ['GET'])]
    public function getEmployees(EntityManagerInterface $entityManager): Response
    {
        $view = $this->view($entityManager->getRepository(Employee::class)->findAll(), Response::HTTP_OK);

        return $this->handleView($view);
    }

    #[Route('/employee/{id}', requirements: ['id' => '\d+'], methods: ['GET'])]
    public function getEmployee(Employee $employee): Response
    {
        $view = $this->view($employee, Response::HTTP_OK);

        return $this->handleView($view);
    }


    #[Route('/employee/{id}', requirements: ['id' => '\d+'], methods: ['PUT'])]
    public function editEmployee(Employee $employee, Request $request, EntityManagerInterface $entityManager): Response
    {
        $form = $this->createForm(
            EmployeeType::class, $employee, [
            'csrf_protection' => false,
        ])->submit($request->request->all(), false);

        if (!($form->isSubmitted() && $form->isValid()))
            return $this->handleView($this->view($request->request->all(), Response::HTTP_BAD_REQUEST));

        $entityManager->persist($employee);
        $entityManager->flush();
        $view = $this->view($employee, Response::HTTP_OK);

        return $this->handleView($view);
    }

    #[Route('/employee', methods: ['POST'])]
    public function createEmployee(Request $request, EntityManagerInterface $entityManager): Response
    {
        $employee = new Employee();

        $form = $this->createForm(
            EmployeeType::class, $employee, [
            'csrf_protection' => false,
        ])->submit($request->request->all(), true);

        if (!($form->isSubmitted() && $form->isValid()))
            return $this->handleView($this->view($request->request->all(), Response::HTTP_BAD_REQUEST));

        $entityManager->persist($employee);
        $entityManager->flush();
        $view = $this->view($employee, Response::HTTP_CREATED);

        return $this->handleView($view);
    }

    #[Route('/employee/{id}', requirements: ['id' => '\d+'], methods: ['DELETE'])]
    public function deleteEmployee(Employee $employee, EntityManagerInterface $entityManager): Response
    {
        foreach ($employee->getAccount() as $acc){
            $entityManager->remove($acc);
        }
        $entityManager->remove($employee);
        $entityManager->flush();
        $view = $this->view($employee, Response::HTTP_CREATED);

        return $this->handleView($view);
    }
}