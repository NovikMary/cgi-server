	CGI-server

	1. Реализован веб-сервер, который умеет выдавать статические страницы и обслуживать одновременно несколько подключений.
	2. Cервер умеет выполнять сторонние программы и выдавать результат их работы.
	3. Использование как многопроцессной, так и многопоточной работы для обеспечения оптимального использования системных ресурсов.

	Для сборки используется команда "make".
	Для запуска -- команда "./server.out port folder", где port -- это номер порта, а folder -- путь к папке со страницами сайта из текущей директории. По умолчанию port = 3425, а путь к папке -- "/pages".
