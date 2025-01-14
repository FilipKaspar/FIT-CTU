<?php declare(strict_types=1);

namespace Books\Rest;

use Books\Database\DB;
use Books\Middleware\JsonBodyParserMiddleware;
use Books\Middleware\SecurityMiddleware;
use FontLib\Table\Type\name;
use PDO;
use Slim\App;
use Slim\Factory\AppFactory;
use Slim\Handlers\Strategies\RequestHandler;
use Slim\Psr7\Request;
use Slim\Psr7\Response;

class RestApp
{
    private ?App $app = null;
    private ?PDO $db = null;

    public function configure(): void
    {
        $this->app = AppFactory::create();
        $this->db = DB::getConnection();

        $this->app->addRoutingMiddleware();
        $this->app->addErrorMiddleware(true, true, true);
        $this->app->add(new JsonBodyParserMiddleware());

        $this->app->get('/books', function (Request $request, Response $response) {
            $stmt = $this->db->query('SELECT id, name, author FROM books');
            $records = $stmt->fetchAll(PDO::FETCH_ASSOC);
            $payload = json_encode($records);
            $response->getBody()->write($payload);
            return $response->withHeader('Content-Type', 'application/json');
        });

        $this->app->get('/books/{id}', function (Request $request, Response $response, array $args) {
            $bookId = $args['id'];
            if(!is_numeric($bookId) || !ctype_digit($bookId)) return $response->withStatus(400);

            $record = $this->findRecordById($bookId);
            if(!$record) return $response->withStatus(404);

            $payload = json_encode($record);
            $response->getBody()->write($payload);
            return $response->withHeader('Content-Type', 'application/json');
        });

        $this->app->post('/books', function (Request $request, Response $response) {
            $body = $request->getParsedBody();

            if(!isset($body['name'], $body['author'], $body['publisher'], $body['isbn'],$body['pages'])) {
                return $response->withStatus(400);
            }

            $stmt = $this->db->prepare('INSERT INTO books (name, author, publisher, isbn, pages) VALUES (:name, :author, :publisher, :isbn, :pages)');
            $this->bindAllBookInfo($stmt, $body['name'], $body['author'], $body['publisher'], $body['isbn'], $body['pages']);
            $stmt->execute();

            return $response->withHeader('Location', "/books/{$this->db->lastInsertId()}")->withStatus(201);
        })->add(new SecurityMiddleware());

        $this->app->put('/books/{id}', function (Request $request, Response $response, array $args) {
            $bookId = $args['id'];
            if(!is_numeric($bookId) || !ctype_digit($bookId)) return $response->withStatus(400);

            $record = $this->findRecordById($bookId);
            if(!$record) return $response->withStatus(404);

            $body = $request->getParsedBody();
            if(!isset($body['name'], $body['author'], $body['publisher'], $body['isbn'],$body['pages'])) {
                return $response->withStatus(400);
            }

            $stmt = $this->db->prepare('UPDATE books SET name = :name, author = :author, publisher = :publisher, isbn = :isbn, pages = :pages WHERE id = :id');
            $stmt->bindParam(':id', $bookId);
            $this->bindAllBookInfo($stmt, $body['name'], $body['author'], $body['publisher'], $body['isbn'], $body['pages']);
            $stmt->execute();

            return $response->withStatus(204);
        })->add(new SecurityMiddleware());

        $this->app->delete('/books/{id}', function (Request $request, Response $response, array $args) {
            $bookId = $args['id'];
            if(!is_numeric($bookId) || !ctype_digit($bookId)) return $response->withStatus(400);

            $record = $this->findRecordById($bookId);
            if(!$record) return $response->withStatus(404);

            $stmt = $this->db->prepare('DELETE FROM books WHERE id = :id');
            $stmt->bindParam(':id', $bookId);
            $stmt->execute();

            return $response->withStatus(204);
        })->add(new SecurityMiddleware());

    }

    public function findRecordById($bookId){
        $stmt = $this->db->prepare('SELECT * FROM books WHERE id = :id');
        $stmt->bindParam(':id', $bookId, PDO::PARAM_INT);
        $stmt->execute();

        return $stmt->fetch(PDO::FETCH_ASSOC);
    }

    public function bindAllBookInfo($stmt, $name, $author, $publisher, $isbn, $pages){
        $stmt->bindParam(':name', $name);
        $stmt->bindParam(':author', $author);
        $stmt->bindParam(':publisher', $publisher);
        $stmt->bindParam(':isbn', $isbn);
        $stmt->bindParam(':pages', $pages, PDO::PARAM_INT);
    }

    public function run(): void {
        $this->app->run();
    }

    public function getApp(): App {
        return $this->app;
    }
}
