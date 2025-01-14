<?php

namespace App\Type;

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

class EmployeeType extends AbstractType
{
    public function buildForm(FormBuilderInterface $builder, array $options): void
    {
        $builder
            ->add('firstName', TextType::class)
            ->add('lastName', TextType::class)
            ->add('positions', EntityType::class, [
                'class' => Role::class,
                'choice_label' => 'title',
                'multiple' => true,
                'expanded' => true
            ])
            ->add('telephone', TextType::class)
            ->add('email', TextType::class)
            ->add('webPage', TextType::class)
            ->add('info', TextType::class)
            ->add('Save', SubmitType::class);

            if($options['data'] && $options['data']->getFirstName() !== null) $builder->add('Smazat', SubmitType::class);
    }
}