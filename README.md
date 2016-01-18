HandlerSocket
=============

**NOTE: DO NOT USE THIS YET. I DO NOT KNOW IF IT WILL BE FINISHED !!**

__WIP__: This is a WIP PHP 7 [HandlerSocket](http://yoshinorimatsunobu.blogspot.co.uk/search/label/handlersocket) extension

API
===
*Also WIP ...*

```php
namespace HandlerSocket {

	class Host {

		public function __construct(string $address);
	}

	class Connection {

		public function __construct(Host $host, int $port = 9998);

		public function authenticate(string $secret) : bool;

		public function openIndex(int 	 	$id,
								  string	$db, 
								  string 	$tbl,
								  array  	$cols = [],
								  int 	 	$type = INDEX::PRIMARY) : Index;
	}

	class Index {

		public function find(mixed $match, int $op = INDEX::EQ) : Result;

		const EQ;
		const LT;
		const LTE;
		const GT;
		const GTE;

		const PRIMARY;
	}

	class Result {
		
	}
}
```

Example
======
*Also WIP ...*

```php
<?php
use HandlerSocket\Host;
use HandlerSocket\Connection;
use HandlerSocket\Index;

$host = new Host("127.0.0.1");
$connection = new Connection($host);
$index = $connection->openIndex(1, "hs", "test", [
	"name",
	"email",
	"created"
]);

/* SELECT name,email,created FROM hs.test WHERE id = 1; */
$result = $index->find(1);

var_dump($host, $connection, $index, $result, $result->name);
?>
```

TODO
====

 - everything
 - no really, everything ...
