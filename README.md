	CGI-server

	1. Реализован веб-сервер, который умеет выдавать статические страницы и обслуживать одновременно несколько подключений.
	2. Cервер умеет выполнять сторонние программы и выдавать результат их работы.
	3. Использование как многопроцессной, так и многопоточной работы для обеспечения оптимального использования системных ресурсов.

	Для сборки используется команда "make".
	Для запуска -- команда "./server.out port folder threads", где port -- это номер порта, а folder -- путь к папке со страницами сайта из текущей директории, threads -- количество обрабатываемых параллельно клиентов. По умолчанию port = 3425, путь к папке -- "/pages", threads = std::thread::hardware_concurrency(). Внимание: большое значение threads может значительно замедлить работу сервера.
