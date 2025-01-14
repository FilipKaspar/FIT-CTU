<?php

namespace App\Controller\API;

use App\Entity\Account;
use App\Entity\Employee;
use App\Type\AccountType;
use Doctrine\ORM\EntityManagerInterface;
use FOS\RestBundle\Controller\AbstractFOSRestController;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

#[Route('/api')]
class RESTAccountController extends AbstractFOSRestController{

    #[Route('/employee/{id}/accounts', requirements: ['id' => '\d+'], methods: ['GET'])]
    public function getEmployeeAccounts(Employee $employee): Response
    {
        $accounts = $employee->getAccount();
        $view = $this->view($accounts, Response::HTTP_OK);

        return $this->handleView($view);
    }

    #[Route('/employee/{id}', requirements: ['id' => '\d+'], methods: ['POST'])]
    public function createAccount(Employee $employee, Request $request, EntityManagerInterface $entityManager): Response
    {
        $account = new Account();

        $form = $this->createForm(
            AccountType::class, $account, [
                'csrf_protection' => false,
        ])->submit($request->request->all(), true);

        if (!($form->isSubmitted() && $form->isValid()))
            return $this->handleView($this->view($request->request->all(), Response::HTTP_BAD_REQUEST));

        $account->setEmployee($employee);
        $entityManager->persist($account);
        $entityManager->flush();
        $view = $this->view($account, Response::HTTP_CREATED);

        return $this->handleView($view);
    }

    #[Route('/account/{id}', requirements: ['id' => '\d+'], methods: ['DELETE'])]
    public function deleteAccount(Account $account, EntityManagerInterface $entityManager): Response
    {
        $entityManager->remove($account);

        $entityManager->flush();
        $view = $this->view(null, Response::HTTP_NO_CONTENT);

        return $this->handleView($view);
    }
}