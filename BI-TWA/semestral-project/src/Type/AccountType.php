<?php

namespace App\Type;

use App\Entity\Account;
use App\Entity\Employee;
use App\Entity\Role;
use Doctrine\ORM\EntityRepository;
use Doctrine\ORM\Mapping\Entity;
use Symfony\Bridge\Doctrine\Form\Type\EntityType;
use Symfony\Component\Form\AbstractType;
use Symfony\Component\Form\Extension\Core\Type\DateType;
use Symfony\Component\Form\Extension\Core\Type\SubmitType;
use Symfony\Component\Form\Extension\Core\Type\TextType;
use Symfony\Component\Form\FormBuilderInterface;
use Symfony\Component\OptionsResolver\OptionsResolver;

class AccountType extends AbstractType
{
    public function buildForm(FormBuilderInterface $builder, array $options): void
    {
        $builder
            ->add('name', TextType::class)
            ->add('type', TextType::class)
            ->add('expiration', TextType::class)
            ->add('employee', EntityType::class, [
                'class' => Employee::class,
                'choice_label' => function (Employee $employee) {
                    return $employee->getName();
                },
                'attr' => ['onclick' => 'toggleDropdown()']
            ])
            ->add('Save', SubmitType::class);
        if(isset($options['data']) && $options['data'] && $options['data']->getName() !== null) $builder->add('Smazat', SubmitType::class);

    }
}