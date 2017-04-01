# line-analyze-rev

## Исходный код программы, использующейся для расчетов при решении кейса для чемпионата CASE-IN 2017 в электротехнической лиге

### Предупреждение

Так как чемпионат еще не закончился, код специально приведен не полностью, чтобы отсутствовала возможность его легкого переиспользования командами соперников.

### Кратко

Исходные данные для рассчетов представляли собой информацию о режимах (напряжение и передаваемая мощность) для нескольких линий, собранные в единую таблицу. Информация приводилась за каждый час, всего были отражены данные за год. Результатом должна была стать информация о структуре техническких потерь (потерь на передачу энергии в линии) и потери на корону.

### Решение

После продолжительного мозгового штурма силами всей команды, мной была написана программа на C++, которая считывала необходимую структуру и параметры линий из конфигурационных файлов (см. dist/description220.txt, dist/description500.txt, dist/wires.txt), а затем производила расчет. 

### Использование

> ./line-analyze-rev [файл описания]

### Формат входных данных

Входные данные должны представлять собой файл в формате CSV, структурированный следующим образом:

Колонка 1: время в формате Д/М/Г ЧЧ:ММ:СС

Колонка(-и) 2-n: напряжение на системе шин

Колонки n-k: активная и реактивная мощность, передаваемая по линии i

### Зависимости

Программа использует следующие библиотеки: xlswriter, zlib, minizip, tmpfileplus

### Связь

danil.kamyshov@gmail.com, https://vk.com/alhayat
