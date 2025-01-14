<?php declare(strict_types=1);

namespace Star;

use Symfony\Component\Console\Attribute\AsCommand;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

#[AsCommand(name: 'star')]
class StarCommand extends Command
{
    protected function configure(): void
    {
        $this->setName("Star drawer")
            ->setDescription('Program for drawing stars based on provided arguments.')
            ->setHelp('If u wanna draw stars provide arguments');

        $this->addArgument('width', InputArgument::REQUIRED, 'Input image width')
            ->addArgument('color',  InputArgument::REQUIRED, 'Input star color')
            ->addArgument('points',  InputArgument::REQUIRED, 'Input number of points on the star')
            ->addArgument('radius',  InputArgument::REQUIRED, 'Input radius between each tip')
            ->addArgument('output',  InputArgument::REQUIRED, 'Input name of output file')
            ->addArgument('bgColor',  InputArgument::OPTIONAL, 'Input background color')
            ->addArgument('borderColor',  InputArgument::OPTIONAL, 'Input color of the border')
            ->addArgument('borderWidth',  InputArgument::OPTIONAL, 'Input width of the border');
    }

    public function getColorFromString($image, $stringColor): int {
        $hexNumber = dechex((int)$stringColor);
        $hexNumber = str_pad($hexNumber, 6, "0", STR_PAD_LEFT);
        $red = substr($hexNumber, 0,2);
        $green = substr($hexNumber, 2, 2);
        $blue =  substr($hexNumber, 4, 2);

        echo "$red\n$green\n$blue\n";
        return imagecolorallocate($image, hexdec($red), hexdec($green), hexdec($blue));
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $width = (int)$input->getArgument("width");
        $color = $input->getArgument("color");
        $pointsAmount = (int)$input->getArgument("points");
        $radius = (float)$input->getArgument("radius");
        $outputName = $input->getArgument("output");
        $outputFile = pathinfo($outputName, PATHINFO_EXTENSION) == 'png' ? $outputName : $outputName . '.png';
        $bgColor = $input->getArgument("bgColor") ?? hexdec("FFFFFF");
        $borderColor = $input->getArgument("borderColor");
        $borderWidth = (int)$input->getArgument("borderWidth");

        if(!$borderColor || !$borderWidth) $borderWidth = 0;

        $image = imagecreatetruecolor($width, $width);
        imagefill($image, 0, 0, $this->getColorFromString($image, $bgColor));

        $baseOffset = $width/2;
        $baseBorderOffset = $width/2 - $borderWidth;
        $starPoints = [];
        $starPointsBorder = [];

        $angleStart = -90;
        $angleEnd = 360 + $angleStart;
        $angleStep = 360 / ($pointsAmount * 2);
        $innerPoint = false;

        for ($a = $angleStart; $a < $angleEnd; $a += $angleStep) {
            $r = $innerPoint ? $radius : 1;

            $starPoints[] = $baseOffset + ($baseBorderOffset * $r * cos(deg2rad($a)));
            $starPoints[] = $baseOffset + ($baseBorderOffset * $r * sin(deg2rad($a)));

            $starPointsBorder[] = $baseOffset + (($baseBorderOffset + $borderWidth) * $r * cos(deg2rad($a)));
            $starPointsBorder[] = $baseOffset + (($baseBorderOffset + $borderWidth) * $r * sin(deg2rad($a)));

            $innerPoint = !$innerPoint;
        }
        if($borderWidth !== 0) imagefilledpolygon($image, $starPointsBorder, $this->getColorFromString($image, $borderColor));
        imagefilledpolygon($image, $starPoints, $this->getColorFromString($image, $color));

        imagepng($image, $outputFile);

        return 0;
    }
}
