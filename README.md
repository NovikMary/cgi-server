	В конечном итоге, мой проект должен представлять собой сервер, который выдает статические и динамические 
страницы, умеет обслуживать одновременно несколько пользователей. Cgi программа в этом проекте генерирует html 
страницы, которые затем отображаются пользователю. Вполне возможно, что для разных пользователей одни и те же 
запросы будут генерировать различные html страницы. То есть страница формируется на основе некоторых действий 
пользователя. 
	Существует два способа посылки параметров запроса GET и POST. В первом случае, параметры помещаются
в URL страницы. Например, поисковики чаще всего используют именно этот способ посылки запросов: вы можете ввести
свой запрос в окне ввода данных, а можете просто дописать в URL страницы после символов "...=". Второй метод 
используется в социальных сетях, почтах и прочих сервисах, которые используют идентификацию пользователя. Именно
благодаря этому методу два различных пользователя запросив один и тот же URL могут получить абсолютно разные 
страницы.
	Описание основного принципа работы cgi-сервера:
1. Клиент посылает данные, обычно с помощью формы, веб-серверу.
2. Сервер выполняет CGI-программу, передавая ей данные.
3. CGI-программа осуществляет свою обработку и возвращает свои выходные данные серверу.
4. Сервер передает выходные данные клиенту.  
	На данный момент реализован сервер, способный одновременно обрабатывать одно подключение, получающий по TCP 
соединению имя, и возвращающий обратно приветствие.

