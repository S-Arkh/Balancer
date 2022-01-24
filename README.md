Балансирует нагрузку по заданным параметрам.
Пример файла конфигурации находится в config/config.txt

Суть алгоритма: просто по кругу загружает сервера.

Сборка:
1) Создать в корне проекта каталог build
2) Перейти в каталог build
3) Выполнить cmake ..
4) Выполнить make

Запуск:
1) Находясь в каталоге build выполнить ./Balancer <путь до файла конфигурации>, пример ./Balancer ../config/config.txt

Для проверки можно воспользоваться https://github.com/S-Arkh/BalancerSender.git и https://github.com/S-Arkh/BalancerReceiver.git
