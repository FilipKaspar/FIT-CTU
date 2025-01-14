<?php declare(strict_types=1);

namespace BIPHP\Application;

use BIPHP\Entity\Product;
use BIPHP\Entity\ProductResult;

use BIPHP\Entity\RatingResult;
use Symfony\Component\BrowserKit\HttpBrowser;
use Symfony\Component\DomCrawler\Crawler;


/**
 * This class represents your scrapping application
 */
class App
{
    /** @var ProductResult[] */
    private array $productResults = [];
    private array $otherResults = [];
    private float $totalPrice = 0;


    // Implement me please :)

    public function run(): void
    {
        $baseURL = "https://bi-php.urbanec.cz/";
        $blackListItems = ["Bag for Samsung (black)", "Garmin Venu 2 Plus Slate/Black Band"];

        $client = new HttpBrowser();
        $crawler = $client->request('GET', 'https://bi-php.urbanec.cz/products/search?form%5Bsearch%5D=phone');

        $crawler = $crawler->filter('.container')->eq(1)->filter('.col');

        foreach($crawler as $ctn => $content){
            $profile = new Crawler($content);
            $name = $profile->filter('.card-title')->text();

            $price = (float)str_replace("Price: ", "", $profile->filter('.card-text')->eq(1)->text());
            $this->totalPrice += $price;
            $priceBeforeSale = null;
            $link = $profile->filter('.btn')->attr('href');
            $id = (int)str_replace("/product-detail/", "", $link);

            $priceClient = new HttpBrowser();
            $priceCrawler = $priceClient->request('GET', $baseURL . $link);

            $description = $priceCrawler->filter('#desc')->text();
            $previousPrice = $priceCrawler->filter('#total_price');
            if($previousPrice->count() > 0){
                $tmpPrice = (float)str_replace("Total price: ", "", $previousPrice->text());
                if($tmpPrice !== $price) $priceBeforeSale = $tmpPrice;
            }

            $ratings = [];
            $priceCrawler = $priceCrawler->filter('.border-warning-subtle')->filter('.card-body')->eq(0)->filter('.card-body');
            foreach ($priceCrawler as $ir => $reviewContent){
                $reviewCrawler = new Crawler($reviewContent);
                $userName = $reviewCrawler->filter(".card-title")->text();
                $userComment = $reviewCrawler->filter(".card-text")->text();
                $userRating = (int)$reviewCrawler->filter("#rating-score")->text();

//                echo $ir . ":\n" . "$userName\n$userComment\n$userRating" . "\n";
                $ratings[] = new RatingResult($userName, $userComment, $userRating);
            }
            array_shift($ratings);
//            echo count($reviews) . "\n";
//            echo $ctn . ":\n" . "$id\n$name\n$description\n$price\n$priceBeforeSale\n$link\n" . "\n";

            $newProduct = new ProductResult($ratings, $id);
            $newProduct->setName($name);
            $newProduct->setDescription($description);
            $newProduct->setTotalPrice($price);
            $newProduct->setTotalPriceWithoutDiscount($priceBeforeSale);
            $newProduct->setLink($link);

            if(in_array($name, $blackListItems) || $price > 25000){
                $this->otherResults[] = $newProduct;
                continue;
            }

            $inserted = false;
            $position = 0;
            foreach ($this->productResults as $product){
                if($product->getTotalPrice() > $newProduct->getTotalPrice()){
                    array_splice($this->productResults, $position, 0, [$newProduct]);
                    $inserted = true;
                    break;
                }
                $position++;
            }

            if(empty($this->productResults) || !$inserted) $this->productResults[] = $newProduct;

//            print_r($this->productResults);
        }

    }


    /**
     * Method which returns all products which we search. Products are ordered by total price.
     * @return ProductResult[]
     */
    public function getProductResults(): array
    {
        return $this->productResults;
    }

    private function addToArray($productArray, &$arrayToInput): void
    {
        foreach ($productArray as $product){
            $totalRating = 0;
            foreach ($product->getRatings() as $rating){
                $totalRating += $rating->getRating();
            }
            $arrayToInput [$product->getId()] = $totalRating/count($product->getRatings());
        }
    }

    /**
     * Method which returns averages for all products we have found form web.
     * @return array<int, float> - it is an array of averages. Product id is the key.
     * @example ['1' => 55.4, '2' => 67.8, '3' => 79,3 ... ]
     */
    public function getProductsRatingAvg(): array
    {
        $averages = [];

        $this->addToArray($this->productResults, $averages);
        $this->addToArray($this->otherResults, $averages);

//        print_r($averages);

        return $averages;
    }
}
