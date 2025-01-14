<?php
namespace Books\Middleware;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Psr\Http\Server\MiddlewareInterface;
use Psr\Http\Server\RequestHandlerInterface as RequestHandler;

class SecurityMiddleware implements MiddlewareInterface
{
    public function process(Request $request, RequestHandler $handler): Response
    {
        if(!$request->hasHeader('Authorization') || empty($request->getHeader('Authorization'))){
            return (new \Slim\Psr7\Response())->withStatus(401);
        }

        $header = $request->getHeader('Authorization')[0];
        $header = str_replace("Basic ", "", $header);
        $header = base64_decode($header);
        if($header != "admin:pas\$word") return (new \Slim\Psr7\Response())->withStatus(401);
        return $handler->handle($request);
    }
}