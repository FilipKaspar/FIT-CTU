<?php

namespace App\Controller;

use App\Entity\Account;
use App\Entity\Employee;
use App\Entity\Role;
use App\Repository\EmployeeRepository;
use App\Type\AccountType;
use App\Type\EmployeeType;
use Doctrine\ORM\EntityManagerInterface;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
//use Symfony\Component\BrowserKit\Request;
use Symfony\Component\HttpClient\HttpClient;
use Symfony\Component\Routing\Annotation\Route;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;

class Router extends AbstractController {
    private EmployeeRepository $employeeRepository;

    public function __construct(EmployeeRepository $employeeService)
    {
        $this->employeeRepository = $employeeService;
    }

    #[Route('/delete_data', name: '/delete_data')]
    public function delete_data(EntityManagerInterface $entityManager) {
        foreach ($entityManager->getRepository(Account::class)->findAll() as $account){
            $entityManager->remove($account);
        }

        foreach ($entityManager->getRepository(Role::class)->findAll() as $role){
            $entityManager->remove($role);
        }

        foreach ($entityManager->getRepository(Employee::class)->findAll() as $employee){
            $entityManager->remove($employee);
        }

        $entityManager->flush();

        return new Response("<html><body>Data has been deleted</body></html>");
    }

    #[Route('/upload_data', name: 'upload_data')]
    public function upload_data(EntityManagerInterface $entityManager) {
        $to_load_roles = [];
        $to_load_accounts = [];
        $to_load_employees = [];
        $to_load_roles[] = (new Role())->setTitle("CEO")->setDescription("head");
        $to_load_roles[] = (new Role())->setTitle("Uklízeč")->setDescription("bottom");
        $to_load_roles[] = (new Role())->setTitle("IT support")->setDescription("IT god");
        $to_load_roles[] = (new Role())->setTitle("Recepční")->setDescription("mrs. Carol");
        $to_load_roles[] = (new Role())->setTitle("Project manager")->setDescription("Big Brain");
        $to_load_roles[] = (new Role())->setTitle("Back office")->setDescription("Noice");

        $to_load_accounts[] = (new Account())->setName("NiceAccount")->setType("karta")->setExpiration("permanent");
        $to_load_accounts[] = (new Account())->setName("EvenNicerAccount")->setType("username/password")->setExpiration("temporary");
        $to_load_accounts[] = (new Account())->setName("BruhAccount")->setType("karta")->setExpiration("temporary");
        $to_load_accounts[] = (new Account())->setName("PoggersAccount")->setType("username/password")->setExpiration("permanent");
        $to_load_accounts[] = (new Account())->setName("Merica account")->setType("karta")->setExpiration("temporary");
        $to_load_accounts[] = (new Account())->setName("Vágní účet")->setType("username/password")->setExpiration("permanent");

        $to_load_employees[] =(new Employee())->setFirstName("Petr")->setLastName("Novak")->setTelephone("132465789")
            ->addPositions($to_load_roles[0])->addPositions($to_load_roles[1])->setEmail("petr.novak@madeupmail.com")
            ->setWebPage("https://testpage.com")->setInfo("Nadšenec do všeho, hlavně krav. Mám rád krávy. Bůů")
            ->addAccount($to_load_accounts[0]);
        $to_load_employees[] =(new Employee())->setFirstName("Jitka")->setLastName("Smělá")->setTelephone("987654321")
            ->addPositions($to_load_roles[1])->addPositions($to_load_roles[2])->setEmail("jitka.smela@madeupmail.com")
            ->setWebPage("https://testpage1.com")->setInfo("IT support jak má být")
            ->addAccount($to_load_accounts[1]);
        $to_load_employees[] =(new Employee())->setFirstName("Vojta")->setLastName("Dráždil")->setTelephone("123789456")
            ->addPositions($to_load_roles[3])->setEmail("vojta.drazdil@madeupmail.com")
            ->setWebPage("https://testpage2.com")->setInfo("Hodně hezká recepční")
            ->addAccount($to_load_accounts[2]);
        $to_load_employees[] =(new Employee())->setFirstName("Radka")->setLastName("Krutá")->setTelephone("456123789")
            ->addPositions($to_load_roles[4])->setEmail("radka.kruta@madeupmail.com")
            ->setWebPage("https://testpage3.com")->setInfo("Crazy manager")
            ->addAccount($to_load_accounts[3])->addAccount($to_load_accounts[4]);
        $to_load_employees[] =(new Employee())->setFirstName("Ivan")->setLastName("HoDostal")->setTelephone("789456123")
            ->addPositions($to_load_roles[5])->setEmail("ivan.hodostal@madeupmail.com")
            ->setWebPage("https://testpage4.com")->setInfo("Back office banger")
            ->addAccount($to_load_accounts[5]);

        for($i = 0;;$i++){
            if($i >= count($to_load_roles) && $i >= count($to_load_accounts) && $i >= count($to_load_employees))break;
            if($i < count($to_load_roles)) $entityManager->persist($to_load_roles[$i]);
            if($i < count($to_load_accounts)) $entityManager->persist($to_load_accounts[$i]);
            if($i < count($to_load_employees)) $entityManager->persist($to_load_employees[$i]);
        }
        $entityManager->flush();

        return new Response("<html><body>Data has been uploaded</body></html>");
    }

    #[Route('/index', name: 'index')]
    public function index(EntityManagerInterface $entityManager) {
        return $this->render( 'pages/index.twig', array(
            'employees' => $entityManager->getRepository(Employee::class)->findAll()
        ) );
    }

    #[Route('/employee_detail/{id}', name: 'employee_detail')]
    public function employee_detail(Request $request,$id, EntityManagerInterface $entityManager) {
        $id = (int) $id;
//        if($this->checkBounds($id, count($entityManager->getRepository(Employee::class)->findAll())))
//            return $this->render( 'pages/fail_page.twig');

        return $this->render( 'pages/employee_detail.twig', array(
            'emp' => $entityManager->getRepository(Employee::class)->findOneBy(array('id' => $id))
        ) );
    }

    #[Route('/employee_accounts_detail/{id}', name: 'employee_accounts_detail')]
    public function employee_accounts_detail($id, EntityManagerInterface $entityManager) {
        $id = (int) $id;
//        if($this->checkBounds($id, count($entityManager->getRepository(Employee::class)->findAll())))
//            return $this->render( 'pages/fail_page.twig');

        return $this->render( 'pages/employee_accounts_detail.twig', array(
            'emp' => $entityManager->getRepository(Employee::class)->findOneBy(array('id' => $id))
        ) );
    }

    #[Route('/fail_page', name: 'fail_page')]
    public function fail_page() {
        return $this->render( 'pages/fail_page.twig');
    }

    #[Route('/all_employees', name: 'all_employees')]
    public function all_employees(EntityManagerInterface $entityManager) {
        return $this->render( 'pages/all_employees.twig', array(
            'employees' => $entityManager->getRepository(Employee::class)->findAll()
        ) );
    }

    #[Route('/search_result', name: 'search_result')]
    public function search_result(Request $request, EntityManagerInterface $entityManager) {
        $input = $request->request->get('search', '');
        return $this->render( 'pages/search_result.twig', array(
            'employees' => $this->employeeRepository->findByAnything($input)
        ) );
    }

    #[Route('/create_edit_user/{id}', name: 'create_edit_user')]
    public function create_edit_user(Request $request, EntityManagerInterface $entityManager,int $id) {
        if($id !== -1) $employee = $entityManager->getRepository(Employee::class)->findOneBy(array('id' => $id));
        else $employee = new Employee();
        $form = $this->createForm(EmployeeType::class, $employee);

        $form->handleRequest($request);
        if ($form->isSubmitted() && $form->isValid()) {
            if($id !== -1 && $form->get('Smazat')->isClicked()){
                foreach ($employee->getAccount() as $acc){
                    $entityManager->remove($acc);
                }
                $entityManager->remove($employee);
                return $this->redirectToRoute('all_employees');
            }
            else $entityManager->persist($employee);
            $entityManager->flush();
            return $this->redirectToRoute('employee_detail', ['id' => $employee->getId()]);
        }
        return $this->render( 'pages/create_user.twig', array(
            "form" => $form
        ) );
    }

    #[Route('/create_edit_account/{id}', name: 'create_edit_account')]
    public function create_edit_account(Request $request, EntityManagerInterface $entityManager, int $id) {
        if($id !== -1) $account = $entityManager->getRepository(Account::class)->findOneBy(array('id' => $id));
        else $account = new Account();
        $form = $this->createForm(AccountType::class, $account);

        $form->handleRequest($request);
        if ($form->isSubmitted() && $form->isValid()) {
            if($id !== -1 && $form->get('Smazat')){
                $entityManager->persist($account);
                $entityManager->flush();
            }

            return $this->redirectToRoute('employee_accounts_detail', ['id' => $account->getEmployee()->getId()]);
        }

        return $this->render( 'pages/create_account.twig', array(
            "form" => $form
        ) );
    }

    public function checkBounds(int $id, int $max): bool{
        if($id <= 0 || $id > $max){
            return true;
        }
        return false;
    }
}
